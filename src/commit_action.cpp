/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
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
#include "svncpp/pool.hpp"
#include "svncpp/status_selection.hpp"

// app
#include "commit_action.hpp"
#include "commit_dlg.hpp"
#include "ids.hpp"
#include "utils.hpp"

CommitAction::CommitAction (wxWindow * parent)
  : Action (parent, _("Commit"), 0)
{
}

bool
CommitAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  CommitDlg dlg(GetParent ());

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  m_recursive = dlg.GetRecursive ();
  m_message = dlg.GetMessage ();
  m_keepLocks = dlg.GetKeepLocks ();

  return true;
}

bool
CommitAction::Perform ()
{
  svn::Client client (GetContext ());

  const svn::StatusSel & statusSel = GetStatusSel ();

  std::string messageUtf8 (LocalToUtf8 (m_message));

  svn::Pool pool;

  svn_revnum_t revision = 
    client.commit (statusSel.targets (), messageUtf8.c_str (),
                   m_recursive, m_keepLocks);

  wxString str;

  str = wxString::Format (wxT("%s %")
                          wxT(SVN_REVNUM_T_FMT)
                          wxT("."),
                          _("Committed revision"), revision);
  Trace (str);

  return true;
}

bool
CommitAction::CheckStatusSel (const svn::StatusSel & statusSel)
{
  if (0 == statusSel.size ())
    return false;

  if (statusSel.hasUnversioned ())
    return false;

  if (statusSel.hasUrl ())
    return false;

  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
