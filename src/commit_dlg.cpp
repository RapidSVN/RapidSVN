/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
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

static const int ID_HISTORY_COMBO_BOX = 1;

struct CommitDlg::Data
{
public:
  bool recursive;
  bool keepLocks;
  wxComboBox * comboHistory;
  wxTextCtrl * msg;
  wxCheckListBox * files;
  wxCheckBox * checkRecursive;

  wxString message;
  svn::PathVector filenames;

  Data(wxWindow * window, bool unexpectedCommit, 
       const svn::PathVector & filenames)
    : recursive(true), comboHistory(0), msg(0), files(0),
      checkRecursive(0)
  {
    // create controls
    wxStaticBox* msgBox =
      new wxStaticBox(window, -1, unexpectedCommit ? _("This action has resulted in a Commit - please enter a log message") : _("Enter log message"));

    if (unexpectedCommit)
      keepLocks = true;
    else
      keepLocks = false;

    wxSize msgSize(window->GetCharWidth() * 80,
                   window->GetCharHeight() * 10);

    {
      Preferences prefs;
      HistoryValidator val(HISTORY_COMMIT_LOG, &message, false,
                           prefs.useLastCommitMessage);
      msg = new wxTextCtrl(window, -1, wxEmptyString, wxDefaultPosition,
                           msgSize, wxTE_MULTILINE, val);
    }

    if (filenames.size() > 0)
    {
      files = new wxCheckListBox(window, -1, wxDefaultPosition, wxDefaultSize, 0, wxLB_EXTENDED);
      svn::PathVector::const_iterator it;

      for(it=filenames.begin(); it!=filenames.end(); it++)
      {
        const svn::Path & path = *it;
        int i = files->Append(PathToNative(path));
        
        files->Check(i, true);
      }
    }

    wxStaticText * labelHistory = new wxStaticText(
      window, -1, _("Recent entries:"), wxDefaultPosition);

    {
      HistoryValidator val(HISTORY_COMMIT_LOG, 0, true);
      comboHistory = new wxComboBox(
        window, ID_HISTORY_COMBO_BOX, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, 0, NULL,
        wxCB_READONLY, val);
    }

    if (!unexpectedCommit)
    {
      wxGenericValidator val(&recursive);
      checkRecursive =
        new wxCheckBox(window, -1, _("Recursive"),
                       wxDefaultPosition, wxDefaultSize, 0,
                       val);
    }
    wxCheckBox * checkKeepLocks = NULL;
    if (!unexpectedCommit)
    {
      wxGenericValidator val(&keepLocks);
      checkKeepLocks =
        new wxCheckBox(window, -1, _("Keep locks"),
                       wxDefaultPosition, wxDefaultSize, 0,
                       val);
    }

    wxButton* ok =
      new wxButton(window, wxID_OK, _("OK"));
    wxButton* cancel =
      new wxButton(window, wxID_CANCEL, _("Cancel"));

    // position controls
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // The message field:
    wxStaticBoxSizer *msgSizer =
      new wxStaticBoxSizer(msgBox, wxHORIZONTAL);
    msgSizer->Add(msg, 1, wxALL | wxEXPAND, 5);

    // the history combo
    wxBoxSizer * histSizer = new wxBoxSizer(wxHORIZONTAL);
    histSizer->Add(labelHistory, 0, wxALL, 5);
    histSizer->Add(comboHistory, 1, wxALL | wxEXPAND, 5);

    // The buttons:
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    if (!unexpectedCommit)
    {
      buttonSizer->Add(checkRecursive, 1,
                       wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT,
                       10);
    }
    if (!unexpectedCommit)
    {
      buttonSizer->Add(checkKeepLocks, 1,
                       wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT,
                       10);
    }
    buttonSizer->Add(ok, 0, wxALL, 10);
    buttonSizer->Add(cancel, 0, wxALL, 10);

    mainSizer->Add(msgSizer, 1, wxALL | wxEXPAND, 5);
    mainSizer->Add(histSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);

    if (files != 0)
    {
      wxStaticBox* filesBox =
        new wxStaticBox(window, -1, _("Files To Commit"));

      // The files field:
      wxStaticBoxSizer *filesSizer =
        new wxStaticBoxSizer (filesBox, wxHORIZONTAL);

      filesSizer->Add (files, 1, wxALL | wxEXPAND, 5);
      mainSizer->Add(filesSizer, 1, wxALL | wxEXPAND, 5);
    }

    // Add all the sizers to the main sizer
    mainSizer->Add(buttonSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);

    window->SetAutoLayout(true);
    window->SetSizer(mainSizer);

    mainSizer->SetSizeHints(window);
    mainSizer->Fit(window);

    ok->SetDefault();
  }
};

bool CommitDlg::TransferDataFromWindow()
{
  m->message = m->msg->GetValue();

  if (m->files != 0)
  {
    for(size_t i=0; i<m->files->GetCount(); i++)
    {
      if(m->files->IsChecked(i))
        m->filenames.push_back(PathUtf8(m->files->GetString(i)));
    }
  }

  return true;
}


BEGIN_EVENT_TABLE(CommitDlg, wxDialog)
  EVT_COMBOBOX(ID_HISTORY_COMBO_BOX, CommitDlg::OnHistoryComboBox)
END_EVENT_TABLE()

CommitDlg::CommitDlg(wxWindow* parent, bool unexpectedCommit, 
                     const svn::PathVector & filenames)
    : wxDialog(parent, -1, unexpectedCommit ? _("Commit Log Message") : _("Commit"),
               wxDefaultPosition, wxDefaultSize,
               wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data(this, unexpectedCommit, filenames);
  CentreOnParent();
}

CommitDlg::~CommitDlg()
{
  delete m;
}

const wxString &
CommitDlg::GetMessage() const
{
  return m->message;
}

const svn::PathVector &
CommitDlg::GetSelectedFilenames() const
{
  return m->filenames;
}

bool
CommitDlg::GetRecursive() const
{
  return m->recursive;
}

void
CommitDlg::SetRecursive(bool recursive)
{
  m->recursive = recursive;
  m->checkRecursive->SetValue(recursive);
}

bool
CommitDlg::GetKeepLocks() const
{
  return m->keepLocks;
}

void
CommitDlg::OnHistoryComboBox(wxCommandEvent &)
{
  // transfer from combobox to text control
  m->msg->SetValue(m->comboHistory->GetValue());
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
