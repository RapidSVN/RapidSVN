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

LogAction::LogAction (wxWindow * parent, Tracer * tr, const char * target)
  : Action (parent, tr, false), m_target(target)
{
}

bool
LogAction::Prepare ()
{
  try
  {
    svn::Log log (m_target, svn::Revision::START, svn::Revision::HEAD);
    LogDlg dlg (GetParent (), log);
    dlg.ShowModal ();
  }
  catch (svn::ClientException & e)
  {
    //TODO handle error (e.g. tracer message)
  }

  return false;
}

bool
LogAction::Perform ()
{
  return true;
}

// void 
// LogAction::setLogMessage (const char * message)
// {
//   m_logDialog->setLogMessage (message);
// }

/* -----------------------------------------------------------------
 * local variables: eval: (load-file "../rapidsvn-dev.el") end:
 */
