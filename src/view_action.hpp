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
#ifndef _VIEW_ACTION_H_INCLUDED_
#define _VIEW_ACTION_H_INCLUDED_

// app
#include "action.hpp"
#include "get_data.hpp"

// forward declarations
class wxWindow;

/**
 * This action downloads a specific revision 
 * of a file (specified in @a GetData) and 
 * displays the file
 */
class ViewAction:public Action
{
public:
  /**
   * Constructor. This constructor creates an
   * action, that will not prompt for
   * input in Prepare. Instead, the settings
   * in @a data will be used
   *
   * @param parent parent window
   * @param data update data
   */
  ViewAction (wxWindow * parent, const GetData & data);

  virtual bool Perform ();
  virtual bool Prepare ();

  /**
   * Describe which targets the action can perform upon
   */
  static unsigned int GetBaseFlags ()
  {
    return DONT_UPDATE|WITHOUT_TARGET;
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
