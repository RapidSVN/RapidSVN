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
#include "svncpp/targets.hpp"

// app
#include "action_event.hpp"
#include "checkout_action.hpp"
#include "checkout_dlg.hpp"
#include "ids.hpp"
#include "utils.hpp"

CheckoutAction::CheckoutAction(wxWindow * parent)
  : Action(parent, _("Checkout"), 0)
{
}

bool
CheckoutAction::Prepare()
{
  if (!Action::Prepare())
  {
    return false;
  }

  const svn::PathVector & v = GetTargets();

  svn::Path selectedUrl("");

  if (v.size() == 1)
  {
    svn::Path selectedPath(v [0]);

    if (selectedPath.isUrl())
    {
      selectedUrl = selectedPath;
    }
  }
  else if (v.size() == 0)
  {
    svn::Path selectedPath(GetPath());
    if (selectedPath.isUrl())
    {
      selectedUrl = selectedPath;
    }
  }

  CheckoutDlg dlg(GetParent(), selectedUrl);

  if (dlg.ShowModal() != wxID_OK)
  {
    return false;
  }

  m_data = dlg.GetData();
  return true;
}

bool
CheckoutAction::Perform()
{
  svn::Client client(GetContext());

  TrimString(m_data.DestFolder);
  wxString dest_folder(m_data.DestFolder);
  UnixPath(dest_folder);
  TrimString(m_data.RepUrl);

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

  wxSetWorkingDirectory(m_data.DestFolder);

  svn::Path repUrlUtf8(PathUtf8(m_data.RepUrl));
  svn::Path destFolderUtf8(PathUtf8(dest_folder));

  bool ignoreExternals = m_data.IgnoreExternals;

  client.checkout(repUrlUtf8.c_str(),
                  destFolderUtf8,
                  revision,
                  m_data.Recursive,
                  ignoreExternals,
                  pegRevision);

  // now post event to add bookmark to bookmarks
  if (m_data.Bookmarks)
  {
    ActionEvent::Post(GetParent(), TOKEN_ADD_BOOKMARK, m_data.DestFolder);
  }

  return true;
}

bool
CheckoutAction::CheckStatusSel(const svn::StatusSel & WXUNUSED(statusSel))
{
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
