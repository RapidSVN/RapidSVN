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

// wxwindows
#include "wx/intl.h"

// app
#include "property_action.hpp"
#include "property_dlg.hpp"

/**
 * right now this supports only one target
 */
PropertyAction::PropertyAction (wxWindow * parent)
  : Action (parent, _("Property"), GetBaseFlags ())
{
}

bool
PropertyAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  PropertyDlg dlg (GetParent (), GetContext (), GetTarget ());

  return dlg.ShowModal () == wxID_OK;
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
