/*
 * ====================================================================
 * Copyright (c) 2002-2011 The RapidSVN Group.  All rights reserved.
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
#include "wx/intl.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/revision.hpp"
#include "svncpp/status_selection.hpp"

// app
#include "action_event.hpp"
#include "ids.hpp"
#include "log_action.hpp"
#include "log_data.hpp"

LogAction::LogAction(wxWindow * parent)
    : Action(parent, _("Log"), DONT_UPDATE)
{
}

bool
LogAction::Perform()
{
  svn::Client client(GetContext());

  svn::Path target = GetTarget();
  const svn::LogEntries * entries =
    client.log(target.c_str(), svn::Revision::START,
               svn::Revision::HEAD, true, false);

  LogData * data = new LogData(entries, target);
  ActionEvent::Post(GetParent(), TOKEN_LOG, data);

  return true;
}

bool
LogAction::CheckStatusSel(const svn::StatusSel & statusSel)
{
  if (1 != statusSel.size())
    return false;

  if (statusSel.hasUnversioned())
    return false;

  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
