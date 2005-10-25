/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxWidgets
#include "wx/wx.h"

// svncpp
#include "svncpp/client.hpp"

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
