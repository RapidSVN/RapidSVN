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
#include "rapidsvn_app.hpp"
#include "update_action.hpp"
#include "svn_notify.hpp"

UpdateAction::UpdateAction (wxFrame * frame, 
                            Tracer * tr, apr_array_header_t * targets)
  : FileAction (frame), m_targets (targets)
{
  SetTracer (tr, FALSE);        // do not own the tracer
  m_pFrame = frame;
}

bool
UpdateAction::PerformUI ()
{
  UpdateDlg *upDlg = new UpdateDlg(m_pFrame, &m_data);

  int retval = upDlg->ShowModal ();
  // destroy the dialog
  upDlg->Close (TRUE);

  return (retval == wxID_OK);
}

void
UpdateAction::Perform ()
{
  svn::Modify modify;
  SvnNotify notify (GetTracer ());
  modify.notification (&notify);

  modify.username (m_data.User);
  modify.password (m_data.Password);

  long revnum = -1;
  // Did the user request a specific revision?:
  if (!m_data.UseLatest)
  {
    TrimString(m_data.Revision);
    if (!m_data.Revision.IsEmpty ())
      m_data.Revision.ToLong(&revnum, 10);  // If this fails, revnum is unchanged.
  }
  
  for (int i = 0; i < m_targets->nelts; i++)
  {
    const char *target = ((const char **) (m_targets->elts))[i];

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
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
