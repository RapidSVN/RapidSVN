/*
 * ====================================================================
 * Copyright (c) 2002-2009 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.  
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxWidgets
#include "wx/wx.h"
#include "wx/filename.h"
#include "wx/filefn.h"
#include "wx/dir.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/wc.hpp"
#include "svncpp/path.hpp"
#include "svncpp/status.hpp"

// app
#include "drag_n_drop_action.hpp"
#include "dnd_dlg.hpp"
#include "commit_dlg.hpp"
#include "utils.hpp"

class DragAndDropImportTraverser : public wxDirTraverser
{
public:
  DragAndDropImportTraverser(const wxString& rootSrcDir,
                             const wxString& destDir)
  {
    m_rootSrcDir = rootSrcDir;
    m_destDir = destDir;

    // Create the initial destionation directory
    wxString newDirectory = wxFileName(m_destDir).GetFullPath();
    if (::wxDirExists(newDirectory) == false)
    {
      ::wxMkdir(newDirectory);
    }
  }

  virtual wxDirTraverseResult OnFile(const wxString& filename)
  {
    // ::wxCopyFile (filename, (destDir + dirname - rootSrcDir))
    ::wxCopyFile(filename, ConvertToDestinationPath(filename));
    return wxDIR_CONTINUE;
  }

  virtual wxDirTraverseResult OnDir(const wxString& dirname)
  {
    // ::wxMkDir (destDir + (dirname - rootSrcDir))
    wxString newDirectory = ConvertToDestinationPath(dirname);
    if (::wxDirExists(newDirectory) == false)
    {
      ::wxMkdir(newDirectory);
    }
    return wxDIR_CONTINUE;
  }

private:

  wxString ConvertToDestinationPath(const wxString& fullPath)
  {
    return wxFileName(m_destDir + wxFileName::GetPathSeparator() +
                      fullPath.Mid(m_destDir.Len()-1)).GetFullPath();
  }

  wxString m_rootSrcDir;
  wxString m_destDir;
};

DragAndDropAction::DragAndDropAction(wxWindow * parent,
                                     DragAndDropData & data)
    : Action(parent, wxEmptyString, 0),
    m_parent(parent)
{
  m = new DragAndDropData();
  m->m_files = data.m_files;
  m->m_destination = data.m_destination;
  m->m_action = DragAndDropDlg::RESULT_CANCEL;
  m->m_logMessage = wxEmptyString;
  m->m_recursiveAdd = false;
}

DragAndDropAction::~DragAndDropAction()
{
  delete m;
}

bool
DragAndDropAction::IsInSameTree(const wxString & srcPath, const wxString & destPath)
{
  //Detect a common part of both path as base path.
  // e.g.
  //               WC                    Repository
  // src:  /home/foo/wc/filename    http://foo/trunk/filename
  // dest: /home/foo/wc/folder      http://foo/trunk/folder
  // base: /home/foo/wc             http://foo/trunk/

  size_t limit = srcPath.Len();
  size_t destlen = destPath.Len();
  if (destlen < limit)
    limit = destlen;

  size_t i;
  for (i = 0; i < limit; i++)
  {
    if (srcPath[i] != destPath[i])
      break;
  }
  const wxString matching = srcPath.SubString(0, i);
  int last_slash = matching.Find(wxT('/'), true);
#ifdef __WXMSW__
  if (last_slash == wxNOT_FOUND)
    last_slash = matching.Find(wxT('\\'), true);
#endif
  if (last_slash == wxNOT_FOUND)
    return false;
  const svn::Path base = PathUtf8(matching.SubString(0, last_slash-1));

  // 1. If base path is local folder which is not WC, srcPath and destPath
  //    are in different WC folders each other.
  if (!base.isUrl() && !svn::Wc::checkWc(base.c_str()))
    return false;

  return true;
}

bool
DragAndDropAction::Prepare()
{
  if (!Action::Prepare())
  {
    return false;
  }

  wxString src = wxEmptyString;
  if (m->m_files.GetCount() > 1)
    src = _("Multiple Files");
  else if (m->m_files.GetCount() == 1)
    src = m->m_files[0];
  else
    return false; // No files dragged

  // If the ctrl key is down, then assume the user
  //  wants to copy rather than move files
  bool showMoveButton = (::wxGetKeyState(WXK_CONTROL) == false);

  // Check if the file being dragged-and-dropped is already
  // under source control or is being imported into the repository
  svn::Path srcSvnPath(PathUtf8(m->m_files[0]));
  svn::Path destSvnPath(PathUtf8(m->m_destination));

  bool importFiles = false;
  bool incrementRevision = false;

  // ==DragDrop action patterns==
  // Any entry    -> Same folder        : Nothing to be done
  // Any folder   -> Itself             : Unexpected!
  // Repository   -> Same repository    : Copy/Move action (immediate commit)
  // Repository   -> Other repository   : Unexpected!
  // WC folder    -> Repository         : Unexpected!
  // WC file      -> Repository         : Import action (immediate commit)
  // Plain file   -> Repository         : Import action (immediate commit)
  // Repository   -> WC folder          : Unexpected!
  // Any wc entry -> Same working copy  : Copy/Move action (uncommitied)
  // WC folder    -> Other working copy : Unexpected!
  // WC file      -> Other working copy : Add action (called as import but uncommitied)
  // Plain file   -> WC folder          : Add action (called as import but uncommitied)

  wxArrayString::iterator it;
  for (it = m->m_files.begin(); it != m->m_files.end(); it++)
  {
    if (Utf8ToLocal(PathUtf8(*it).dirpath()) ==
        Utf8ToLocal(PathUtf8(m->m_destination).c_str()))
      return false;
    if (*it == m->m_destination)
      return false;
  }

  if (destSvnPath.isUrl())
  {
    //Drop into repository.

    if (srcSvnPath.isUrl())
    {
      if (!IsInSameTree(m->m_files[0], m->m_destination))
        return false;

      importFiles = false;
      incrementRevision = true;
    }
    else
    {
      for (it = m->m_files.begin(); it != m->m_files.end(); it++)
      {
        if (svn::Wc::checkWc(PathUtf8(*it)))
          return false;
      }

      importFiles = true;
      incrementRevision = true;
    }
  }
  else
  {
    //Drop into working copy.

    if (srcSvnPath.isUrl())
      return false;
    else if (svn::Wc::checkWc(srcSvnPath.dirpath()) &&
             IsInSameTree(m->m_files[0], m->m_destination))
    {
      importFiles = false;
      incrementRevision = false;
    }
    else {
      for (it = m->m_files.begin(); it != m->m_files.end(); it++)
      {
        if (svn::Wc::checkWc(PathUtf8(*it)))
          return false;
      }

      importFiles = true;
      incrementRevision = false;
    }
    m->m_recursiveAdd = true;
  }

  int possibleActions;
  if (importFiles)
    possibleActions = DragAndDropDlg::IMPORT;
  else if (showMoveButton)
    possibleActions = DragAndDropDlg::COPY_MOVE;
  else
    possibleActions = DragAndDropDlg::COPY;

  // Present the confirmation dialog to the user
  DragAndDropDlg dlg(m_parent, src, m->m_destination,
                     possibleActions);
  m->m_action = dlg.ShowModal();

  if (DragAndDropDlg::RESULT_CANCEL == m->m_action)
    return false;

  // Imports require additional information from the user
  //  so present the commit dialog to get that information
  if (DragAndDropDlg::RESULT_IMPORT == m->m_action && incrementRevision)
  {
    CommitDlg commitDlg(m_parent);
    if (commitDlg.ShowModal() != wxID_OK)
      return false;

    m->m_logMessage = commitDlg.GetMessage();
    m->m_recursiveAdd = commitDlg.GetRecursive();
  }

  return true;
}

bool
DragAndDropAction::Perform()
{
  ::wxBusyCursor();
  svn::Revision unusedRevision;
  svn::Client client(GetContext());

  wxString msg = wxEmptyString;
  for (unsigned int i=0; i<m->m_files.GetCount(); i++)
  {
    svn::Path srcPath = PathUtf8(m->m_files [i]);
    wxFileName srcFilename(m->m_files[i]);
    svn::Path destPath = PathUtf8(m->m_destination);
    destPath.addComponent(LocalToUtf8(srcFilename.GetFullName()));
    switch (m->m_action)
    {
    case DragAndDropDlg::RESULT_COPY:
      msg.Printf(_("Copying file: %s"), PathToNative(srcPath).c_str());
      Trace(msg);
      client.copy(srcPath, unusedRevision, destPath);
      break;
    case DragAndDropDlg::RESULT_MOVE:
      msg.Printf(_("Moving file: %s"), PathToNative(srcPath).c_str());
      Trace(msg);
      client.move(srcPath, unusedRevision, destPath, false);
      break;
    case DragAndDropDlg::RESULT_IMPORT:
      // Imports only work when the destination directory is a URL.
      // For imports into a WC, copy the files to the WC and mark
      //  them to be added to the repository
      if (destPath.isUrl())
      {
        msg.Printf(_("Importing file into repository: %s"),
                   PathToNative(srcPath).c_str());
        Trace(msg);
        client.import(srcPath, destPath,
                      LocalToUtf8(m->m_logMessage).c_str(),
                      srcFilename.IsDir() && m->m_recursiveAdd);
      }
      else
      {
        // If the src file is just a file, then use ::wxCopyFile
        //  otherwise is wxDirTraverser to copy the whole directory
        msg.Printf(_("Copying file into working directory: %s"),
                   PathToNative(srcPath).c_str());
        Trace(msg);
        if (::wxDirExists(m->m_files [i]))
        {
          msg.Printf(_("Adding directory to repository: %s"),
                     PathToNative(srcPath).c_str());
          wxDir dir(m->m_files [i]);
          if (dir.IsOpened())
          {
            DragAndDropImportTraverser dirTraverser(
              m->m_files [i], PathToNative(destPath));
            dir.Traverse(dirTraverser);
          }
        }
        else
        {
          wxString srcNative(PathToNative(srcPath));
          msg.Printf(_("Adding file to repository: %s"),
                     srcNative.c_str());
          ::wxCopyFile(srcNative,
                       PathToNative(destPath));
        }
        Trace(msg);
        client.add(destPath, m->m_recursiveAdd);
      }
      break;
    case DragAndDropDlg::RESULT_CANCEL:
      break;
    default:
      break;
    };
  }

  return true;
}

bool
DragAndDropAction::CheckStatusSel(const svn::StatusSel & WXUNUSED(statusSel))
{
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
