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
#ifndef _MKDIR_ACTION_H_INCLUDED_
#define _MKDIR_ACTION_H_INCLUDED_

// app
#include "action.hpp"

class MkdirAction:public Action
{
private:
  wxString m_path;
  wxString m_target;

public:
  /**
   * make directory action.
   *
   * @param parent parent window
   * @param path path in which the directory is to
   *             be created
   */
  MkdirAction (wxWindow * parent, const wxString & path);

  virtual bool Perform ();
  virtual bool Prepare ();

  /**
   * Describe which targets the action can perform upon
   */
  static unsigned int GetBaseFlags ()
  {
    return SINGLE_TARGET| MULTIPLE_TARGETS|
           RESPOSITORY_TYPE| VERSIONED_WC_TYPE| UNVERSIONED_WC_TYPE|
           UPDATE_TREE;
  }
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
