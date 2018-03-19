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

#ifndef _LOG_DLG_H_INCLUDED_
#define _LOG_DLG_H_INCLUDED_

// stl
#include <set>
#include <memory>

// wxWidgets
#include "wx/dialog.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/repository_path.hpp"

// app
#include "rapidsvn_generated.h"

class LogDlg : public LogDlgBase
{
public:
  /**
   * constructor. the @a entries are NOT owned by
   * this class.
   *
   * @param parent parent window
   * @param path path of selected item
   * @param entries log entries
   */
  LogDlg(wxWindow * parent,
         const svn::RepositoryPath & path,
         svn::LogEntries * entries);

  /**
   * destructor
   */
  virtual ~LogDlg();

  void
  NextLogEntriesCallback(svn::LogEntries* nextLogEntries);

  void
  AddLogEntries(svn::LogEntries* logEntries);


protected:  // events inherited from LogDlgBase
  void
  OnGet(wxCommandEvent & event);

  void
  OnView(wxCommandEvent & event);

  void
  OnDiff(wxCommandEvent & event);

  void
  OnMerge(wxCommandEvent & event);

  void
  OnAnnotate(wxCommandEvent & event);

  void
  OnMore(wxCommandEvent & event);

  void
  OnRevSelected(wxListEvent & event);

  void
  OnRevDeselected(wxListEvent & event);

  void
  OnAffectedFileOrDirRightClick(wxListEvent & event);

private:
  /** hide implementation details */
  struct Data;
  std::auto_ptr<Data> m;

  std::list<svn::LogChangePathEntry> affectedFiles;
  svn_revnum_t m_NextRevision;

  void OnAffectedFileOrDirCommand(wxCommandEvent & event);

  void OnView(wxString & path);
  void OnDiff(wxString & path, bool singleItemDiff = false);
  void OnAnnotate(wxString & path);
  void OnMore(wxString & path);
  void OnLog(wxString & path);

  void CheckControls();
  void UpdateSelection();
  void ReduceSelectionToOnlyTwoItems();
  void FillAffectedFiles();
  std::set<std::string> GetIntersectionOfAffectedPaths();
  std::list<svn::LogChangePathEntry> FilterAffectedPaths(std::list<svn::LogChangePathEntry> const & changedPaths, std::set<std::string> const & filter);
  std::set<std::string> GetAffectedPathsForItem(long itemIndex);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
