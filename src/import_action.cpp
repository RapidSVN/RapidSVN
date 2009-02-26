/*
 * ====================================================================
 * Copyright (c) 2002-2009 The RapidSvn Group.  All rights reserved.
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
#include "svncpp/targets.hpp"

// app
#include "import_dlg.hpp"
#include "import_action.hpp"
#include "utils.hpp"

ImportAction::ImportAction(wxWindow * parent)
    :Action(parent, _("Import"))
{
}

bool
ImportAction::Prepare()
{
  if (!Action::Prepare())
  {
    return false;
  }

  const svn::PathVector & v = GetTargets();

  svn::Path selectedPath("");

  if (v.size() == 1)
  {
    selectedPath = v [0];
  }

  ImportDlg dlg(GetParent(), selectedPath);

  if (dlg.ShowModal() != wxID_OK)
  {
    return false;
  }

  m_data = dlg.GetData();
  return true;
}

bool
ImportAction::Perform()
{
  svn::Client client(GetContext());

  svn::Path pathUtf8(PathUtf8(m_data.Path));
  svn::Path repositoryUtf8(PathUtf8(m_data.Repository));
  std::string LogMessageUtf8(LocalToUtf8(m_data.LogMessage));

  client.import(pathUtf8, repositoryUtf8.c_str(),
                LogMessageUtf8.c_str(), m_data.Recursive);

  return true;
}

bool
ImportAction::CheckStatusSel(const svn::StatusSel & WXUNUSED(statusSel))
{
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
