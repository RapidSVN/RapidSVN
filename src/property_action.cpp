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
#include "svncpp/exception.hpp"

// app
#include "property_action.hpp"
#include "property_dlg.hpp"

PropertyAction::PropertyAction (wxWindow * parent, 
                                Tracer * tr, const char * target)
  : Action (parent, tr, false), m_target(target)
{
}

bool
PropertyAction::Prepare ()
{
  try
  {
    PropertyDlg dlg (GetParent (), m_target);
    dlg.ShowModal ();
  }
  catch (svn::Exception &)
  {
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
