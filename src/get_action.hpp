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
#ifndef _GET_ACTION_H_INCLUDED_
#define _GET_ACTION_H_INCLUDED_

// app
#include "action.hpp"
#include "get_data.hpp"

// forward declarations
class wxWindow;

/**
 * This class works very much like UpdateAction.
 * There are differences:
 * - there wont be any dialogs shown
 * - the action ignores "targets" 
 * - works only on a single path that is 
 *   pass in the @a GetData structure
 */
class GetAction:public Action
{
public:
  /**
   * Constructor. This constructor creates an
   * get action, that will not prompt for
   * input in Prepare. Instead, the settings
   * in @a data will be used
   *
   * @param parent parent window
   * @param data update data
   */
  GetAction (wxWindow * parent, const GetData & data);

  virtual bool Perform ();
  virtual bool Prepare ();

  /**
   * Describe which targets the action can perform upon
   */
  static unsigned int GetBaseFlags ()
  {
    return WITHOUT_TARGET;
  }

private:
  GetData m_data;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
