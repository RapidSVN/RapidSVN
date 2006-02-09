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
#include "svncpp/status.hpp"

// app
#include "switch_action.hpp"
#include "update_dlg.hpp"
#include "utils.hpp"

SwitchAction::SwitchAction (wxWindow * parent)
 : Action (parent, _("Switch"), GetBaseFlags ())
{
}

bool
SwitchAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  // first try to get the URL for the target
  wxString url;
  {
    svn::Path path = GetTarget ();
    svn::Client client (GetContext ());
    svn::Status status (client.singleStatus (path.c_str ()));
    url = Utf8ToLocal (status.entry ().url ());
  }

  // create flags for the dialog
  int flags = UpdateDlg::WITH_URL;

  UpdateDlg dlg (GetParent (), _("Switch"), flags,
                 true);
  dlg.GetData ().url = url;

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  m_data = dlg.GetData ();

  return true;
}

bool
SwitchAction::Perform ()
{
  svn::Path path = GetTarget ();
  svn::Path urlUtf8 (PathUtf8 (m_data.url));
  svn::Revision revision (svn::Revision::HEAD);

  if (!m_data.useLatest)
  {
    svn_revnum_t revnum;
    TrimString (m_data.revision);
    m_data.revision.ToLong (&revnum);
    revision = revnum;
  }

  svn::Client client (GetContext ());
  client.doSwitch (path, urlUtf8.c_str (), revision,
                   m_data.recursive);

  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
