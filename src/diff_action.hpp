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
#ifndef _DIFF_ACTION_H_INCLUDED
#define _DIFF_ACTION_H_INCLUDED

// app
#include "action.hpp"

class DiffAction:public Action
{
public:
  /**
   * Constructor
   *
   * @param parent parent window
   */
  DiffAction (wxWindow * parent);


  /** Destructor */
  virtual ~DiffAction ();

  virtual bool Perform ();
  virtual bool Prepare ();

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
