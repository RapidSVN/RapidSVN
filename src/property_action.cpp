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

#include "svncpp/property.hpp"
#include "include.hpp"
#include "wx/resource.h"
#include "rapidsvn_app.hpp"
#include "property_action.hpp"
#include "svn_notify.hpp"


PropertyAction::PropertyAction (wxFrame * frame, 
                      Tracer * tr, const char * target)
  : ActionThread (frame), _target(target)
{
  thisframe = frame;
  SetTracer (tr, FALSE);        // do not own the tracer
}

void
PropertyAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.
  svn::Property property;

  property.loadPath (_target);

  if(!property.isVersioned ())
	  return;

  propDialog = new PropertyDlg(thisframe, &property);

  if (propDialog->ShowModal () == wxID_OK)
  {
    // #### TODO: check errors and throw an exception
    // create the thread
    Create ();

    // here we start the action thread
    Run ();

    ////////////////////////////////////////////////////////////
  }

  // destroy the dialog
  propDialog->Close (TRUE);
}

void *
PropertyAction::Entry ()
{
  return NULL;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
