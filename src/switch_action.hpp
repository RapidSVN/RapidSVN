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
#ifndef _SWITCH_ACTION_H_INCLUDED_
#define _SWITCH_ACTION_H_INCLUDED_

// svncpp
#include "svncpp/targets.hpp"

// app
#include "action.hpp"
#include "update_data.hpp"

/**
 * this action class can be used to copy, move and rename
 * files and folders. Right now it supports only a single target
 */
class SwitchAction:public Action
{
private:
  UpdateData m_data;

public:
  /**
   * constructor
   *
   * @param parent parent window
   */
  SwitchAction (wxWindow * parent);

  virtual bool Perform ();
  virtual bool Prepare ();
  
  /**
   * Describe which targets the action can perform upon
   */
  static unsigned int GetBaseFlags ()
  {
    return SINGLE_TARGET|VERSIONED_WC_TYPE;
  }
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
