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

// wxwindows
#include "wx/string.h"

// forward declarations
class Tracer;
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
 * Use the class @a ActionWorker to run actions.
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
   * @param name of the action
   * @param options 
   */
  Action (wxWindow * parent, const wxString & name, ActionOptions options);

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
   * If own is TRUE, then the @a Action class
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
   * Make sure to call @a Action::Prepare in every class
   * that inherits from @a Action. Make sure you check the
   * returned value as well.
   *
   * Return false to cancel action
   *
   * @see ActionWorker
   *
   * @retval true continue
   * @retval false cancel
   */
  virtual bool 
  Prepare ();
   

  /**
   * perform action. if any error occurs, an exception
   * will be thrown.
   */
  virtual bool 
  Perform () = 0;

  /**
   * sets the path for the action
   *
   * @param path
   */
  void 
  SetPath (const svn::Path & path);

  /**
   * @return path
   */
  const svn::Path & 
  GetPath ();

  /**
   * sets the targets for the action.
   * Not every action will need targets.
   *
   * @param targets
   */
  void 
  SetTargets (const svn::Targets & targets);

  /**
   * @return the targets for this action
   */
  const svn::Targets & 
  GetTargets ();

  /**
   * @return a single target for this action
   */
  const svn::Path 
  GetTarget ();

  /**
   * retrieves the options for the action
   *
   * @return options
   */
  ActionOptions 
  GetOptions ();


  /** set the name of the action */
  void
  SetName (const char * name);

  /** returns the name of the action */
  const char * 
  GetName () const;


  /**
   * output message with the tracer
   */
  void Trace (const wxString & msg);


  /**
   * output error message with the tracer
   */
  void TraceError (const wxString & msg);
protected:

  /**
   * thread-save method to post an event. The event will
   * be sent to the actions parent
   *
   * @param event event to send
   */
  void
  PostEvent (wxEvent & event);

private:
  struct Data;
  // this structure contains implementation specific data
  Data * m;

  /**
   * private default constructor
   */
  Action ();

  /**
   * private copy constructor
   */
  Action (const Action &);

  /**
   * thread-safe method to post a string event
   * the event is posted to parent
   */
  void PostStringEvent (int code, wxString str, int event_id);

  /**
   * thread-safe method to post a data event
   */
  void PostDataEvent (int code, void *data, int event_id);

};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
