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

#include "svncpp/modify.hpp"
#include "include.hpp"
#include "wx/resource.h"
#include "utils.hpp"
#include "checkout_dlg.hpp"
#include "rapidsvn_app.hpp"
#include "checkout_action.hpp"
#include "svn_notify.hpp"

CheckoutAction::CheckoutAction (wxFrame * frame, Tracer * tr)
  : ActionThread (frame)
{
  SetTracer (tr, FALSE);        // do not own the tracer
  m_pFrame = frame;
}

void
CheckoutAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.
  CheckoutDlg *coDlg = new CheckoutDlg(m_pFrame, &m_data);

  if (coDlg->ShowModal () == wxID_OK)
  {
    // #### TODO: check errors and throw an exception
    // create the thread
    Create ();

    // here we start the action thread
    Run ();
  }
  
  // destroy the dialog
  coDlg->Close (TRUE);
}

void *
CheckoutAction::Entry ()
{
  svn::Modify modify;
  SvnNotify notify (GetTracer ());
  modify.notification (&notify);

  modify.username (m_data.User);
  modify.password (m_data.Password);

  TrimString(m_data.DestFolder);
  UnixPath(m_data.DestFolder);
  TrimString(m_data.ModuleName);
  
  long revnum = -1;
  // Did the user request a specific revision?:
  if (!m_data.UseLatest)
  {
    TrimString(m_data.Revision);
    if (!m_data.Revision.IsEmpty ())
      m_data.Revision.ToLong(&revnum, 10);  // If this fails, revnum is unchanged.
  }

  try
  {
    modify.checkout (m_data.ModuleName, m_data.DestFolder, 
                     revnum, m_data.Recursive);
  }
  catch (svn::ClientException &e)
  {
      PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                       ACTION_EVENT);
      GetTracer ()->Trace ("Checkout failed:");
      GetTracer ()->Trace (e.description ());
  }
 
  return NULL;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
