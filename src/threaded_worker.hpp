/*
 * ====================================================================
 * Copyright (c) 2002-2011 The RapidSVN Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _THREADED_WORKER_H_
#define _THREADED_WORKER_H_

// app
#include "action_worker.hpp"

// forward declarations
class wxWindow;
class Action;

class ThreadedWorker : public ActionWorker
{
public:
  /**
   * default constructor.
   */
  ThreadedWorker(wxWindow * parent = 0);

  /**
   * destructor
   */
  virtual ~ThreadedWorker();

  /**
   * @see ActionWorker
   */
  virtual void
  Create(wxWindow * parent);

  /**
   * @see ActionWorker
   */
  virtual ActionState
  GetState();

  /**
   * @see ActionWorker
   */
  virtual ActionResult
  GetResult();

  /**
   * @see ActionWorker
   */
  virtual void SetTracer(Tracer * tr);

  /**
   * @see ActionWorker
   */
  virtual bool
  Perform(Action * action);

  /**
   * @see ActionWorker
   */
  virtual void
  SetContext(svn::Context * context, bool own = false);

  /**
   * @see ActionWorker
   */
  virtual svn::Context *
  GetContext() const;

private:
  class Data;
  Data * m;

  /**
   * disallow private copy constructor
   */
  ThreadedWorker(const ThreadedWorker &);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
