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
#ifndef _MOVE_ACTION_H_INCLUDED_
#define _MOVE_ACTION_H_INCLUDED_

// svncpp
#include "svncpp/targets.hpp"

// app
#include "action.hpp"

enum
{
  MOVE_MOVE,
  MOVE_COPY,
  MOVE_RENAME
};

/**
 * this action class can be used to copy, move and rename
 * files and folders. Right now it supports only a single target
 */
class MoveAction:public Action
{
private:
  int m_kind;
  wxString m_destination;
  bool m_force;

public:
  /**
   * constructor
   *
   * @param parent parent window
   * @param type kind of action (MOVE_MOVE, MOVE_COPY, MOVE_RENAME)
   */
  MoveAction (wxWindow * parent, int kind);

  virtual bool Perform ();
  virtual bool Prepare ();
  
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
