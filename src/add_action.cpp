/*
 * ====================================================================
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#include "svncpp/modify.h"
#include "include.h"
#include "tracer.h"
#include "rapidsvn_app.h"
#include "add_action.h"
#include "svn_notify.h"

AddAction::AddAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr, apr_array_header_t * trgts):ActionThread (frame, __pool),
  targets
  (trgts)
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
  svn::Modify modify;
  SvnNotify notify (GetTracer ());
  modify.notification (&notify);

  for (int i = 0; i < targets->nelts; i++)
  {
    const char *target = ((const char **) (targets->elts))[i];

    try
    {
      modify.add (target, false);
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
