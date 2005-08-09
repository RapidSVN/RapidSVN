/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
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
#include "wx/intl.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/revision.hpp"

// app
#include "log_action.hpp"
#include "log_dlg.hpp"

LogAction::LogAction (wxWindow * parent)
  : Action (parent, _("Log"), GetBaseFlags ())
{
}

bool
LogAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  svn::Client client (GetContext ());

  const svn::LogEntries * entries;
  svn::Path target = GetTarget ();
  {
    wxBusyCursor cursor;
    entries = client.log (target.c_str (), svn::Revision::START,
                          svn::Revision::HEAD, false, false);
  }
  {
    LogDlg dlg (GetParent (), target.c_str (), entries);
    dlg.ShowModal ();
  }
  delete entries;

  return false;
}

bool
LogAction::Perform ()
{
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
