/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.  
 * If not, see <http://www.gnu.org/licenses/>.
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
 : Action (parent, _("Mkdir"), UPDATE_TREE),
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

  svn::Path pathUtf8 (PathUtf8 (m_path));
  svn::Path targetUtf8 (PathUtf8 (m_target));
  pathUtf8.addComponent (targetUtf8.c_str ());

  client.mkdir (pathUtf8);
  return true;
}

bool
MkdirAction::CheckStatusSel (const svn::StatusSel & statusSel)
{
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
