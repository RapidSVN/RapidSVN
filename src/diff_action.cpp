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

// app
#include "diff_action.hpp"
#include "diff_dlg.hpp"
#include "ids.hpp"
#include "utils.hpp"

DiffAction::DiffAction (wxWindow * parent)
  : Action (parent, _("Diff"), actionWithSingleTarget)
{
}

bool
DiffAction::Prepare ()
{
  if (!Action::Prepare ())
    return false;

  DiffDlg dlg (GetParent ());
  if( dlg.ShowModal () != wxID_OK )
    return false;

  // @todo INSERT CODE HERE ;-)

  return true;
}

bool
DiffAction::Perform ()
{
  // @todo INSERT CODE HERE ;-)
 
  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
