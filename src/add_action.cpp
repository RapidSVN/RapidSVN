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
#include "ids.hpp"
#include "add_action.hpp"

struct AddAction::Data
{
public:
  bool recursive;

  Data(bool recursive_)
      : recursive(recursive_)
  {
  }
};

AddAction::AddAction(wxWindow * parent, bool recursive)
    : Action(parent, _("Add"), 0),
    m(new Data(recursive))
{
}

AddAction::~AddAction()
{
  delete m;
}

bool
AddAction::Prepare()
{
  // No dialog for Add
  return Action::Prepare();
}

bool
AddAction::Perform()
{
  svn::Client client(GetContext());

  const svn::PathVector & v = GetTargets().targets();
  svn::PathVector::const_iterator it;

  for (it = v.begin(); it != v.end(); it++)
  {
    const svn::Path & path = *it;

    client.add(path.c_str(), m->recursive);
  }

  return true;
}


bool
AddAction::CheckStatusSel(const svn::StatusSel & statusSel)
{
  if (0 == statusSel.size())
    return false;

  if (statusSel.hasUrl())
    return false;

  if (statusSel.hasVersioned())
    return false;

  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
