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

#ifndef _LOG_ACTION_H_INCLUDED_
#define _LOG_ACTION_H_INCLUDED_

// app
#include "action.hpp"

class LogAction : public Action
{
public:
  LogAction (wxWindow * parent);

  /**
   * @see Action
   */
  virtual bool Prepare ();

  /**
   * @see Action
   */
  virtual bool Perform ();
  void setLogMessage (const char * message);

private:
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
