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
#ifndef _THREADED_WORKER_H_
#define _THREADED_WORKER_H_

// wxwindows
#include "wx/thread.h"

// app
//#include "action.hpp"
#include "action_worker.hpp"

// forward declarations
class Action;
class wxWindow;

/**
 * this is the threaded implementation of the
 * class ActionWorker. For a documentation of
 * the common methods take a look at ActionWorker
 *
 * @see ActionWorker
 */
class ThreadedWorker : public wxThread, ActionWorker
{
public:
  /**
   * default constructor. 
   */
  ThreadedWorker ();

  /**
   * destructor
   */
  virtual ~ThreadedWorker ();

  /**
   * @see ActionWorker
   */
  virtual void Create (wxWindow * parent);

  /**
   * @see ActionWorker
   */
  virtual ActionState GetState ();

  /**
   * @see ActionWorker
   */
  virtual ActionResult GetResult ();

  /**
   * @see ActionWorker
   */
  virtual const bool Perform (Action * action);

protected:
  virtual void * Entry ();
  
private:
  Action * m_action;
  ActionState m_state;
  ActionResult m_result;
  wxWindow * m_parent;
  wxMutex m_mutex;

  /**
   * private copy constructor
   */
  ThreadedWorker (const ThreadedWorker &);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
