/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
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
  SimpleWorker (wxWindow * parent = 0);

  /**
   * destructor
   */
  virtual ~SimpleWorker ();

  /**
   * @see ActionWorker
   */
  virtual void 
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
  virtual void SetTracer (Tracer * tr);

  /**
   * @see ActionWorker
   */
  virtual bool 
  Perform (Action * action);

  /**
   * @see ActionWorker
   */
  virtual void
  SetContext (svn::Context * context, bool own = false);

  /**
   * @see ActionWorker
   */
  virtual svn::Context * 
  GetContext () const;
private:
  struct Data;
  Data * m;

  /**
   * private copy constructor
   */
  SimpleWorker (const SimpleWorker &);

  /**
   * sends a message to the tracer. This is
   * a utility function that checks m_tracer
   * before tracing
   *
   * @param message to send
   */
  void Trace (const wxString & message);

  /**
   * Posts a string event. This is used the
   * completion of an action or an error
   * 
   * @param code event code
   * @param str string
   * @param event_id
   */
  void
  PostStringEvent (int code, wxString str, int event_id);

  /**
   * Posts a data event. This call is thread-safe
   *
   * @param code
   * @param data
   * @param event_id
   */
  void
  PostDataEvent (int code, void * data, int event_id);

};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
