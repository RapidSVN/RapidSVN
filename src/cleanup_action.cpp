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
#include "wx/wx.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/status_selection.hpp"

// app
#include "cleanup_action.hpp"
#include "ids.hpp"
#include "tracer.hpp"
#include "utils.hpp"

CleanupAction::CleanupAction(wxWindow * parent)
    : Action(parent, _("Cleanup"), 0)
{
}

bool
CleanupAction::Prepare()
{
  return Action::Prepare();
}

bool
CleanupAction::Perform()
{
  svn::Client client(GetContext());
  const svn::Path & path = GetPath();

  wxSetWorkingDirectory(Utf8ToLocal(path.c_str()));
  client.cleanup(path.c_str());

  return true;
}

bool
CleanupAction::CheckStatusSel(const svn::StatusSel & statusSel)
{
  if (statusSel.size() != 1)
    return false;

  if (statusSel.hasUnversioned())
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
