/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxwindows
#include "wx/wx.h"
#include "wx/filename.h"

// svncpp
#include "svncpp/client.hpp"

// app
#include "merge_action.hpp"
#include "utils.hpp"

MergeAction::MergeAction (wxWindow * parent)
  : Action (parent, _("Merge"), GetBaseFlags ()),
    m_calledByLogDlg (false)
{
}

MergeAction::MergeAction (wxWindow * parent, MergeData & data)
  : Action (parent, _("Merge"), GetBaseFlags ()), m_data (data),
    m_calledByLogDlg (true)
{
}

bool
MergeAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  // Set default Destination to the selected path in the folder browser
  // or, if called by the log dialogue, to the source path
  if (m_calledByLogDlg)
    m_data.Destination = m_data.Path1;
  else
    m_data.Destination = Utf8ToLocal (GetPath().c_str());

  MergeDlg dlg (GetParent (), m_calledByLogDlg, m_data);

  if (dlg.ShowModal () != wxID_OK)
  { 
    return false;
  }

  return true;
}

bool
MergeAction::Perform ()
{
  svn::Client client (GetContext ());

  // Set current working directory to the path where the
  // merge will be performed. If Destination is a file and not a
  // directory, only the directory part should be used
  wxFileName path (m_data.Destination);
  if (!wxSetWorkingDirectory (path.GetPath (wxPATH_GET_VOLUME)))
  {
    wxString msg;
    msg.Printf(_("Could not set working directory to %s"),
               path.GetPath (wxPATH_GET_VOLUME).c_str ());
    TraceError (msg);
    //PostStringEvent (TOKEN_VSVN_INTERNAL_ERROR, msg, ACTION_EVENT);
    return false;
  }

  svn_revnum_t rev1 = 0;
  svn_revnum_t rev2 = 0;

  if (!m_data.Path1Rev.ToLong (&rev1) || !m_data.Path2Rev.ToLong (&rev2))
  {
    wxString msg = _("Invalid revision number detected");
    TraceError (msg);
    return false;
  }

  std::string Path1Utf8, Path2Utf8, DestinationUtf8;
  LocalToUtf8(m_data.Path1, Path1Utf8);
  LocalToUtf8(m_data.Path2, Path2Utf8);
  LocalToUtf8(m_data.Destination, DestinationUtf8);
  client.merge (svn::Path (Path1Utf8), 
                rev1, 
                svn::Path (Path2Utf8), 
                rev2, 
                svn::Path (DestinationUtf8), 
                m_data.Force, 
                m_data.Recursive);
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
