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
#include "cleanup_action.hpp"
#include "ids.hpp"
#include "tracer.hpp"
#include "utils.hpp"

CleanupAction::CleanupAction (wxWindow * parent)
  : Action (parent, _("Cleanup"), GetBaseFlags ())
{
}

bool
CleanupAction::Prepare ()
{
  return Action::Prepare ();
}

bool
CleanupAction::Perform ()
{
  svn::Client client (GetContext ());
  const svn::Path & path = GetPath ();

  wxSetWorkingDirectory (Utf8ToLocal (path.c_str ()));
  client.cleanup (path.c_str ());

  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
