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
#ifndef _ACTION_H_
#define _ACTION_H_

// forward declarations
class Tracer;
class wxString;
class wxWindow;

/**
 * Inherit from this class
 * and use the constructor to pass parameters to the class.
 *
 * Use the class ActionWorker to run actions.
 *
 * @see ActionWorker
 */
class Action
{
public:
  /**
   * constructor
   *
   * @param parent parent window
   * @param tracer tracer
   * @param own tracer owned by action?
   */
  Action (wxWindow * parent, Tracer * tracer=0, bool own=true);

  /**
   * destructor
   */
  virtual ~Action ();

  /**
   * @return Tracer instance
   */
  Tracer * GetTracer ();

  /**
   * Sets the tracer passed as an argument.
   * If own is TRUE, then the ActionThread class
   * is responsible for deleting the tracer.
   */
  void SetTracer (Tracer * t, bool own = true);

  /**
   * thread-safe method to post a string event
   * the event is posted to parent
   */
  void PostStringEvent (int code, wxString str, int event_id);

  /**
   * thread-safe method to post a data event
   */
  void PostDataEvent (int code, void *data, int event_id);

  /**
   * Prepare action. This method is execute in the main
   * thread. You can use this method for user interaction.
   *
   * Return false to cancel action
   *
   * @see ActionWorker
   *
   * @return true=continue/false=cancel
   */
  virtual bool Prepare () = 0;
   
  /**
   * perform action. if any error occurs, an exception
   * will be thrown.
   */
  virtual bool Perform () = 0;

  /**
   * @return parent
   */
  wxWindow * GetParent ();

protected:
  void Trace (const wxString & msg);

private:
  /**
   * The parent
   */
  wxWindow * m_parent;
    
  /**
   * This member variable will take the address 
   * of a trace object allocated in a class 
   * derived from ActionThread. It will be used
   * from the svn_delta_editor callbacks.
   */
  Tracer * m_tracer;
    
  /**
   * If ownTracer is TRUE, then the ActionThread class
   * is responsible for deleting the tracer.
   */
  bool m_ownTracer;

  /**
   * private default constructor
   */
  Action ();

  /**
   * private copy constructor
   */
  Action (const Action &);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
