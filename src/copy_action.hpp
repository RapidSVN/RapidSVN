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
#ifndef _COPY_ACTION_H_INCLUDED_
#define _COPY_ACTION_H_INCLUDED_

// svncpp
//REMOVE #include "svncpp/targets.hpp"

// app
#include "action.hpp"

// forward declarations
namespace svn
{
  class Path;
}

class CopyAction:public Action
{
public:
  CopyAction (wxWindow * parent);

  virtual bool Perform ();
  virtual bool Prepare ();

private:
  const svn::Path DestinationPath (const svn::Path & src);

};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
