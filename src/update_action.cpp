/*
 * ====================================================================
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
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
#include "update_action.h"
#include "svn_notify.h"

UpdateAction::UpdateAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr, apr_array_header_t * trgts):ActionThread (frame, __pool),
  targets
  (trgts)
{
  SetTracer (tr, FALSE);        // do not own the tracer
  m_pFrame = frame;
}

void
UpdateAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.

  UpdateDlg *upDlg = new UpdateDlg(m_pFrame, &Data);

  if (upDlg->ShowModal () == wxID_OK)
  {
    // #### TODO: check errors and throw an exception
    // create the thread
    Create ();

    // here we start the action thread
    Run ();

    ////////////////////////////////////////////////////////////
  }

  // destroy the dialog
  upDlg->Close (TRUE);
}

void *
UpdateAction::Entry ()
{
  svn::Modify modify;
  SvnNotify notify (GetTracer ());
  modify.notification (&notify);

  modify.username (Data.User);
  modify.password (Data.Password);

  long revnum = -1;
  // Did the user request a specific revision?:
  if (!Data.UseLatest)
  {
    TrimString(Data.Revision);
    if (!Data.Revision.IsEmpty ())
      Data.Revision.ToLong(&revnum, 10);  // If this fails, revnum is unchanged.
  }
  
  for (int i = 0; i < targets->nelts; i++)
  {
    const char *target = ((const char **) (targets->elts))[i];

    try
    {
      modify.update (target, revnum, true);
    }
    catch (svn::ClientException &e)
    {
      PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                       ACTION_EVENT);
      GetTracer ()->Trace ("Update failed:");
      GetTracer ()->Trace (e.description ());
    }
  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);

  return NULL;
}
