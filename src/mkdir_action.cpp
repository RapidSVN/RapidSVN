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

// app
#include "include.hpp"
//#include "wx/resource.h"
#include "utils.hpp"
#include "mkdir_dlg.hpp"
#include "rapidsvn_app.hpp"
#include "mkdir_action.hpp"
#include "svn_notify.hpp"

MkdirAction::MkdirAction (wxFrame * frame, Tracer * tr)
 : ActionThread (frame)
{
  m_thisframe = frame;
  SetTracer (tr, FALSE);        // do not own the tracer
}

void
MkdirAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.

  MkdirDlg *mkDlg = new MkdirDlg(m_thisframe, &m_data);

  if (mkDlg->ShowModal () == wxID_OK)
  {
    UnixPath (m_data.Target);
    TrimString (m_data.Target);
    
    // #### TODO: check errors and throw an exception
    // create the thread
    Create ();

    // here we start the action thread
    Run ();
     ////////////////////////////////////////////////////////////
  }

  // destroy the dialog
  mkDlg->Close (TRUE);
}

void *
MkdirAction::Entry ()
{
  svn::Modify modify;
  SvnNotify notify (GetTracer ());
  modify.notification (&notify);
  modify.username (m_data.User);
  modify.password (m_data.Password);

  try
  {
    modify.mkdir (m_data.Target.c_str (), m_data.LogMessage.c_str ());
  }
  catch (svn::ClientException &e)
  {
    PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                     ACTION_EVENT);
    GetTracer ()->Trace ("Mkdir failed:");
    GetTracer ()->Trace (e.description ());
  }
  return NULL;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
