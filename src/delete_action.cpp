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
#include "delete_action.h"
#include "wx/resource.h"
#include "delete_dlg.h"
#include "svn_notify.h"
#include "rapidsvn_app.h"

DeleteAction::DeleteAction (wxFrame * frame, Tracer * tr, apr_array_header_t * targets) 
  : ActionThread (frame), m_targets (targets)
{
  SetTracer (tr, FALSE);        // do not own the tracer
  m_pFrame = frame;
}

void
DeleteAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.

  DeleteDlg *delDlg = new DeleteDlg(m_pFrame, &Data);

  if (delDlg->ShowModal () == wxID_OK)
  {
    // #### TODO: check errors and throw an exception
    // create the thread
    Create ();

    // here we start the action thread
    Run ();
      ////////////////////////////////////////////////////////////
  }

  // destroy the dialog
  delDlg->Close (TRUE);
}

void *
DeleteAction::Entry ()
{
  svn::Modify modify;
  SvnNotify notify (GetTracer ());
  modify.notification (&notify);

  for (int i = 0; i < m_targets->nelts; i++)
  {
    const char *target = ((const char **) (m_targets->elts))[i];

    try
    {
      modify.remove (target, Data.Force);
    }
    catch (svn::ClientException &e)
    {
      PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                       ACTION_EVENT);
      GetTracer ()->Trace ("Delete failed:");
      GetTracer ()->Trace (e.description ());
    }
  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);

  return NULL;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
