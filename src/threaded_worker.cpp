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

// app
#include "action.hpp"
#include "threaded_worker.hpp"

ThreadedWorker::ThreadedWorker ()
  : wxThread (wxTHREAD_DETACHED),
    m_action (NULL), m_state (ACTION_NONE), 
    m_result (ACTION_NOTHING), m_parent (NULL)
{
}

ThreadedWorker::~ThreadedWorker ()
{
}

void
ThreadedWorker::Create (wxWindow * parent)
{
  m_action = NULL;
  m_state = ACTION_NONE;
  m_result = ACTION_NOTHING;
  m_parent = parent;

  wxThread::Create ();
  Run ();
}

ActionState 
ThreadedWorker::GetState ()
{
  wxMutexLocker locker (m_mutex);

  return m_state;
}

ActionResult
ThreadedWorker::GetResult ()
{
  wxMutexLocker locker (m_mutex);

  return m_result;
}

bool
ThreadedWorker::Perform (Action * action)
{
  {
    wxMutexLocker locker (m_mutex);

    if (m_state != ACTION_NONE)
    {
      return false;
    }

    m_result = ACTION_NOTHING;
    m_action = action;
    m_state = ACTION_INIT;
  }

  if( !m_action->Prepare () )
  {
    {
      wxMutexLocker locker (m_mutex);
      m_result = ACTION_ABORTED;
      delete m_action;
      m_action = NULL;
      m_state = ACTION_NONE;
      return true;
    }
  }

  {
    wxMutexLocker locker (m_mutex);
    m_state = ACTION_RUNNING;
  }

  return true;
}

void *
ThreadedWorker::Entry ()
{
  while (!TestDestroy ())
  {
    bool run = false;
    {
      wxMutexLocker locker (m_mutex);
      if (m_state == ACTION_RUNNING )
      {
        run = true;
      }
    }

    if (run)
    {
      // perform action
      bool result = m_action->Perform ();

      // after this check result and free action
      {
        wxMutexLocker locker (m_mutex);

        if (result) 
        {
          m_result = ACTION_SUCCESS;
        }
        else
        {
          m_result = ACTION_ERROR;
        }

        delete m_action;
        m_action = NULL;
        m_state = ACTION_NONE;
      }
    }
    
    // give the rest of the time slice to other threads
    Yield ();
  }

  return NULL;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
