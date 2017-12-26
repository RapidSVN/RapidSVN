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
#include "wx/app.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/targets.hpp"

// app
#include "export_action.hpp"
#include "export_dlg.hpp"
#include "ids.hpp"
#include "utils.hpp"

ExportAction::ExportAction(wxWindow * parent)
  :Action(parent, _("Export"), DONT_UPDATE)
{
}

bool
ExportAction::Prepare()
{
  if (!Action::Prepare())
  {
    return false;
  }

  const svn::PathVector & v = GetTargets();

  svn::Path selectedPath("");

  if (v.size() == 1)
  {
    selectedPath = v[0];
  }
  else if (v.size() == 0)
  {
    selectedPath = GetPath();
  }


  ExportDlg dlg(GetParent(), selectedPath);

  if (dlg.ShowModal() != wxID_OK)
  {
    return false;
  }

  m_data = dlg.GetData();
  return true;
}

bool
ExportAction::Perform()
{
  svn::Client client(GetContext());

  TrimString(m_data.DestPath);
  UnixPath(m_data.DestPath);
  TrimString(m_data.SrcPath);

  long revnum = -1;
  svn::Revision revision(svn::Revision::HEAD);
  svn::Revision pegRevision;

  // Did the user request a specific revision?:
  if (!m_data.UseLatest)
  {
    TrimString(m_data.Revision);
    if (!m_data.Revision.IsEmpty())
    {
      m_data.Revision.ToLong(&revnum, 10);  // If this fails, revnum is unchanged.
    }
    revision = svn::Revision(revnum);
  }

  revnum = -1;
  // Did the user request a specific peg revision?:
  if (!m_data.NotSpecified)
  {
    TrimString(m_data.PegRevision);
    if (!m_data.PegRevision.IsEmpty())
    {
      m_data.PegRevision.ToLong(&revnum, 10);  // If this fails, revnum is unchanged.
    }
    if (revnum != -1)
      pegRevision = svn::Revision(revnum);
  }

  if (!m_data.DestPath.empty())
    wxSetWorkingDirectory(m_data.DestPath);

  svn::Path srcPathUtf8(PathUtf8(m_data.SrcPath));
  svn::Path destPathUtf8(PathUtf8(m_data.DestPath));

  client.doExport(srcPathUtf8.c_str(),
                  destPathUtf8,
                  revision,
                  m_data.Overwrite,
                  pegRevision,
                  m_data.IgnoreExternals,
                  m_data.Recursive,
                  m_data.Eol);
  return true;
}

bool
ExportAction::CheckStatusSel(const svn::StatusSel & WXUNUSED(statusSel))
{
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
