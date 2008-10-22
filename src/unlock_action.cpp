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
#include "svncpp/status_selection.hpp"

// app
#include "unlock_action.hpp"
#include "unlock_dlg.hpp"
#include "ids.hpp"
#include "utils.hpp"

UnlockAction::UnlockAction(wxWindow * parent)
    : Action(parent, _("Unlock"))
{
}

bool
UnlockAction::Prepare()
{
  if (!Action::Prepare())
  {
    return false;
  }

  UnlockDlg dlg(GetParent());

  if (dlg.ShowModal() != wxID_OK)
  {
    return false;
  }

  m_force = dlg.GetForce();

  return true;
}

bool
UnlockAction::Perform()
{
  svn::Client client(GetContext());

  const svn::Targets & statusSel = GetTargets();

  client.unlock(statusSel, m_force);

  return true;
}

bool
UnlockAction::CheckStatusSel(const svn::StatusSel & statusSel)
{
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
