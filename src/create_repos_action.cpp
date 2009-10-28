/*
 * ====================================================================
 * Copyright (c) 2002-2009 The RapidSvn Group.  All rights reserved.
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
#include "create_repos_action.hpp"
//#include "create_repos_dlg.hpp"

CreateRepositoryAction::CreateRepositoryAction(wxWindow * parent)
    : Action(parent, _("Add"), 0)
{
}

CreateRepositoryAction::~CreateRepositoryAction()
{
}

bool
CreateRepositoryAction::Prepare()
{
  // TODO
  return false;
}

bool
CreateRepositoryAction::Perform()
{
  // TODO
  //svn::Client client(GetContext());
  return true;
}


bool
CreateRepositoryAction::CheckStatusSel(const svn::StatusSel & WXUNUSED(statusSel))
{
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
