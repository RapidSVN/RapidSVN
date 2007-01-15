/*
 * ====================================================================
 * Copyright (c) 2002-2007 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
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

// app
#include "ids.hpp"
#include "add_action.hpp"

struct AddAction::Data
{
public:
  bool recursive;

  Data (bool recursive_)
    : recursive (recursive_)
  {
  }
};

AddAction::AddAction (wxWindow * parent, bool recursive)
  : Action (parent, _("Add"), GetBaseFlags ()),
    m (new Data (recursive))
{
}

AddAction::~AddAction ()
{
  delete m;
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
  svn::Client client (GetContext ());

  const std::vector<svn::Path> & v = GetTargets ();
  std::vector<svn::Path>::const_iterator it;

  for (it = v.begin (); it != v.end (); it++)
  {
    const svn::Path & path = *it;

    client.add (path.c_str (), m->recursive);
  }

  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
