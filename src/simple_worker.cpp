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
#include "wx/utils.h"

// app
#include "action.hpp"
#include "simple_worker.hpp"

SimpleWorker::SimpleWorker ()
  : m_action (0), m_state (ACTION_NONE), 
    m_result (ACTION_NOTHING), m_parent (0)
{
}

SimpleWorker::~SimpleWorker ()
{
}

void
SimpleWorker::Create (wxWindow * parent)
{
  m_action = 0;
  m_state = ACTION_NONE;
  m_result = ACTION_NOTHING;
  m_parent = parent;
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

  if (!m_action->Prepare ())
  {
    m_result = ACTION_ABORTED;
    delete m_action;
    m_action = 0;
    m_state = ACTION_NONE;
    return true;
  }
  
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
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
