/*
 * ====================================================================
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _ACTION_THREAD_H_INCLUDED_
#define _ACTION_THREAD_H_INCLUDED_

#include "tracer.h"

/**
* An action thread class is used as a base class
* for all thread classes that perform actions as checkout, 
* commit etc on a separate thread. Each of there threads 
* communicates with the main frame by sending events with 
* wxPostEvent.
*/
class ActionThread:public wxThread
{
protected:
    /**
    * The main frame of the application
    */
  wxFrame * mainFrame;

    /**
    * This member variable will take the address 
    * of a trace object allocated in a class 
    * derived from ActionThread. It will be used
    * from the svn_delta_editor callbacks.
    */
  Tracer *tracer;

    /**
    * If ownTracer is TRUE, then the ActionThread class
    * is responsible for deleting the tracer.
    */
  bool ownTracer;

  apr_pool_t *pool;

   ActionThread (wxFrame * frame, apr_pool_t * __po0l);
  ~ActionThread ();

  Tracer *GetTracer ()
  {
    return tracer;
  }

    /**
    * Sets the tracer passed as an argument.
    * If own is TRUE, then the ActionThread class
    * is responsible for deleting the tracer.
    */
  void SetTracer (Tracer * t, bool own = TRUE)
  {
    tracer = t;
  }

  void PostStringEvent (int code, wxString str, int event_id);
  void PostDataEvent (int code, void *data, int event_id);

    /**
    * Abstract method to be defined 
    * in every action class, accordingly 
    * with the class type.
    */
  virtual void Perform () = 0;

};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
