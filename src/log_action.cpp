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


//#include "svncpp/log.h"
#include "include.h"
#include "wx/resource.h"
#include "rapidsvn_app.h"
#include "log_action.h"
#include "svn_notify.h"

LogAction::LogAction (wxFrame * frame, apr_pool_t * __pool, 
                      Tracer * tr, const char * target)
                      : ActionThread (frame, __pool), _target(target)
{
  thisframe = frame;
  SetTracer (tr, FALSE);        // do not own the tracer
}

void
LogAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.
  svn::Log log;

  log.loadPath (_target, -2, 1);

  if(!log.isVersioned ())
	  return;

  logDialog = new LogDlg(thisframe, &log);

  if (logDialog->ShowModal () == wxID_OK)
  {
    // #### TODO: check errors and throw an exception
    // create the thread
    Create ();

    // here we start the action thread
    Run ();

    ////////////////////////////////////////////////////////////
  }

  // destroy the dialog
  logDialog->Close (TRUE);
}

void *
LogAction::Entry ()
{
  return NULL;
}

void 
LogAction::setLogMessage (const char * message)
{
  logDialog->setLogMessage (message);
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
