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
#include "tracer.hpp"
#include "add_action.hpp"
#include "svn_notify.hpp"

AddAction::AddAction (wxWindow * parent, 
                      const svn::Targets & targets)
  : Action (parent), m_targets (targets)
{
}

bool
AddAction::Prepare ()
{
  // No dialog for Add
  return true;
}

bool
AddAction::Perform ()
{
  svn::Client client;
  SvnNotify notify (GetTracer ());
  client.notification (&notify);
  bool result = true;

  const std::vector<svn::Path> & v = m_targets.targets ();
  std::vector<svn::Path>::const_iterator it;

  for (it = v.begin (); it != v.end (); it++)
  {
    const svn::Path & path = *it;

    try
    {
      client.add (path.c_str (), false);
    }
    catch (svn::ClientException &e)
    {
      PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                       ACTION_EVENT);
      result = false;
    }

  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);

  return false;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
