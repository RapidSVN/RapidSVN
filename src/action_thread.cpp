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
#include "include.hpp"
#include "action_thread.hpp"

ActionThread::ActionThread (wxFrame * frame):wxThread ()
  // default creates the thread DETACHED
  , m_mainFrame (frame)
{
  m_pool.Create(NULL);
  m_tracer = NULL;
  m_ownTracer = FALSE;
}

ActionThread::~ActionThread ()
{
  // The derived classes allocate a tracer, 
  // set ActionThread::tracer to point to it
  // and this class deletes it.
  if (m_tracer && m_ownTracer)
  {
    delete m_tracer;
  }
}

void
ActionThread::PostStringEvent (int code, wxString str, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetString (str);

  // send in a thread-safe way
  wxPostEvent (m_mainFrame, event);

}

void
ActionThread::PostDataEvent (int code, void *data, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetClientData (data);

  // send in a thread-safe way
  wxPostEvent (m_mainFrame, event);

}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
