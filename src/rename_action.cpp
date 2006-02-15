/*
 * ====================================================================
 * Copyright (c) 2002-2006 The RapidSvn Group.  All rights reserved.
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
#include "rename_action.hpp"
#include "destination_dlg.hpp"
#include "utils.hpp"

RenameAction::RenameAction (wxWindow * parent)
 : Action (parent, wxEmptyString, GetBaseFlags ())
{
  SetName (_("Rename"));

}

bool
RenameAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  // show destination dialog
  DestinationDlg dlg (GetParent (), GetName (),
                      _("Enter new name:"), 
                      DestinationDlg::WITH_FORCE);
  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  m_destination = dlg.GetDestination ();
  m_force = dlg.GetForce ();

  return true;
}

bool
RenameAction::Perform ()
{
  svn::Client client (GetContext ());

  svn::Path srcPath = GetTarget ();
  svn::Path destPath (PathUtf8 (m_destination));
  svn::Revision unusedRevision;

  std::string basename;
  std::string dirpath;
  srcPath.split (dirpath, basename);

  svn::Path newDestPath (dirpath);
  newDestPath.addComponent(destPath.c_str ());

  client.move (srcPath, unusedRevision, newDestPath, m_force);

  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
