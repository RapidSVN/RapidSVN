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
#ifndef _ADD_ACTION_H_INCLUDED_
#define _ADD_ACTION_H_INCLUDED_

// svncpp
#include "svncpp/path.hpp"
#include "svncpp/targets.hpp"

// app
#include "action.hpp"

class AddAction:public Action
{
private:
  svn::Targets m_targets;

public:
  AddAction (wxWindow * parent, const svn::Targets & trgts);

  bool Prepare ();
  bool Perform ();
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
