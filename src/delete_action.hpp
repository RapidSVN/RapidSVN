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
#ifndef _DELETE_ACTION_H_INCLUDED_
#define _DELETE_ACTION_H_INCLUDED_

// svncpp
#include "svncpp/targets.hpp"

// app
#include "action.hpp"
#include "delete_dlg.hpp"

class DeleteAction:public Action
{
public:
  DeleteAction (wxWindow * parent);

  virtual bool Perform ();
  virtual bool Prepare ();

  /**
   * Describe which targets the action can perform upon
   */
  static unsigned int GetBaseFlags ()
  {
    return SINGLE_TARGET|MULTIPLE_TARGETS|RESPOSITORY_TYPE|VERSIONED_WC_TYPE|UNVERSIONED_WC_TYPE;
  }

private:
  bool m_force;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
