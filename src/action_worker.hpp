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
#ifndef _ACTION_WORKER_H_
#define _ACTION_WORKER_H_

// declarations
class Action;
class wxWindow;

enum ActionState
{
  ACTION_NONE,
  ACTION_INIT,
  ACTION_RUNNING
};

enum ActionResult
{
  ACTION_NOTHING,
  ACTION_ABORTED,
  ACTION_ERROR,
  ACTION_SUCCESS
};

/**
 * this is the full abstract class of the action
 * worker. it is used to get a unique calling interface.
 * use either the implementation SimpleWorker or
 * ThreadedWorker
 *
 * @see SimpleWorker
 * @see ThreadedWorker
 */
class ActionWorker
{
public:
  /**
   * associate worker with parent window and
   *
   * @param parent parent window
   */
  virtual void 
  Create (wxWindow * parent) = 0;

  /**
   * check if there is already an action
   * running
   *
   * @return true=active / false=inactive
   */
  virtual ActionState 
  GetState () = 0;

  /**
   * returns the result of the action
   *
   * @see ActionResult
   *
   * @return result of the last action
   */
  virtual ActionResult 
  GetResult () = 0;

  /**
   * Used to run an Action. If there
   * is already an action running return
   * false. In this case you have to
   * release action. If the action is
   * accepted it will be owned by the ActionWorker
   * and released after its finished.
   *
   * @param action Action to run
   * @return true=success/false=another action running
   */
  virtual bool 
  Perform (Action * action) = 0;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
