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
#include "svncpp/client.hpp"

// app
#include "checkout_action.hpp"
#include "checkout_dlg.hpp"
#include "ids.hpp"
#include "utils.hpp"
#include "svn_notify.hpp"
#include "tracer.hpp"

CheckoutAction::CheckoutAction (wxWindow * parent, 
                                Tracer * tracer)
  : Action (parent, tracer, false)
{
}

bool
CheckoutAction::Prepare ()
{
  CheckoutDlg dlg (GetParent ());
  if( dlg.ShowModal () != wxID_OK )
  {
    return false;
  }
  m_data = dlg.GetData ();
  return true;
}

bool
CheckoutAction::Perform ()
{
  svn::Context context (m_data.User, m_data.Password);
  svn::Client client (&context);
  SvnNotify notify (GetTracer ());
  client.notification (&notify);

  TrimString(m_data.DestFolder);
  UnixPath(m_data.DestFolder);
  TrimString(m_data.ModuleName);
  
  long revnum=-1;
  // Did the user request a specific revision?:
  if (!m_data.UseLatest)
  {
    TrimString(m_data.Revision);
    if (!m_data.Revision.IsEmpty ())
    {
      m_data.Revision.ToLong(&revnum, 10);  // If this fails, revnum is unchanged.
    }
  }

  bool result = true;
  try
  {
    svn::Revision revision (revnum);
    wxSetWorkingDirectory (m_data.DestFolder);
    client.checkout (m_data.ModuleName.c_str (), 
                     m_data.DestFolder.c_str (), 
                     revision, m_data.Recursive);
  }
  catch (svn::ClientException &e)
  {
      PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                       ACTION_EVENT);
      GetTracer ()->Trace ("Checkout failed:");
      GetTracer ()->Trace (e.description ());
      result = false;
  }
 
  return result;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
