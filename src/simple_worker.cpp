/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxwindows
#include "wx/wx.h"

// svncpp
#include "svncpp/exception.hpp"

// app
#include "action.hpp"
#include "ids.hpp"
#include "simple_worker.hpp"
#include "tracer.hpp"

SimpleWorker::SimpleWorker (wxWindow * parent)
  : m_action (0), m_state (ACTION_NONE), 
    m_result (ACTION_NOTHING), m_parent (parent), m_tracer (0)
{
}

SimpleWorker::~SimpleWorker ()
{
}

void
SimpleWorker::Create (wxWindow * parent)
{
  m_action = 0;
  m_state  = ACTION_NONE;
  m_result = ACTION_NOTHING;
  m_parent = parent;
  m_tracer = 0;
}

ActionState 
SimpleWorker::GetState ()
{
  return m_state;
}

ActionResult
SimpleWorker::GetResult ()
{
  return m_result;
}

bool
SimpleWorker::Perform (Action * action)
{
  m_result = ACTION_NOTHING;
  m_action = action;
  m_state = ACTION_INIT;

  try
  {
    if (!m_action->Prepare ())
    {
      m_result = ACTION_ABORTED;
      delete m_action;
      m_action = 0;
      m_state = ACTION_NONE;
      return true;
    }
  }
  catch (svn::ClientException & e)
  {
    wxString msg;
    msg.Printf ( _("Error while preparing action: %s"), e.message () );
    Trace (msg);
    return false;
  }
  catch (...)
  {
    Trace (_("Error while preparing action."));
    return false;
  }
  
  try
  {
    // this cursor stuff has to change...
    wxBusyCursor wait;
    m_state = ACTION_RUNNING;
    bool result = m_action->Perform ();
    if (!result)
    {
      m_result = ACTION_ERROR;
    }
    else
    {
      m_result = ACTION_SUCCESS;
    }
    delete m_action;
    m_action = 0;
    m_state = ACTION_NONE;
    return true;
  }
  catch (svn::ClientException & e)
  {
    wxString msg;
    msg.Printf (_("Error while performing action: %s"), 
                e.message ());
    PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, msg, ACTION_EVENT);
    //TODO thread safe? 
    //Trace (msg);
    return false;
  }
  catch (...)
  {
    wxString msg (_("Error while performing action."));
    PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, msg, ACTION_EVENT);
    //TODO thread safe?
    //Trace ();
    return false;
  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);
}

void
SimpleWorker::SetTracer (Tracer * tracer)
{
  m_tracer = tracer;
}

void
SimpleWorker::Trace (const wxString & message)
{
  if (m_tracer)
  {
    m_tracer->Trace (message);
  }
}

void
SimpleWorker::PostStringEvent (int code, wxString str, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetString (str);

  // send in a thread-safe way
  wxPostEvent (m_parent, event);
}

void
SimpleWorker::PostDataEvent (int code, void *data, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetClientData (data);

  // send in a thread-safe way
  wxPostEvent (m_parent, event);
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
