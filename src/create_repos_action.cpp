/*
 * ====================================================================
 * Copyright (c) 2002-2010 The RapidSvn Group.  All rights reserved.
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

// app
#include "action_event.hpp"
#include "create_repos_action.hpp"
#include "create_repos_dlg.hpp"
#include "ids.hpp"
#include "svn_executables.hpp"

CreateRepositoryAction::CreateRepositoryAction(wxWindow * parent)
  : Action(parent, _("Create Repository"), 0)
{
}

CreateRepositoryAction::~CreateRepositoryAction()
{
}

bool
CreateRepositoryAction::Prepare()
{
  CreateReposDlg dlg(GetParent());

  if (dlg.ShowModal() != wxID_OK)
    return false;

  // now we have to build the svnadmin commandline options
  // from the dialog fields
  wxString cmd(SVNADMIN_EXECUTABLE);
  cmd += wxT(" create");

  switch (dlg.GetType())
  {
  case CreateReposDlg::TYPE_FSFS:
    cmd += wxT(" --fs-type fsfs");
    break;
  case CreateReposDlg::TYPE_BDB:
    cmd += wxT(" --fs-type bdb");
    break;
  default:
    // Unknown type
    return false;
  }

  switch (dlg.GetCompat())
  {
  case CreateReposDlg::COMPAT_PRE_1_6:
    cmd += wxT(" --pre-1-6-compatible");
    break;
  case CreateReposDlg::COMPAT_PRE_1_5:
    cmd += wxT(" --pre-1-5-compatible");
    break;
  case CreateReposDlg::COMPAT_PRE_1_4:
    cmd += wxT(" --pre-1-4-compatible");
    break;
  default:
    ; // No default option for this option
  }

  wxString configDir(dlg.GetConfigDir());
  if (!configDir.IsEmpty())
    cmd += wxString::Format(wxT(" --configDir \"%s\""), configDir.c_str());


  if (dlg.GetBdbLogKeep())
    cmd += wxT(" --bdb-log-keep");

  if (dlg.GetBdbTxnNoSync())
    cmd += wxT(" --bdb-txn-nosync");

  wxString filename(dlg.GetFilename());
  cmd += wxString::Format(wxT(" \"%s\""), filename.c_str());

  bool addBookmark = dlg.GetAddBookmark();

  // we can run svnadmin from the main thread as this should
  // not take too long (< 1 second)
  Trace(wxString::Format(_("Running command %s:"), cmd.c_str()));
  wxArrayString output;

  int result = ::wxExecute(cmd, output, output, wxEXEC_SYNC);

  if (result < 0)
  {
    Trace(_("Error running svnadmin"));
    return false;
  }

  wxArrayString::const_iterator it;
  for (it=output.begin(); it != output.end(); it++)
    Trace(*it);

  if (result > 0)
  {
    Trace(_("The svnadmin command was not successful."));
    return false;
  }

  
  // now add a bookmark
  if (addBookmark)
  {
    // this works for simple bookmarks like "/foo/bar"
    // or "c:/foo". 
    // TODO check and implement this for UNC paths on
    // windows as well, e.g. "\\server\foo\bar"
    wxString repoBookmark(wxT("file://"));
    if (!filename.StartsWith(wxT("/")))
      repoBookmark += wxT("/");

    // we need / instead of a backslash
    wxFileName intern(filename);
    repoBookmark += intern.GetFullPath(wxPATH_UNIX);

    ActionEvent::Post(GetParent(), TOKEN_ADD_BOOKMARK, repoBookmark);
  }


  return true;
}

bool
CreateRepositoryAction::Perform()
{
  return true;
}


bool
CreateRepositoryAction::CheckStatusSel(const svn::StatusSel & WXUNUSED(statusSel))
{
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
