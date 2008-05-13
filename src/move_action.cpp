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
#include "hist_entries.hpp"
#include "move_action.hpp"
#include "utils.hpp"

MoveAction::MoveAction (wxWindow * parent, int kind)
 : Action (parent, wxEmptyString, UPDATE_TREE),
   m_kind (kind)
{
  if (kind == MOVE_MOVE)
    SetName (_("Move"));
  else
    SetName (_("Copy"));
}

bool
MoveAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  // create flags for the dialog
  int flags = 0;

  if (m_kind == MOVE_MOVE)
    flags = DestinationDlg::WITH_FORCE;

  // create description for the dialog
  DestinationDlg dlg (GetParent (), GetName (),
                      _("Select destination:"), flags,
                      wxEmptyString,
                      HISTORY_COPYMOVE_DESTINATION);

  if (dlg.ShowModal () != wxID_OK)
    return false;

  m_destination = dlg.GetDestination ();
  m_force = dlg.GetForce ();

  return true;
}

bool
MoveAction::Perform ()  
{
  svn::Client client (GetContext ());
  svn::Path destPath (GetPath().path());
  destPath.addComponent((PathUtf8 (m_destination)).c_str ());
  svn::Revision unusedRevision;
  std::vector<svn::Path> targets (GetTargets ().targets ());
  std::vector<svn::Path>::iterator it;

  for (it = targets.begin (); it != targets.end (); it++)
  {
    svn::Path srcPath (*it);

    if (srcPath == GetPath ())
      srcPath = ".";

    if (m_kind == MOVE_MOVE)
      client.move (srcPath, unusedRevision, destPath, m_force);
    else
      client.copy (srcPath, unusedRevision, destPath);
  }
  return true;
}

bool
MoveAction::CheckStatusSel (const svn::StatusSel & statusSel)
{
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
