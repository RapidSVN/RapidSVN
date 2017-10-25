/*
 * ====================================================================
 * Copyright (c) 2002-2012 The RapidSVN Group.  All rights reserved.
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
#include "add_recursive_action.hpp"

AddRecursiveAction::AddRecursiveAction(wxWindow * parent)
  : Action(parent, _("Add"), 0)
{
}

AddRecursiveAction::~AddRecursiveAction()
{
}

bool
AddRecursiveAction::Prepare()
{
  // No dialog for Add
  return Action::Prepare();
}

bool
AddRecursiveAction::Perform()
{
  svn::Client client(GetContext());

  // with recursion we have to cycle over the selection
  // and find unversioned subdirectories and add them
  svn::StatusFilter filter;
  filter.showUnversioned = true;

  svn::PathVector paths = GetTargets().targets();

  do
  {
    svn::PathVector nextPaths;
    svn::PathVector::const_iterator it;

    for (it = paths.begin(); it != paths.end(); it++)
    {
      const svn::Path & path = *it;

      svn::StatusEntries entries;
      client.status(path.c_str(), filter, true, false, entries);

      svn::StatusEntries::const_iterator itStatus;

      for (itStatus = entries.begin(); itStatus != entries.end(); itStatus++)
      {
        const svn::Status & status = *itStatus;
        client.add(status.path(), false);
        nextPaths.push_back(status.path());
      }
    }

    // for the next round we copy the newly added files/dirs
    // and find subdirectories there
    paths = nextPaths;
  }
  while (paths.size() > 0);

  return true;
}


bool
AddRecursiveAction::CheckStatusSel(const svn::StatusSel & statusSel)
{
  if (0 == statusSel.size())
    return false;

  if (statusSel.hasUrl())
    return false;

  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
