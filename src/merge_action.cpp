/*
 * ====================================================================
 * Copyright (c) 2002-2010 The RapidSVN Group.  All rights reserved.
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
#include "wx/filename.h"

// svncpp
#include "svncpp/client.hpp"

// app
#include "merge_action.hpp"
#include "merge_dlg.hpp"
#include "utils.hpp"

MergeAction::MergeAction(wxWindow * parent)
    : Action(parent, _("Merge"))
{
  m_data.calledByLogDlg = false;
}

MergeAction::MergeAction(wxWindow * parent, MergeData & data)
    : Action(parent, _("Merge")), m_data(data)
{
  m_data.calledByLogDlg = true;
}

bool
MergeAction::Prepare()
{
  if (!Action::Prepare())
  {
    return false;
  }

  // Set default Destination to the selected path in the folder browser
  // or, if called by the log dialogue, to the source path
  if (m_data.calledByLogDlg)
    m_data.Destination = m_data.Path1;
  else
    m_data.Destination = Utf8ToLocal(GetPath().c_str());

  MergeDlg dlg(GetParent(), m_data.calledByLogDlg, m_data);

  if (dlg.ShowModal() != wxID_OK)
  {
    return false;
  }

  return true;
}

bool
MergeAction::Perform()
{
  svn::Client client(GetContext());

  // Set current working directory to the path where the
  // merge will be performed. If Destination is a file and not a
  // directory, only the directory part should be used
  wxFileName path(m_data.Destination);
  if (!wxSetWorkingDirectory(path.GetPath(wxPATH_GET_VOLUME)))
  {
    wxString msg;
    msg.Printf(_("Could not set working directory to %s"),
               path.GetPath(wxPATH_GET_VOLUME).c_str());
    TraceError(msg);
    return false;
  }

  svn_revnum_t rev1 = 0;
  svn_revnum_t rev2 = 0;

  if (!m_data.Path1Rev.ToLong(&rev1) || !m_data.Path2Rev.ToLong(&rev2))
  {
    wxString msg = _("Invalid revision number detected");
    TraceError(msg);
    return false;
  }

  svn::Path path1Utf8(PathUtf8(m_data.Path1));
  svn::Path path2Utf8(PathUtf8(m_data.Path2));
  svn::Path destinationUtf8(PathUtf8(m_data.Destination));
  client.merge(path1Utf8,
               rev1,
               path2Utf8,
               rev2,
               destinationUtf8,
               m_data.Force,
               m_data.Recursive);
  return true;
}

bool
MergeAction::CheckStatusSel(const svn::StatusSel & /*statusSel*/)
{
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
