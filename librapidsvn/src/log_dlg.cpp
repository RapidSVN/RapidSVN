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

// stl
#include <algorithm>

// wx windows
#include "wx/wx.h"
#include "wx/valgen.h"
#include "wx/listctrl.h"
#include "wx/splitter.h"
#include "wx/notebook.h"


// svncpp
#include "svncpp/log_entry.hpp"

// app
#include "action_event.hpp"
#include "diff_data.hpp"
#include "get_data.hpp"
#include "ids.hpp"
#include "log_dlg.hpp"
#include "log_aff_list.hpp"
#include "log_rev_list.hpp"
#include "merge_dlg.hpp"
#include "utils.hpp"
#include "annotate_data.hpp"
#include "log_action.hpp"

struct LogDlg::Data
{
public:
  svn::LogEntries * entries;
  wxString path;
  svn::RepositoryPath repositoryPath;

public:
  Data(const svn::RepositoryPath & path_,
       svn::LogEntries * entries_)
    : entries(entries_), path(Utf8ToLocal(path_.c_str())), repositoryPath(path_)
  {
  }

};

LogDlg::LogDlg(wxWindow * parent,
               const svn::RepositoryPath & path,
               svn::LogEntries * entries)
  : LogDlgBase(parent, -1, _("Log History"), wxDefaultPosition,
               parent->GetSize(), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | wxMAXIMIZE_BOX)
{
  m = std::auto_ptr<Data>(new Data(path, entries));

  m_staticRevisions->SetLabel(
    wxString::Format(_("History: %zd revisions"), entries->size()));

  // Remove the last log entry (only included so we know if more are available)
  if (entries->size() > LogAction::LogLimit)
  {
    svn::LogEntry & lastLogEntry = entries->back();
    m_NextRevision = lastLogEntry.revision;
    entries->pop_back();
    m_buttonMore->Enable(true);
  }
  else
  {
    m_NextRevision = SVN_INVALID_REVNUM;
    m_buttonMore->Enable(false);
  }

  m_listRevisions->SetEntries(entries);

  CheckControls();

  SetAutoLayout(true);
  SetSizer(m_mainSizer);

  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  m_listFiles->Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(LogDlg::OnAffectedFileOrDirCommand), NULL, this);

  // Set size of this dialog to be 5% smaller than parent window.
  wxSize parentSize = parent->GetSize();
  parentSize.DecBy(parentSize.GetWidth() * 0.05);
  this->SetSize(parentSize);

  CentreOnParent();
}

LogDlg::~LogDlg()
{
  m_listFiles->Disconnect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(LogDlg::OnAffectedFileOrDirCommand), NULL, this);
}

void
LogDlg::NextLogEntriesCallback(svn::LogEntries* nextLogEntries)
{
  AddLogEntries(nextLogEntries);
}

void
LogDlg::AddLogEntries(svn::LogEntries* logEntries)
{
  // Remove the last log entry (only included so we know if more are available)
  m_buttonMore->Enable(false);
  if (logEntries->size() > LogAction::LogLimit)
  {
    svn::LogEntry & lastLogEntry = logEntries->back();
    m_NextRevision = lastLogEntry.revision;
    logEntries->pop_back();
    m_buttonMore->Enable(true);
  }
  else
  {
    m_NextRevision = SVN_INVALID_REVNUM;
  }

  svn::LogEntries::iterator iter;
  for (iter = logEntries->begin(); iter != logEntries->end(); ++iter)
  {
    const svn::LogEntry & entry = *iter;
    m->entries->push_back(entry);
  }
  m_listRevisions->AddEntriesToList(logEntries);
}

void
LogDlg::OnGet(wxCommandEvent & WXUNUSED(event))
{
  svn_revnum_t revnum = m_listRevisions->GetSelectedRevision();

  GetData * data = new GetData();
  data->revision = revnum;
  data->path = m->path;

  ActionEvent::Post(GetParent(), TOKEN_GET, data);
}

void
LogDlg::OnView(wxCommandEvent & WXUNUSED(event))
{
  OnView(m->path);
}

void
LogDlg::OnView(wxString & path)
{
  svn_revnum_t revnum = m_listRevisions->GetSelectedRevision();

  GetData * data = new GetData();
  data->revision = revnum;
  data->path = path;

  ActionEvent::Post(GetParent(), TOKEN_VIEW, data);
}

void
LogDlg::OnDiff(wxCommandEvent & WXUNUSED(event))
{
  OnDiff(m->path);
}

void
LogDlg::OnDiff(wxString & path, bool singleItemDiff)
{
  RevnumArray array(m_listRevisions->GetSelectedRevisions());

  wxASSERT(array.Count() >= 1);
  wxASSERT(array.Count() <= 2);

  DiffData * data = new DiffData();
  data->path = path;

  if (2 == array.Count())
  {
    data->compareType = DiffData::TWO_REVISIONS;
    data->revision1 = svn::Revision(array[0]);
    data->revision2 = svn::Revision(array[1]);
  }
  else if (m->repositoryPath.isUrl() || singleItemDiff)
  {
    data->compareType = DiffData::TWO_REVISIONS;
    data->revision1 = svn::Revision(array[0]);
    data->revision2 = svn::Revision(array[0] - 1);
  }
  else
  {
    svn_revnum_t revnumPrior = m_listRevisions->GetPriorRevision(array[0]);
    if (revnumPrior != -1)
    {
      data = new DiffData();
      data->path = path;
      data->compareType = DiffData::TWO_REVISIONS;
      data->revision1 = svn::Revision(array[0]);
      data->revision2 = svn::Revision(revnumPrior);
    }
  }

  ActionEvent::Post(GetParent(), TOKEN_DIFF, data);
}

void
LogDlg::OnMerge(wxCommandEvent & WXUNUSED(event))
{
  RevnumArray array(m_listRevisions->GetSelectedRevisions());

  wxASSERT(2 == array.Count());

  MergeData * data = new MergeData();
  data->Path1 = data->Path2 = m->path;
  if (array[0] > array[1])
  {
    data->Path1Rev << array[1];
    data->Path2Rev << array[0];
  }
  else
  {
    data->Path1Rev << array[0];
    data->Path2Rev << array[1];
  }

  ActionEvent::Post(GetParent(), TOKEN_MERGE, data);
}

void
LogDlg::OnAnnotate(wxCommandEvent & WXUNUSED(event))
{
  OnAnnotate(m->path);
}

void
LogDlg::OnAnnotate(wxString & path)
{
  RevnumArray array(m_listRevisions->GetSelectedRevisions());

  wxASSERT(1 == array.Count());

  AnnotateData * data = new AnnotateData();
  data->path = path;
  data->endRevision = svn::Revision(array[0]);

  ActionEvent::Post(GetParent(), TOKEN_ANNOTATE, data);
}

void
LogDlg::OnMore(wxCommandEvent & WXUNUSED(event))
{
  OnMore(m->path);
}

void
LogDlg::OnMore(wxString & path)
{
  if (m_NextRevision == SVN_INVALID_REVNUM)
    return;

  m_buttonMore->Enable(false);

  LogNextData * data = new LogNextData(path, svn::Revision(m_NextRevision), svn::Revision::HEAD, (LogDlg*)this);

  ActionEvent::Post(GetParent(), TOKEN_LOG_NEXT, data);
}

void
LogDlg::OnLog(wxString & path)
{
}

void
LogDlg::OnAffectedFileOrDirRightClick(wxListEvent & event)
{
  long focusedIdx = m_listFiles->GetFocusedItem();

  if (focusedIdx == -1)
  {
    return;
  }

  wxMenu menu;
  AppendLogItemQueryMenu(&menu);

  // Disable non-applicable menu items
  wxListItem listItem;
  listItem.SetColumn(0);
  listItem.SetMask(wxLIST_MASK_DATA | wxLIST_MASK_TEXT);
  listItem.SetId(focusedIdx);
  if (!m_listFiles->GetItem(listItem))
    return;

  long originalAffectedFilesIndex = listItem.m_data;
  if (originalAffectedFilesIndex < 0)
    return;

  wxString svnStatusAdded = _("A");
  wxString svnStatusDeleted = _("D");

  if (listItem.GetText() == svnStatusAdded)
  {
    wxMenuItem* pMenuItem = menu.FindItem(ID_Diff);
    if (pMenuItem != NULL)
      pMenuItem->Enable(false);
    pMenuItem = menu.FindItem(ID_Annotate);
    if (pMenuItem != NULL)
      pMenuItem->Enable(false);
  }
  else if (listItem.GetText() == svnStatusDeleted)
  {
    wxMenuItem* pMenuItem = menu.FindItem(ID_Diff);
    if (pMenuItem != NULL)
      pMenuItem->Enable(false);
    pMenuItem = menu.FindItem(ID_Annotate);
    if (pMenuItem != NULL)
      pMenuItem->Enable(false);
    pMenuItem = menu.FindItem(ID_Edit);
    if (pMenuItem != NULL)
      pMenuItem->Enable(false);
  }
  // The Log menu item is never applicable within the Log Dialog
  menu.Remove(ID_Log);

  m_listFiles->PopupMenu(&menu, event.GetPoint());
}

void
LogDlg::OnAffectedFileOrDirCommand(wxCommandEvent & event)
{
  long focusedFileIdx = m_listFiles->GetFocusedItem();

  wxASSERT(focusedFileIdx != -1);

  wxListItem listItem;
  listItem.SetColumn(0);
  listItem.SetMask(wxLIST_MASK_DATA | wxLIST_MASK_TEXT);
  listItem.SetId(focusedFileIdx);
  if (!m_listFiles->GetItem(listItem))
    return;

  long originalAffectedFilesIndex = listItem.m_data;
  if (originalAffectedFilesIndex < 0)
    return;


  std::list<svn::LogChangePathEntry>::const_iterator it = affectedFiles.begin();
  std::advance(it, originalAffectedFilesIndex);
  wxString file(Utf8ToLocal(m->repositoryPath.getRepositoryRoot() + it->path));

  wxString svnStatusAdded = _("A");
  wxString svnStatusDeleted = _("D");

  int id = event.GetId();
  switch (id)
  {
  case ID_Diff:
    if (listItem.GetText() != svnStatusAdded  && listItem.GetText() != svnStatusDeleted)
      OnDiff(file, true);
    break;
  case ID_Edit:
    if (listItem.GetText() != svnStatusDeleted)
      OnView(file);
    break;
  case ID_Log:
    if (listItem.GetText() != svnStatusAdded  && listItem.GetText() != svnStatusDeleted)
      OnLog(file);
    break;
  case ID_Annotate:
    if (listItem.GetText() != svnStatusDeleted)
      OnAnnotate(file);
    break;
  }
}


void
LogDlg::UpdateSelection()
{
  int count = m_listRevisions->GetSelectedItemCount();

  if (1 != count)
  {
    m_textLog->Clear();
    ReduceSelectionToOnlyTwoItems();
  }
  else
  {
    long itemIndex = m_listRevisions->GetFirstSelected();
    if (itemIndex < 0 || itemIndex >= m->entries->size())
    {
      return;
    }
    const svn::LogEntry & entry = (*m->entries)[itemIndex];

    wxString message(Utf8ToLocal(entry.message.c_str()));
    message.Trim();

    m_textLog->SetValue(message);
  }
  FillAffectedFiles();
  CheckControls();
}

void
LogDlg::OnRevSelected(wxListEvent & WXUNUSED(event))
{
  UpdateSelection();
}


void
LogDlg::OnRevDeselected(wxListEvent & WXUNUSED(event))
{
  UpdateSelection();
}


void
LogDlg::CheckControls()
{
  svn::Path pathUtf8(PathUtf8(m->path));
  bool isUrl = pathUtf8.isUrl();
  int count = m_listRevisions->GetSelectedItemCount();

  bool twoRevs = 2 == count;
  bool oneRev = 1 == count;

  m_buttonGet->Enable(oneRev && (!isUrl));
  m_buttonView->Enable(oneRev);
  m_buttonDiff ->Enable(oneRev || twoRevs);
  m_buttonMerge->Enable(twoRevs);
  m_buttonAnnotate->Enable(oneRev);
}

void
LogDlg::ReduceSelectionToOnlyTwoItems()
{
  if (m_listRevisions->GetSelectedItemCount() <= 2)
  {
    return;
  }

  long previousSelectedItemIndex = m_listRevisions->GetNextSelected(m_listRevisions->GetFirstSelected()),
       selectedItemIndex = m_listRevisions->GetNextSelected(previousSelectedItemIndex);
  bool focusedWasSelected = false;

  while (selectedItemIndex != -1)
  {
    if (previousSelectedItemIndex == m_listRevisions->GetFocusedItem())
    {
      focusedWasSelected = true;
    }
    else
    {
      m_listRevisions->Select(previousSelectedItemIndex, false);
    }
    previousSelectedItemIndex = selectedItemIndex;
    selectedItemIndex = m_listRevisions->GetNextSelected(previousSelectedItemIndex);
  }

  if (focusedWasSelected)
  {
    m_listRevisions->Select(previousSelectedItemIndex, false);
  }
}

void
LogDlg::FillAffectedFiles()
{
  m_listFiles->DeleteAllItems();
  affectedFiles.clear();

  long firstSelectedItemIndex = m_listRevisions->GetFirstSelected();
  if (firstSelectedItemIndex == -1)
  {
    return;
  }

  const svn::LogEntry & entry = (*m->entries)[firstSelectedItemIndex];
  affectedFiles = entry.changedPaths;

  if (m_listRevisions->GetSelectedItemCount() > 1)
  {
    std::set<std::string> affectedFilesIntersection = GetIntersectionOfAffectedPaths();
    affectedFiles = FilterAffectedPaths(affectedFiles, affectedFilesIntersection);
  }

  m_listFiles->SetValue(affectedFiles);
}

std::set<std::string>
LogDlg::GetIntersectionOfAffectedPaths()
{
  long firstSelectedItemIndex = m_listRevisions->GetFirstSelected();
  std::set<std::string> affectedPaths = GetAffectedPathsForItem(firstSelectedItemIndex);
  long nextSelectedItemIndex = m_listRevisions->GetNextSelected(firstSelectedItemIndex);

  while (nextSelectedItemIndex != -1)
  {
    std::set<std::string> nextAffectedPaths = GetAffectedPathsForItem(nextSelectedItemIndex);
    std::vector<std::string> intersection(affectedPaths.size());

    set_intersection(affectedPaths.begin(), affectedPaths.end(),
                     nextAffectedPaths.begin(), nextAffectedPaths.end(),
                     intersection.begin());
    affectedPaths = std::set<std::string>(intersection.begin(), intersection.end());

    nextSelectedItemIndex = m_listRevisions->GetNextSelected(nextSelectedItemIndex);
  }

  return affectedPaths;
}

std::list<svn::LogChangePathEntry>
LogDlg::FilterAffectedPaths(std::list<svn::LogChangePathEntry> const & changedPaths, std::set<std::string> const & filter)
{
  std::list<svn::LogChangePathEntry> result;

  for (std::list<svn::LogChangePathEntry>::const_iterator it = changedPaths.begin();
       it != changedPaths.end();
       ++it)
  {
    if (filter.find(it->path) != filter.end())
    {
      result.push_back(*it);
    }
  }

  return result;
}

std::set<std::string>
LogDlg::GetAffectedPathsForItem(long itemIndex)
{
  if (itemIndex < 0)
  {
    return std::set<std::string>();
  }

  std::set<std::string> result;
  const svn::LogEntry & entry = (*m->entries)[itemIndex];

  for (std::list<svn::LogChangePathEntry>::const_iterator it = entry.changedPaths.begin();
       it != entry.changedPaths.end();
       ++it)
  {
    result.insert(it->path);
  }

  return result;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
