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
#include "svncpp/revision.hpp"

// wxwindows
#include "wx/intl.h"

// app
#include "log_action.hpp"
#include "log_dlg.hpp"

LogAction::LogAction (wxWindow * parent)
  : Action (parent, _("Log"), GetBaseFlags ())
{
}

bool
LogAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  svn::Client client (GetContext ());

  const svn::LogEntries * entries;
  svn::Path target = GetTarget ();
  {
    wxBusyCursor cursor;
    entries = client.log (target.c_str (), svn::Revision::START, 
                          svn::Revision::HEAD);
  }
  {
    LogDlg dlg (GetParent (), target.c_str (), entries);
    dlg.ShowModal ();
  }
  delete entries;

  return false;
}

bool
LogAction::Perform ()
{
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
