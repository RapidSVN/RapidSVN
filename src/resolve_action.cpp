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
#include "svncpp/exception.hpp"
#include "svncpp/client.hpp"

// wxwindows
#include "wx/intl.h"

// app
#include "ids.hpp"
#include "tracer.hpp"
#include "resolve_action.hpp"
#include "svn_notify.hpp"

ResolveAction::ResolveAction (wxWindow * parent)
  : Action (parent, _("Resolve"), actionWithTargets)
{
}

bool
ResolveAction::Prepare ()
{
  // No dialog for Resolve. 
  return Action::Prepare ();
}

bool
ResolveAction::Perform ()
{
  svn::Client client;
  SvnNotify notify (GetTracer ());
  client.notification (&notify);
  bool result = true;

  const std::vector<svn::Path> v = GetTargets ();
  std::vector<svn::Path>::const_iterator it;

  for (it = v.begin (); it != v.end (); it++)
  {
    const svn::Path & path = *it;

    client.resolve (path.c_str (), false);
  }

  return result;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
