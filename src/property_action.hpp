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

#ifndef _PROPERTY_ACTION_H_INCLUDED_
#define _PROPERTY_ACTION_H_INCLUDED_

#include "action.hpp"

class PropertyAction : public Action
{
public:
  PropertyAction (wxWindow * parent);

  virtual bool Prepare ();
  virtual bool Perform ();

  /**
   * Describe which targets the action can perform upon
   */
  static unsigned int GetBaseFlags ()
  {
    return SINGLE_TARGET|RESPOSITORY_TYPE|VERSIONED_WC_TYPE;
  }
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
