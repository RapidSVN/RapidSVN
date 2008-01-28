/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _ACTION_WORKER_H_
#define _ACTION_WORKER_H_

// forward declarations
class Action;
class wxWindow;
class Tracer;

namespace svn
{
  class Context;
}

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
   * @param the tracer that will receive notifications
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
   * sets the tracer that will be notified
   * this values will be passed to each
   * action that is to be performed.
   * The tracer will NOT be owned by the
   * worker then.
   *
   * @param tracer
   */
  virtual void 
  SetTracer (Tracer * tr) = 0;

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

  /**
   * Sets the context to use for actions
   *
   * @param context
   * @param own true: this class will own and delete the worker
   */
  virtual void
  SetContext (svn::Context * context, bool own = false) = 0;

  /**
   * @return the context
   */
  virtual svn::Context * 
  GetContext () const = 0;

  virtual ~ActionWorker () { }
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
