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
#include "svncpp/log.hpp"
#include "svncpp/revision.hpp"

// app
#include "log_action.hpp"
#include "log_dlg.hpp"

LogAction::LogAction (wxWindow * parent)
  : Action (parent, actionWithSingleTarget)
{
}

bool
LogAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }
      
  svn::Path target = GetTarget ();
  svn::Log log (target.c_str (), 
                svn::Revision::START, 
                svn::Revision::HEAD);
  LogDlg dlg (GetParent (), log);
  dlg.ShowModal ();

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
