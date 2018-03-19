/*
 * ====================================================================
 * Copyright (c) 2002-2018 The RapidSVN Group.  All rights reserved.
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

#include <algorithm>

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/info.hpp"
#include "svncpp/repository_path.hpp"
#include "svncpp/revision.hpp"
#include "svncpp/status_selection.hpp"

// app
#include "action_event.hpp"
#include "utils.hpp"
#include "ids.hpp"
#include "log_action.hpp"
#include "log_data.hpp"


/* static */ int LogAction::LogLimit = 30;

struct LogNextAction::Data
{
private:
  Action * action;

public:
  LogNextData data;
  wxWindow * parent;

  Data(Action * action_)
    : action(action_)
  {
  }

  Data(Action * action_, LogNextData & data_)
    : action(action_), data(data_)
  {
  }

  svn::Context *
  GetContext()
  {
    return action->GetContext();
  }

  void
  GetLogNext(svn::LogEntries& logEntriesNext)
  {
  }
};


LogNextAction::LogNextAction(wxWindow * parent,
                             LogNextData & data)
  : Action(parent, _("Log"), UPDATE_LATER)
{
  m = new Data(this, data);
  m->parent = parent;
}

LogNextAction::~LogNextAction()
{
  delete m;
}

bool
LogNextAction::Perform()
{
  // wxBusyCursor busyCursor;

  svn::Client client(GetContext());

  svn::Path path = PathUtf8(m->data.path);
  svn::Path target = GetTarget();

  svn::LogEntries * entries = const_cast<svn::LogEntries *>(client.log(target.c_str(), m->data.startRevision,
                              svn::Revision::START, LogAction::LogLimit + 1, true, false));
  std::reverse(entries->begin(), entries->end());

  LogNextData * data = new LogNextData(m->data.path, m->data.startRevision,
                                       svn::Revision::HEAD, m->data.logdlg,
                                       (svn::LogEntries *) entries);
  ActionEvent::Post(GetParent(), TOKEN_LOG_NEXT_CALLBACK, data);

  return true;
}

bool
LogNextAction::CheckStatusSel(const svn::StatusSel & statusSel)
{
  if (1 != statusSel.size())
    return false;

  if (statusSel.hasUnversioned())
    return false;

  return true;
}

// ===============================================================================================

LogAction::LogAction(wxWindow * parent)
  : Action(parent, _("Log"), DONT_UPDATE)
{
}

bool
LogAction::Perform()
{
  svn::Client client(GetContext());

  svn::Path target = GetTarget();
  svn::LogEntries * entries = const_cast<svn::LogEntries *>(client.log(target.c_str(), svn::Revision::HEAD,
                              svn::Revision::START, LogLimit + 1, true, false));
  std::reverse(entries->begin(), entries->end());

  LogData * data = new LogData(entries, CreateRepositoryPath(client, target));
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

svn::RepositoryPath
LogAction::CreateRepositoryPath(svn::Client & client, svn::Path & path)
{
  svn::InfoVector info = client.info(path, svn::Revision::START, svn::Revision::HEAD);
  std::string repositoryRoot = !info.empty() ? info.back().repos() : "";
  return svn::RepositoryPath(path.path(), repositoryRoot);
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
