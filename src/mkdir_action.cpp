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
#include "wx/wx.h"

// app
#include "destination_dlg.hpp"
#include "mkdir_action.hpp"
#include "utils.hpp"

MkdirAction::MkdirAction (wxWindow * parent, const wxString & path)
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
  // TODO: What is newDir for???
  
  std::string pathUtf8 (LocalToUtf8 (m_path));
  std::string targetUtf8 (LocalToUtf8 (m_target));
  svn::Path target (pathUtf8);
  target.addComponent (targetUtf8);

  client.mkdir (target, "");
  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
