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
#include "include.h"
#include "action_thread.h"

ActionThread::ActionThread (wxFrame * frame, apr_pool_t * __pool):wxThread ()
  // default creates the thread DETACHED
  , mainFrame (frame), pool (__pool)
{
  tracer = NULL;
  ownTracer = FALSE;
  mainFrame->SetCursor(wxCURSOR_ARROWWAIT);
}

ActionThread::~ActionThread ()
{
  // The derived classes allocate a tracer, 
  // set ActionThread::tracer to point to it
  // and this class deletes it.
  if (tracer && ownTracer)
    delete tracer;
  mainFrame->SetCursor(wxCURSOR_ARROW);
}

void
ActionThread::PostStringEvent (int code, wxString str, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetString (str);

  // send in a thread-safe way
  wxPostEvent (mainFrame, event);

}

void
ActionThread::PostDataEvent (int code, void *data, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetClientData (data);

  // send in a thread-safe way
  wxPostEvent (mainFrame, event);

}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
