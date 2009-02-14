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
#include "svncpp/pool.hpp"
#include "svncpp/status_selection.hpp"
#include "svncpp/targets.hpp"


// app
#include "commit_action.hpp"
#include "commit_dlg.hpp"
#include "ids.hpp"
#include "utils.hpp"

CommitAction::CommitAction(wxWindow * parent)
    : Action(parent, _("Commit"), 0)
{
}

bool
CommitAction::Prepare()
{
  if (!Action::Prepare())
    return false;

  svn::Client client (GetContext());
  const svn::StatusSel& statusSel = GetStatusSel();
  const svn::PathVector & selectedTargets = statusSel.targets();
  svn::PathVector targets;
  svn::PathVector::const_iterator it;
  bool recursive;

  // for local files we wanna filter out the unmodified files
  if (statusSel.hasUrl())
  {
    // but we can add all remote files immediately
    targets = selectedTargets;

    // recursive can be on
    recursive = true;
  }
  for (it = selectedTargets.begin(); it != selectedTargets.end(); it++)
  {
    svn::Path path = *it;

    svn::StatusFilter filter;
    filter.showModified = true;
    filter.showExternals = true;
    filter.showConflicted = true;

    // Get status array for parent and all entries within it
    svn::StatusEntries entries;
    client.status(path.c_str(), filter, true, false, entries);

    svn::StatusEntries::iterator it;
    for (it = entries.begin(); it != entries.end(); it++)
    {
      svn::Status& status = *it;

      targets.push_back(status.path());
    }

    // we dont need recursice since we added all the
    // files that are available
    recursive = false;
  }

  CommitDlg dlg(GetParent(), targets);
  dlg.SetRecursive(recursive);

  if (dlg.ShowModal() != wxID_OK)
    return false;

  m_recursive = dlg.GetRecursive();
  m_message = dlg.GetMessage();
  m_keepLocks = dlg.GetKeepLocks();
  m_files = dlg.GetSelectedFilenames();

  // are there any files left over to commit?
  if (m_files.size() == 0)
    return false;

  return true;
}

bool
CommitAction::Perform()
{
  svn::Client client(GetContext());

  std::string messageUtf8(LocalToUtf8(m_message));

  svn::Pool pool;

  svn_revnum_t revision = 
    client.commit(m_files, messageUtf8.c_str(),
                  m_recursive, m_keepLocks);

  wxString str;

  str = wxString::Format(wxT("%s %")
                         wxT(SVN_REVNUM_T_FMT)
                         wxT("."),
                         _("Committed revision"), revision);
  Trace(str);

  return true;
}

bool
CommitAction::CheckStatusSel(const svn::StatusSel & statusSel)
{
  if (0 == statusSel.size())
    return false;

  if (statusSel.hasUnversioned())
    return false;

  if (statusSel.hasUrl())
    return false;

  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
