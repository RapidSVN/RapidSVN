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

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/exception.hpp"

// app
#include "ids.hpp"
//REMOVE #include "tracer.hpp"
#include "add_action.hpp"
#include "svn_notify.hpp"

AddAction::AddAction (wxWindow * parent)
  : Action (parent, actionWithTargets)
{
}

AddAction::~AddAction ()
{
}

bool
AddAction::Prepare ()
{
  // No dialog for Add
  return Action::Prepare ();
}

bool
AddAction::Perform ()
{
  svn::Client client;
  SvnNotify notify (GetTracer ());
  client.notification (&notify);

  const std::vector<svn::Path> & v (GetTargets ());
  std::vector<svn::Path>::const_iterator it;

  for (it = v.begin (); it != v.end (); it++)
  {
    const svn::Path & path = *it;

    client.add (path.c_str (), false);
  }

  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
