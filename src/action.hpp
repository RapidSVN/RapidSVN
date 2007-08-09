/*
 * ====================================================================
 * Copyright (c) 2002-2007 The RapidSvn Group.  All rights reserved.
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
#ifndef _ACTION_H_
#define _ACTION_H_

// svncpp
#include "svncpp/path.hpp"
#include "svncpp/revision.hpp"

// wxWidgets
#include "wx/string.h"

// forward declarations
class Tracer;
class wxWindow;

namespace svn
{
  class Context;
  class StatusSel;
  class Targets;
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
   * if set then the tree will be updated as well.
   */
  static const unsigned int UPDATE_TREE;

  /**
   * constructor
   *
   * @param parent parent window
   * @param name of the action
   * @param flags
   */
  Action (wxWindow * parent, const wxString & name, 
          unsigned int flags = 0);

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
   * sets a selection of @ref Status for the action.
   * Not every action will need it.
   *
   * The contents of @a statusSel will be copied
   *
   * @param targets
   */
  void
  SetStatusSel (const svn::StatusSel & statusSel);

  /**
   * @return the status selection for this action
   */
  const svn::StatusSel &
  GetStatusSel () const;

  /**
   * @return the targets for this action
   */
  const svn::Targets &
  GetTargets () const;

  /**
   * @return a single target for this action
   */
  const svn::Path
  GetTarget () const;


  /**
   * retrieves the flags for this action.
   *
   * @see DONT_UPDATE
   */
  unsigned int
  GetFlags () const;


  /** set the name of the action */
  void
  SetName (const wxString & name);

  /** returns the name of the action */
  const wxString &
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
  GetPathAsTempFile (
    const svn::Path & path,
    const svn::Revision & revision = svn::Revision::HEAD);

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
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
