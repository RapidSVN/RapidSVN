/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
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
#include "wx/wx.h"

// svncpp
#include "svncpp/client.hpp"

// app
#include "revert_action.hpp"

RevertAction::RevertAction (wxWindow * parent)
  : Action (parent, _("Revert"), GetBaseFlags ())
{
}

bool
RevertAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  wxMessageDialog dlg (GetParent (),
                       _("Do you want to revert local changes?"),
                       _("Revert"), wxYES_NO | wxICON_QUESTION);

  if (dlg.ShowModal () != wxID_YES)
  {
    return false;
  }

  return true;
}


bool
RevertAction::Perform ()
{
  svn::Client client;
  client.revert (GetTargets (), false);

  return true;
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
