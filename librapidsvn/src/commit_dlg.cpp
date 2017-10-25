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
#include "wx/valgen.h"

// svncpp
#include "svncpp/path.hpp"

// app
#include "commit_dlg.hpp"
#include "hist_val.hpp"
#include "hist_entries.hpp"
#include "preferences.hpp"
#include "utils.hpp"
#include "diff_data.hpp"
#include "diff_action.hpp"
#include "action_event.hpp"
#include "ids.hpp"


CommitDlg::CommitDlg(wxWindow* parent, const svn::PathVector & filenames)
  : CommitDlgBase(parent, -1, _("Commit"), wxDefaultPosition, wxDefaultSize,
                  wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m_textMessage->SetMinSize(wxSize(GetCharWidth() * 60, GetCharHeight() * 8));
  m_textMessage->SetSize(GetCharWidth() * 80, GetCharHeight() * 10);

  Preferences prefs;
  HistoryValidator valMessage(HISTORY_COMMIT_LOG, &m_message, false,
                              prefs.useLastCommitMessage);
  m_textMessage->SetValidator(valMessage);

  HistoryValidator valHistory(HISTORY_COMMIT_LOG, 0, true);
  m_comboHistory->SetValidator(valHistory);

  m_usesFilenames = filenames.size() > 0;
  if (!m_usesFilenames)
    m_mainSizer->Show(m_filesSizer, false);
  else
  {
    m_checkListFiles->Clear();

    svn::PathVector::const_iterator it;

    for (it=filenames.begin(); it!=filenames.end(); it++)
    {
      const svn::Path & path = *it;
      int i = m_checkListFiles->Append(PathToNative(path));

      m_checkListFiles->Check(i, true);
    }

    CheckFilesButtons();
  }

  wxGenericValidator valRecursive(&m_recursive);
  m_checkRecursive->SetValidator(valRecursive);

  m_keepLocks = false;
  wxGenericValidator valKeepLocks(&m_keepLocks);
  m_checkKeepLocks->SetValidator(valKeepLocks);

  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  Layout();
  CentreOnParent();
}


CommitDlg::~CommitDlg()
{
}


bool
CommitDlg::TransferDataFromWindow()
{
  bool result = wxDialog::TransferDataFromWindow();

  m_selectedFilenames.clear();

  if (result && m_usesFilenames)
  {
    size_t count = m_checkListFiles->GetCount();
    for (size_t i=0; i < count; i++)
    {
      if (m_checkListFiles->IsChecked(i))
        m_selectedFilenames.push_back(PathUtf8(m_checkListFiles->GetString(i)));
    }
  }

  return result;
}


const wxString &
CommitDlg::GetMessage() const
{
  return m_message;
}


const svn::PathVector &
CommitDlg::GetSelectedFilenames() const
{
  return m_selectedFilenames;
}


bool
CommitDlg::GetRecursive() const
{
  return m_recursive;
}


void
CommitDlg::SetRecursive(bool recursive)
{
  m_recursive = recursive;
  m_checkRecursive->SetValue(recursive);
}


bool
CommitDlg::GetKeepLocks() const
{
  return m_keepLocks;
}


void
CommitDlg::OnComboHistory(wxCommandEvent &)
{
  // transfer from combobox to text control
  m_textMessage->SetValue(m_comboHistory->GetValue());
}

void
CommitDlg::OnCheckListFilesDClick(wxCommandEvent &event)
{
  OnButtonDiff(event);
}


void
CommitDlg::OnCheckListFiles(wxCommandEvent &)
{
  CheckFilesButtons();
}


void
CommitDlg::OnCheckListFilesToggle(wxCommandEvent & event)
{
  OnButtonToggle(event);
}


void
CommitDlg::OnButtonDiff(wxCommandEvent &)
{
  wxArrayInt selection;
  m_checkListFiles->GetSelections(selection);

  // right now we support only single selection
  wxASSERT(selection.size() <= 1);

  wxArrayInt::const_iterator it;

  for (it = selection.begin(); it != selection.end(); it++)
  {
    int item = *it;

    DiffData *data = new DiffData();
    data->path = m_checkListFiles->GetString((unsigned int)item);
    data->compareType = DiffData::WITH_BASE;

    ActionEvent::Post(GetParent(), TOKEN_DIFF, data);
  }
}


void
CommitDlg::OnButtonToggle(wxCommandEvent &)
{
  wxArrayInt selection;
  m_checkListFiles->GetSelections(selection);

  if (selection.size() < 1)
    return;

  // the first selection is taken to get the new value
  bool newValue = !m_checkListFiles->IsChecked(selection[0]);

  wxArrayInt::const_iterator it;

  for (it = selection.begin(); it != selection.end(); it++)
  {
    int item = *it;

    m_checkListFiles->Check(item, newValue);
  }
}


void
CommitDlg::CheckFilesButtons(void)
{
  wxArrayInt selection;
  m_checkListFiles->GetSelections(selection);

  m_buttonToggle->Enable(selection.size() > 0);
  m_buttonDiff->Enable(selection.size() == 1);
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
