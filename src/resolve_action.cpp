/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxWidgets
#include "wx/intl.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/status_selection.hpp"
#include "svncpp/targets.hpp"

// app
#include "resolve_action.hpp"

ResolveAction::ResolveAction(wxWindow * parent)
    : Action(parent, _("Resolve"))
{
}

bool
ResolveAction::Prepare()
{
  // No dialog for Resolve.
  return Action::Prepare();
}

bool
ResolveAction::Perform()
{
  const svn::PathVector v = GetTargets();
  svn::PathVector::const_iterator it;

  svn::Client client(GetContext());
  for (it = v.begin(); it != v.end(); it++)
  {
    const svn::Path & path = *it;

    client.resolved(path.c_str(), false);
  }

  return true;
}

bool
ResolveAction::CheckStatusSel(const svn::StatusSel & statusSel)
{
  // allow only local+versioned files
  if (!statusSel.hasVersioned())
    return false;

  if (statusSel.hasUnversioned())
    return false;

  if (!statusSel.hasLocal())
    return false;

  if (!statusSel.hasFiles())
    return false;

  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
