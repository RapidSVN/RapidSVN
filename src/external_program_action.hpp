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
#ifndef _EXTERNAL_PROGRAM_ACTION_HPP_INCLUDED_
#define _EXTERNAL_PROGRAM_ACTION_HPP_INCLUDED_

// app
#include "action.hpp"

class ExternalProgramAction:public Action
{
public:
  /**
   * Launch an external editor on the target
   *
   * @param verb_id         The index of the verb, found by using a Verblist,
   *                        or -1 to use whichever is the default.
   * @param treat_as_folder If true, the target will be treated like a folder
   *                        (typically: "explored"). If the target is a file,
   *                        then only it's path will be used.
   *                        If false, the target is assumed to be a file, and
   *                        the verb should be explicit.
   */
  ExternalProgramAction (wxWindow * parent, long verb_id, 
    bool treat_as_folder);

  virtual bool Perform ();
  virtual bool Prepare ();

  /**
   * Describe which targets the action can perform upon
   */
  static unsigned int GetBaseFlags ()
  {
    return UPDATE_LATER|SINGLE_TARGET|RESPOSITORY_TYPE|VERSIONED_WC_TYPE|UNVERSIONED_WC_TYPE;
  }

protected:
  long m_verb_id;
  bool m_treat_as_folder;
};

#endif 
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
