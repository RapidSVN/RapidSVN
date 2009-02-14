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
#include "commit_log_dlg.hpp"
#include "hist_val.hpp"
#include "hist_entries.hpp"
#include "preferences.hpp"

enum
{
  ID_HISTORY_COMBO_BOX = wxID_HIGHEST
};

struct CommitLogDlg::Data
{
public:
  wxComboBox * comboHistory;
  wxTextCtrl * msg;

  wxString message;

  Data(wxWindow * window, wxSizer * msgSizer, wxSizer * histSizer)
  {
    // create controls
    wxSize msgSize(window->GetCharWidth() * 80,
                   window->GetCharHeight() * 10);

    {
      Preferences prefs;
      HistoryValidator val(HISTORY_COMMIT_LOG, &message, false,
                           prefs.useLastCommitMessage);
      msg = new wxTextCtrl(window, -1, wxEmptyString, wxDefaultPosition,
                           msgSize, wxTE_MULTILINE, val);
      msgSizer->Add(msg, 1, wxALL | wxEXPAND, 5);
    }

    {
      HistoryValidator val(HISTORY_COMMIT_LOG, 0, true);
      comboHistory = new wxComboBox(
        window, ID_HISTORY_COMBO_BOX, wxEmptyString,
        wxDefaultPosition, wxDefaultSize, 0, NULL,
        wxCB_READONLY, val);
      
      histSizer->Add(comboHistory, 1, wxALL | wxEXPAND, 5);
    }

  }
};


BEGIN_EVENT_TABLE(CommitLogDlg, wxDialog)
  EVT_COMBOBOX(ID_HISTORY_COMBO_BOX, CommitLogDlg::OnHistoryComboBox)
END_EVENT_TABLE()


CommitLogDlg::CommitLogDlg(wxWindow* parent)
  : CommitLogDlgBase(parent, -1, _("Commit Log Message"),
                     wxDefaultPosition, wxDefaultSize,
                     wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data(this, m_msgSizer, m_histSizer);

  SetAutoLayout(true);
  SetSizer(m_mainSizer);
  
  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  CentreOnParent();
}


CommitLogDlg::~CommitLogDlg()
{
  delete m;
}


const wxString &
CommitLogDlg::GetMessage() const
{
  return m->message;
}


void
CommitLogDlg::OnHistoryComboBox(wxCommandEvent &)
{
  // transfer from combobox to text control
  m->msg->SetValue(m->comboHistory->GetValue());
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
