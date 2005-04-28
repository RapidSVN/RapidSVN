/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _ADD_ACTION_H_INCLUDED_
#define _ADD_ACTION_H_INCLUDED_

// svncpp
#include "svncpp/path.hpp"

// app
#include "action.hpp"

class AddAction:public Action
{
public:
  /**
   * Constructor
   *
   * @param parent parent window
   * @param recursive if true, add files/directories recursive
   */
  AddAction (wxWindow * parent, bool recursive=false);
  virtual ~AddAction ();

  virtual bool Prepare ();
  virtual bool Perform ();

  static unsigned int GetBaseFlags ()
  {
    return SINGLE_TARGET|MULTIPLE_TARGETS|UNVERSIONED_WC_TYPE;
  }
  
private:
  struct Data;
  Data * m;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
