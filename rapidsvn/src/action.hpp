/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
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
#include "svncpp/revision.hpp"

// wxwindows
#include "wx/string.h"

// forward declarations
class Tracer;
class wxWindow;
class wxEvent;

namespace svn
{
  class Context;
}

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
   * if set then no files have changed after this
   * operation and we dont have to update the filelist
   *
   * @see GetFlags
   */
  static const unsigned int DONT_UPDATE;

  /**
   * if set then the files wont be changed immediately.
   * To detect this one can set a flag and update later,
   * when the application gets activated again
   * (action opened different application, user applied
   * changes, saved them and comes back to rapidsvn
   * later -> BOOM, update)
   *
   * @remarks Use either @a DONT_UPDATE or @a UPDATE_LATER
   */
  static const unsigned int UPDATE_LATER;

  /**
   * the action does not depend on the currently selected
   * target - so can proceed regardless
   *
   * @remarks Shouldn't be used with any of the @a TARGET_QUANTITY_MASK flags
   */
  static const unsigned int WITHOUT_TARGET;
  
  /**
   * the action can act on a single target
   *
   * @see TARGET_QUANTITY_MASK
   */
  static const unsigned int SINGLE_TARGET;
    
  /**
   * the action can act on multiple targets simultaneously
   *
   * @see TARGET_QUANTITY_MASK
   */
  static const unsigned int MULTIPLE_TARGETS;
  
  /**
   * covers both target quantity flags for some
   * bitwise actsions, but not @a WITHOUT_TARGET
   *
   * @see SINGLE_TARGET
   * @see MULTIPLE_TARGETS
   */
  static const unsigned int TARGET_QUANTITY_MASK;
  
  /**
   * the action can work with Url type paths
   * from the repository
   *
   * @see TARGET_TYPE_MASK
   */
  static const unsigned int RESPOSITORY_TYPE;
  
  /**
   * the action can work with versioned
   * files from the working copies
   *
   * @see TARGET_TYPE_MASK
   */
  static const unsigned int VERSIONED_WC_TYPE;
  
  /**
   * the action can work with un-versioned
   * files from the working copies
   *
   * @see TARGET_TYPE_MASK
   */
  static const unsigned int UNVERSIONED_WC_TYPE;
  
  /**
   * covers the three target types for some
   * bitwise actsions, but not @a IS_DIR
   *
   * @see RESPOSITORY_TYPE
   * @see VERSIONED_WC_TYPE
   * @see UNVERSIONED_WC_TYPE
   */
  static const unsigned int TARGET_TYPE_MASK;
  
  /**
   * used by the framework to indicate a target is a directory,
   * either repository or working copy
   */
  static const unsigned int IS_DIR;

  /**
   * constructor
   *
   * @param parent parent window
   * @param name of the action
   * @param options 
   */
  Action (wxWindow * parent, const wxString & name, unsigned int flgs);

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
   * retrieves the flags for this action.
   *
   * @see DONT_UPDATE
   */
  unsigned int 
  GetFlags () const;


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

  
  /**
    * retrieves a file @a path with @a revision
    * settings from the repository and write it to
    * a temporary file
    *
    * @return temporary filename
    */
  svn::Path
  Action::GetPathAsTempFile (
    const svn::Path & path, 
    const svn::Revision & revision = svn::Revision::HEAD);

protected:

  /**
   * thread-save method to post an event. The event will
   * be sent to the actions parent
   *
   * @param event event to send
   */
  void
  PostEvent (wxEvent & event);


  /**
   * sets the flags for this action - but passing into 
   * the constructor is preferred
   */
  void
  SetFlags (unsigned int flags);

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
