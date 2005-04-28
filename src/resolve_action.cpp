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

// svncpp
#include "svncpp/client.hpp"

// wxWidgets
#include "wx/intl.h"

// app
#include "resolve_action.hpp"

ResolveAction::ResolveAction (wxWindow * parent)
  : Action (parent, _("Resolve"), GetBaseFlags ())
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
  const std::vector<svn::Path> v = GetTargets ();
  std::vector<svn::Path>::const_iterator it;

  svn::Client client (GetContext ());
  for (it = v.begin (); it != v.end (); it++)
  {
    const svn::Path & path = *it;

    client.resolved (path.c_str (), false);
  }

  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
