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
#ifndef _MERGE_ACTION_H_INCLUDED_
#define _MERGE_ACTION_H_INCLUDED_

// svncpp
#include "svncpp/path.hpp"

// app
#include "action.hpp"
#include "merge_dlg.hpp"

class MergeAction:public Action
{
public:
  MergeAction (wxWindow * frame);

  /**
   * Constructor, that takes read configured @a MergeData
   * so dialog will be displayed with given data
   *
   * @param parent parent window
   * @param data merge data
   */
  MergeAction (wxWindow * frame, MergeData & data);

  virtual bool Prepare ();
  virtual bool Perform ();

  /**
   * Describe which targets the action can perform upon
   */
  static unsigned int GetBaseFlags ()
  {
    return WITHOUT_TARGET;
  }

private:
  MergeData m_data;
  bool m_calledByLogDlg;
};

#endif

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
