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
#include "svncpp/exception.hpp"

// wxwindows
#include "wx/intl.h"

// app
#include "property_action.hpp"
#include "property_dlg.hpp"

/**
 * right now this supports only one target
 */
PropertyAction::PropertyAction (wxWindow * parent)
  : Action (parent, _("Property"), actionWithSingleTarget)
{
}

bool
PropertyAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  try
  {
    PropertyDlg dlg (GetParent (), GetTarget ());
    dlg.ShowModal ();
  }
  catch (svn::Exception &)
  {
    //TODO handle this
  }

  return false;
}

bool
PropertyAction::Perform ()
{
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
