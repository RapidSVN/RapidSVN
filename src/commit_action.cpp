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
#include "include.hpp"
#include "rapidsvn_app.hpp"
#include "commit_action.hpp"
#include "svn_notify.hpp"

CommitAction::CommitAction (wxFrame * frame, 
                            Tracer * tr, const svn::Targets & targets)
  : FileAction (frame), m_targets (targets)
{
  m_thisframe = frame;
  SetTracer (tr, FALSE);        // do not own the tracer
}

bool
CommitAction::PerformUI ()
{
  CommitDlg *ciDlg = new CommitDlg(m_thisframe, &m_data);

  int retval = ciDlg->ShowModal ();
  // destroy the dialog
  ciDlg->Close (TRUE);

  return (retval == wxID_OK);
}

void
CommitAction::Perform ()
{
  svn::Modify modify;
  SvnNotify notify (GetTracer ());
  modify.notification (&notify);
  long revision;
  svn::Pool pool(NULL);

  modify.username (m_data.User);
  modify.password (m_data.Password);

  svn::Targets targets (m_targets);

  try
  {
    revision = 
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
  }
  catch (svn::ClientException &e)
  {
    PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                     ACTION_EVENT);
    GetTracer ()->Trace ("The commit action failed:");
    GetTracer ()->Trace (e.description ());
  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
