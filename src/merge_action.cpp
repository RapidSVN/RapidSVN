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

// svncpp
#include "svncpp/client.hpp"

// app
#include "merge_action.hpp"

MergeAction::MergeAction (wxWindow * parent)
  : Action (parent, _("Merge"), GetBaseFlags ())
{
}

bool
MergeAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  MergeDlg dlg (GetParent (), m_data);

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

  // Set current working directory to point to the path
  // in the folder browser (the path where the merge will be 
  // performed)
  wxString path (GetPath ().c_str ());
  if (!wxSetWorkingDirectory (path))
  {
    wxString msg;
    msg.Printf(_("Could not set working directory to %s"),
               path.c_str ());
    TraceError (msg);
    //PostStringEvent (TOKEN_VSVN_INTERNAL_ERROR, msg, ACTION_EVENT);
    return false;
  }


  //TODO check this
  long rev1 = 0;//MergeAction::getRevision (m_data.Path1Rev);
  long rev2 = 0;//MergeAction::getRevision (m_data.Path2Rev);
  client.merge (m_data.Path1.c_str (), 
                rev1, 
                m_data.Path2.c_str (), 
                rev2, 
                path.c_str (), 
                m_data.Force, 
                m_data.Recursive);
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
