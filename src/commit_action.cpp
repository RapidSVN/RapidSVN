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

#include "svncpp/modify.h"
#include "include.h"
#include "wx/resource.h"
#include "rapidsvn_app.h"
#include "commit_action.h"
#include "svn_notify.h"

CommitAction::CommitAction (wxFrame * frame, apr_pool_t * __pool, 
                            Tracer * tr, apr_array_header_t * trgts)
                            : FileAction (frame, __pool), targets (trgts)
{
  thisframe = frame;
  SetTracer (tr, FALSE);        // do not own the tracer
}

bool
CommitAction::PerformUI ()
{
  CommitDlg *ciDlg = new CommitDlg(thisframe, &Data);

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

  modify.username (Data.User);
  modify.password (Data.Password);

  for (int i = 0; i < targets->nelts; i++)
  {
    const char *target = ((const char **) (targets->elts))[i];

    try
    {
      revision = modify.commit (target, Data.LogMessage.c_str (), Data.Recursive);
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
  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
