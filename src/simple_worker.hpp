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
#ifndef _SIMPLE_WORKER_H_
#define _SIMPLE_WORKER_H_

// app
#include "action_worker.hpp"

// forward declarations
class wxWindow;
class Action;

class SimpleWorker : public ActionWorker
{
public:
  /**
   * default constructor.
   */
  SimpleWorker ();

  /**
   * destructor
   */
  virtual ~SimpleWorker ();

  /**
   * @see ActionWorker
   */
  void 
  Create (wxWindow * parent);

  /**
   * @see ActionWorker
   */
  virtual ActionState 
  GetState ();

  /**
   * @see ActionWorker
   */
  virtual ActionResult 
  GetResult ();

  /**
   * @see ActionWorker
   */
  virtual bool 
  Perform (Action * action);

private:
  Action * m_action;
  ActionState m_state;
  ActionResult m_result;
  wxWindow * m_parent;

  /**
   * private copy constructor
   */
  SimpleWorker (const SimpleWorker &);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
