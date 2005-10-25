/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wx
#include "wx/wx.h"
#include "wx/filename.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/entry.hpp"
#include "svncpp/exception.hpp"
#include "svncpp/status.hpp"
#include "svncpp/url.hpp"

// app
#include "diff_action.hpp"
#include "diff_data.hpp"
#include "diff_dlg.hpp"
#include "ids.hpp"
#include "preferences.hpp"
#include "utils.hpp"


struct DiffAction::Data 
{
private:
  Action * mAction;

public:
  bool showDialog;
  DiffData diffData;


  Data (Action * action)
    : mAction (action), showDialog (true)
  {
  }


  Data (Action * action, DiffData & data)
    : mAction (action), showDialog (false), diffData (data)
  {
  }


  svn::Context * 
  GetContext ()
  {
    return mAction->GetContext ();
  }


  void
  Trace (const wxString & msg)
  {
    mAction->Trace (msg);
  }


  /**
   * retrieves the status information for a file
   */
  svn::Status 
  getStatus (const svn::Path & path)
  {
    svn::Client client (GetContext ());
    svn::Status status (client.singleStatus (path.c_str ()));

    return status;
  }


  /**
   * retrieves the revision information for a file
   */
  svn::Revision
  getRevision (const svn::Path & path, const svn::Status & status) 
  {
    svn::Entry entry (status.entry ());
    return entry.revision ();
  }


  /**
   * retrieves the effective path for the first file
   */
  svn::Path 
  getPath1 (const svn::Path & path)
  {
    if (diffData.useUrl1)
    {
      std::string url1Utf8 (LocalToUtf8 (diffData.url1));
      return svn::Path (url1Utf8);
    }
    return path;
  }


  /**
   * retrieves the effective path for the second file
   */
  svn::Path 
  getPath2 (const svn::Path & path)
  {
    if (diffData.useUrl2)
    {
      std::string url2Utf8 (LocalToUtf8 (diffData.url1));
      return svn::Path (url2Utf8);
    }
    else
      return path;
  }


  /** 
   * Check the status of a file. Only regular
   * versioned files will pass this check
   */
  bool
  checkStatus (const svn::Status & status)
  {
    if (!status.isVersioned ())
      return false;

    if (status.entry ().kind () != svn_node_file)
      return false;

    return true;
  }


  /**
   * performs the diff operation on a single path:
   *
   * 0. check the path; skip directories and
   *    unversioned entries
   *
   * 1. fetch the first file (unless we want to
   *    compare with the working copy to
   *    a temporary filename
   *
   * 2. fetch the second file to a temporary filename
   *
   * 3. run the diff tool
   */
  void 
  diffTarget (const svn::Path & path)
  {
    svn::Path dstFile1, dstFile2;
    svn::Status status (getStatus (path));

    if (!checkStatus (status))
    {
      wxString msg, wxpath (Utf8ToLocal (path.c_str ()));
      msg.Printf (_("Skipped: %s"), wxpath.c_str());
      Trace (msg);
      return;
    }


    switch (diffData.compareType)
    {
    case DiffData::WITH_BASE:
      dstFile1 = path;
      dstFile2 = mAction->GetPathAsTempFile (getPath1 (path), svn::Revision::BASE);

      break;

    case DiffData::WITH_HEAD:
      dstFile1 = path;
      dstFile2 = mAction->GetPathAsTempFile (getPath1 (path), svn::Revision::HEAD);

      break;

    case DiffData::WITH_DIFFERENT_REVISION:
      dstFile1 = path;
      dstFile2 = mAction->GetPathAsTempFile (getPath1 (path), diffData.revision1);

      break;

    case DiffData::TWO_REVISIONS:
      dstFile1 = mAction->GetPathAsTempFile (getPath1 (path), diffData.revision1);
      dstFile2 = mAction->GetPathAsTempFile (getPath2 (path), diffData.revision2);

      break;

    default:
      // do nothing!
      return;
    }

    // prepare command line to execute
    Preferences prefs;
    wxString args (prefs.diffToolArgs);
    wxString dstFile1Native (Utf8ToLocal (dstFile1.native ().c_str ()));
    wxString dstFile2Native (Utf8ToLocal (dstFile2.native ().c_str ()));

    TrimString (args);

    if (args.Length () == 0)
      args.Printf (wxT("\"%s\" \"%s\""), dstFile1Native.c_str (), 
                   dstFile2Native.c_str ());
    else
    {
      args.Replace (wxT("%1"), dstFile1Native.c_str (), true);
      args.Replace (wxT("%2"), dstFile2Native.c_str (), true);
    }

    wxString cmd (prefs.diffTool + wxT(" ") + args);

    wxString msg;
    msg.Printf (_("Execute diff tool: %s"), cmd.c_str ());
    Trace (msg);
    wxExecute (cmd);
  }
};


DiffAction::DiffAction (wxWindow * parent)
  : Action (parent, _("Diff"), GetBaseFlags ())
{
  m = new Data (this);
}


DiffAction::DiffAction (wxWindow * parent, DiffData & data)
  : Action (parent, _("Diff"), GetBaseFlags ())
{
  m = new Data (this, data);
}


DiffAction::~DiffAction ()
{
  delete m;
}


bool
DiffAction::Prepare ()
{
  if (!Action::Prepare ())
    return false;

  if (m->showDialog)
  {
    // check the first target and try to
    // determine whether we are on a local
    // or remote location
    svn::Path target = GetTarget ();
    bool isRemote = svn::Url::isValid (target.c_str ());

    DiffDlg dlg (GetParent ());

    size_t count = GetTargets ().size ();

    if (count != 1)
      dlg.EnableUrl (false);

    if (isRemote)
    {
      DiffData::CompareType types [] = {
        DiffData::TWO_REVISIONS
      };

      dlg.AllowCompareTypes (types, WXSIZEOF (types));
    }

    if( dlg.ShowModal () != wxID_OK )
      return false;

    m->diffData = dlg.GetData ();
  }

  return true;
}


bool
DiffAction::Perform ()
{
  const std::vector<svn::Path> & v = GetTargets ();
  std::vector<svn::Path>::const_iterator it;

  for (it=v.begin (); it != v.end (); it++)
  {
    m->diffTarget (*it);
  }
 
  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
