/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// svncpp
#include "svncpp/exception.hpp"
#include "svncpp/modify.hpp"
#include "svncpp/pool.hpp"
#include "svncpp/targets.hpp"

// app
//REMOVE #include "include.hpp"
//REMOVE #include "rapidsvn_app.hpp"
#include "commit_action.hpp"
#include "commit_dlg.hpp"
#include "ids.hpp"
#include "svn_notify.hpp"
#include "tracer.hpp"
#include "utils.hpp"

CommitAction::CommitAction (wxWindow * parent, const svn::Targets & targets,
                            wxString & path, Tracer * tr, bool own)
  : Action (parent, tr, own), m_targets (targets), m_path (path)
{
}

bool
CommitAction::Prepare ()
{
  CommitDlg dlg(GetParent ());

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  m_data = dlg.GetData ();
  return true;
}

bool
CommitAction::Perform ()
{
  SvnNotify notify (GetTracer ());

  svn::Modify modify;
  modify.notification (&notify);
  modify.username (m_data.User);
  modify.password (m_data.Password);

  svn::Targets targets (m_targets);

  bool result = false;
  try
  {
    svn::Pool pool;
    wxSetWorkingDirectory (m_path);
    long revision = 
      modify.commit (targets.array (pool.pool ()), m_data.LogMessage.c_str (), 
                     m_data.Recursive);
    wxString str;

    if(!modify.isAuthenticated ())
    {
      str = "Authentication failed.";
    }
    else
    {
      str = wxString::Format ("Committed revision %" SVN_REVNUM_T_FMT ".",
                              revision);
    }
    GetTracer ()->Trace (str);
    result = true;
  }
  catch (svn::ClientException &e)
  {
    PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                     ACTION_EVENT);
    GetTracer ()->Trace ("The commit action failed:");
    GetTracer ()->Trace (e.description ());
  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);
  return result;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */