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


//#include "svncpp/log.hpp"
#include "include.hpp"
#include "wx/resource.h"
#include "rapidsvn_app.hpp"
#include "log_action.hpp"
#include "svn_notify.hpp"

LogAction::LogAction (wxFrame * frame, Tracer * tr, const char * target)
  : ActionThread (frame), m_thisframe(frame), m_target(target)
{
  SetTracer (tr, FALSE);        // do not own the tracer
}

void
LogAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.
  svn::Log log;

  log.loadPath (m_target, -2, 1);

  if(!log.isVersioned ())
	  return;

  m_logDialog = new LogDlg(m_thisframe, &log);

  if (m_logDialog->ShowModal () == wxID_OK)
  {
    // #### TODO: check errors and throw an exception
    // create the thread
    Create ();

    // here we start the action thread
    Run ();

    ////////////////////////////////////////////////////////////
  }

  // destroy the dialog
  m_logDialog->Close (TRUE);
}

void *
LogAction::Entry ()
{
  return NULL;
}

void 
LogAction::setLogMessage (const char * message)
{
  m_logDialog->setLogMessage (message);
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
