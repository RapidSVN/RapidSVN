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

// wxwindows
#include "wx/intl.h"

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
  : Action (parent, _("Add"), actionWithTargets),
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
  svn::Client client;

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
