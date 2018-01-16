/*
 * ====================================================================
 * Copyright (c) 2002-2012 The RapidSVN Group.  All rights reserved.
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
#include "upgrade_action.hpp"
#include "ids.hpp"
#include "tracer.hpp"
#include "utils.hpp"

UpgradeAction::UpgradeAction(wxWindow * parent)
  : Action(parent, _("Upgrade"), 0)
{
}

bool
UpgradeAction::Prepare()
{
  if (!Action::Prepare())
  {
    return false;
  }

  svn::Path selectedPath(GetPath());
  if (!selectedPath.length()) {
    wxMessageBox(_("No path is selected. Cannot upgrade working copy."),
                 _("Upgrade Working Copy"), wxOK);
    return false;
  }

  wxString pathMsg = wxString::Format(_("Path: %s\n"), selectedPath.c_str());
  pathMsg.Append(_("Notice: if you upgrade your working copy, you won't be able to access the "
                   "upgraded working copy with older SVN clients anymore!\n"
                   "Do you really want to upgrade this working copy?"));
  int id = wxMessageBox(pathMsg, _("Upgrade Working Copy"), wxYES_NO);
  if (id != wxYES)
  {
    return false;
  }

  return true;
}

bool
UpgradeAction::Perform()
{
  svn::Client client(GetContext());
  const svn::Path & path = GetPath();

  const wxString & dir = Utf8ToLocal(path.c_str());
  if (!dir.empty())
    wxSetWorkingDirectory(dir);

  client.upgrade(path);

  return true;
}

bool
UpgradeAction::CheckStatusSel(const svn::StatusSel & statusSel)
{
  svn::Path path = statusSel.target();
  /*printf("statussel s=%d url=%d n1=%s\n", (int)statusSel.size(),
   * statusSel.hasUrl(), path.c_str()); */

  if (statusSel.size() == 0) {
    // an outdated working copy doesn't appear in the statusSel targets,
    // therefore nothing will be selected
    return true;
  }
  else if (statusSel.size() == 1 && !statusSel.hasUrl()) {
    // single folder selected
    return true;
  }

  return false;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
