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

// svncpp
#include "svncpp/path.hpp"
#include "svncpp/targets.hpp"

// forward declarations
class Tracer;
class wxString;
class wxWindow;
namespace svn
{
  class Context;
}

enum ActionOptions
{
  actionWithoutTarget,
  actionWithSingleTarget,
  actionWithTargets
};

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
   * @param options 
   * @param own tracer owned by action?
   */
  Action (wxWindow * parent, ActionOptions options);

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
  void SetTracer (Tracer * t, bool own);

  /**
   * set actions parent window
   *
   * @param parent the parent that will receive events
   */
  void SetParent (wxWindow * parent);

  /**
   * @return parent
   */
  wxWindow * GetParent ();

  /**
   * sets the context for this action
   *
   * @param context
   */
  void SetContext (svn::Context * context);

  /**
   * @return the context of the action
   */
  svn::Context *
  GetContext ();

  /**
   * Prepare action. This method is execute in the main
   * thread. You can use this method for user interaction.
   * Make sure to call Action::Prepare in every class
   * that inherits from Action. Make sure you check the
   * returned value as well.
   *
   * Return false to cancel action
   *
   * @see ActionWorker
   *
   * @return true=continue/false=cancel
   */
  virtual bool Prepare ();
   
  /**
   * perform action. if any error occurs, an exception
   * will be thrown.
   */
  virtual bool Perform () = 0;

  /**
   * sets the path for the action
   *
   * @param path
   */
  void SetPath (const svn::Path & path);

  /**
   * @return path
   */
  const svn::Path & GetPath ();

  /**
   * sets the targets for the action.
   * Not every action will need targets.
   *
   * @param targets
   */
  void SetTargets (const svn::Targets & targets);

  /**
   * @return the targets for this action
   */
  const svn::Targets & GetTargets ();

  /**
   * @return a single target for this action
   */
  const svn::Path GetTarget ();

  /**
   * retrieves the options for the action
   *
   * @return options
   */
  ActionOptions GetOptions ();

protected:
  void Trace (const wxString & msg);

  /**
   * thread-safe method to post a string event
   * the event is posted to parent
   */
  void PostStringEvent (int code, wxString str, int event_id);

  /**
   * thread-safe method to post a data event
   */
  void PostDataEvent (int code, void *data, int event_id);

private:
  /**
   * The parent
   */
  wxWindow * m_parent;
    
  /**
   * Options for the action. 
   */
  ActionOptions m_options;

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
   * the context under which the action will be
   * performed. this includes auth info and callback
   * addresses
   */
  svn::Context * m_context;

  /**
   * the path where the action will take place
   */
  svn::Path m_path;

  svn::Targets m_targets;


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
