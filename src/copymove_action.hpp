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
#ifndef _COPYMOVE_ACTION_H_INCLUDED_
#define _COPYMOVE_ACTION_H_INCLUDED_

// svncpp
#include "svncpp/targets.hpp"

// app
#include "action.hpp"
#include "delete_dlg.hpp"

class CopyMoveAction:public Action
{
private:
  bool m_copy;
  wxString m_destination;
  bool m_force;

public:
  /**
   * constructor
   *
   * @param parent parent window
   * @param copy true=copy, false=move
   */
  CopyMoveAction (wxWindow * parent, bool copy);

  virtual bool Perform ();
  virtual bool Prepare ();
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
