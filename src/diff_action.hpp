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
#ifndef _DIFF_ACTION_H_INCLUDED
#define _DIFF_ACTION_H_INCLUDED

// app
#include "action.hpp"

// forward declaration
struct DiffData;

class DiffAction:public Action
{
public:
  /**
   * Constructor
   *
   * @param parent parent window
   */
  DiffAction (wxWindow * parent);


  /**
   * Constructor, that takes read configured @a DiffData,
   * so no dialog will be displayed
   *
   * @param parent parent window
   * @param data diff data
   */
  DiffAction (wxWindow * parent, DiffData & data);


  /** Destructor */
  virtual ~DiffAction ();

  virtual bool Perform ();
  virtual bool Prepare ();

  /**
   * Describe which targets the action can perform upon
   */
  static unsigned int GetBaseFlags ()
  {
    return DONT_UPDATE|SINGLE_TARGET|MULTIPLE_TARGETS|RESPOSITORY_TYPE|VERSIONED_WC_TYPE|UNVERSIONED_WC_TYPE;
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
