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

// wxwindows
#include "wx/intl.h"

// app
#include "ids.hpp"
#include "mkdir_dlg.hpp"
#include "mkdir_action.hpp"
#include "svn_notify.hpp"

MkdirAction::MkdirAction (wxWindow * parent, const char * path)
 : Action (parent, _("Mkdir"), actionWithoutTarget),
   m_path (path)
{
}

bool
MkdirAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  MkdirDlg dlg  ( GetParent (), "");

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  wxString target (dlg.GetTarget ()); 
  m_target = target.Strip (wxString::both);
  return true;
}

bool
MkdirAction::Perform ()
{
  svn::Client client (GetContext ());
  SvnNotify notify (GetTracer ());
  client.notification (&notify);

  // add target to path
  wxString newDir (m_path + m_target);

  svn::Path target (m_path.c_str ());
  target.addCompontent (m_target.c_str ());

  client.mkdir (target, "");
  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
