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
