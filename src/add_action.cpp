/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
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
#include "include.hpp"
#include "tracer.hpp"
#include "rapidsvn_app.hpp"
#include "add_action.hpp"
#include "svn_notify.hpp"

AddAction::AddAction (wxFrame * frame, Tracer * tr, const svn::Targets & targets)
  : ActionThread (frame), m_targets(targets)
{
  SetTracer (tr, FALSE);        // do not own the tracer
}

void
AddAction::Perform ()
{
  // No dialog for Add. Just start the thread.

  // #### TODO: check errors and throw an exception
  // create the thread
  Create ();

  // here we start the action thread
  Run ();
}

void *
AddAction::Entry ()
{
  svn::Client client;
  SvnNotify notify (GetTracer ());
  client.notification (&notify);

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
    }

  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);

  return NULL;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
