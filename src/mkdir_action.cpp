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

// svncpp
#include "svncpp/client.hpp"

// wxwindows
#include "wx/intl.h"

// app
#include "destination_dlg.hpp"
#include "mkdir_action.hpp"

MkdirAction::MkdirAction (wxWindow * parent, const char * path)
 : Action (parent, _("Mkdir"), GetBaseFlags ()),
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

  DestinationDlg dlg (GetParent (), _("Make directory"),
                      _("Directory:"));

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  wxString target (dlg.GetDestination ()); 
  m_target = target.Strip (wxString::both);
  return true;
}

bool
MkdirAction::Perform ()
{
  svn::Client client (GetContext ());

  // add target to path
  wxString newDir (m_path + m_target);

  svn::Path target (m_path.c_str ());
  target.addComponent (m_target.c_str ());

  client.mkdir (target, "");
  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
