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

MkdirAction::MkdirAction (wxWindow * parent)
 : Action (parent, _("Mkdir"), actionWithoutTarget)
{
}

bool
MkdirAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  MkdirDlg dlg  ( GetParent (), &m_data);

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }
  
  m_data.Target = m_data.Target.Strip(wxString::both);
  return true;
}

bool
MkdirAction::Perform ()
{
  svn::Client client (GetContext ());
  SvnNotify notify (GetTracer ());
  client.notification (&notify);

  client.mkdir (m_data.Target.c_str (), m_data.LogMessage.c_str ());
  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
