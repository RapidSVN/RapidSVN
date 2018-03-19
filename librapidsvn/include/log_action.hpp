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
 * along with this program (in the file GPL.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#ifndef _LOG_ACTION_H_INCLUDED_
#define _LOG_ACTION_H_INCLUDED_

// app
#include "action.hpp"

// forward declarations
namespace svn
{
  class StatusSel;
  class Client;
  class Path;
  class RepositoryPath;
  class LogEntry;
  typedef std::vector<LogEntry> LogEntries;
}
class LogDlg;

struct LogNextData
{
public:
  wxString path;
  svn::Revision startRevision;
  svn::Revision endRevision;
  LogDlg* logdlg;
  svn::LogEntries* logEntries;

  /** Constructor */
  LogNextData()
    : startRevision(svn::Revision::START),
      endRevision(svn::Revision::HEAD),
      logdlg(NULL),
      logEntries(NULL)
  {
  }
  /** Constructor */
  LogNextData(wxString path_, svn::Revision startRevision_ = svn::Revision::START,
              svn::Revision endRevision_ = svn::Revision::HEAD,
              LogDlg* logdlg_ = NULL, svn::LogEntries* logEntries_ = NULL)
    : path(path_),
      startRevision(startRevision_),
      endRevision(endRevision_),
      logdlg(logdlg_),
      logEntries(logEntries_)
  {
  }
};

class LogNextAction : public Action
{
public:
  /** Constructor */
  LogNextAction(wxWindow * parent, LogNextData & data);

  /** Desctructor */
  virtual ~LogNextAction();

  /**
   * @see Action
   */
  virtual bool
  Perform();

  static bool
  CheckStatusSel(const svn::StatusSel & statusSel);

private:
  struct Data;
  Data * m;
};

class LogAction : public Action
{
public:
  static int LogLimit;

public:
  LogAction(wxWindow * parent);

  /**
   * @see Action
   */
  virtual bool
  Perform();

  static bool
  CheckStatusSel(const svn::StatusSel & statusSel);

private:
  svn::RepositoryPath CreateRepositoryPath(svn::Client & client, svn::Path & path);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
