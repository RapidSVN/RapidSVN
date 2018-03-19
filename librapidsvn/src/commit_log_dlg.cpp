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


CommitLogDlg::CommitLogDlg(wxWindow* parent)
  : CommitLogDlgBase(parent, -1, _("Commit Log Message"),
                     wxDefaultPosition, wxDefaultSize,
                     wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m_textMessage->SetMinSize(wxSize(GetCharWidth() * 60, GetCharHeight() * 8));
  m_textMessage->SetSize(GetCharWidth() * 80, GetCharHeight() * 10);

  Preferences prefs;
  HistoryValidator histVal(HISTORY_COMMIT_LOG, &m_message, false,
                           prefs.useLastCommitMessage);
  m_textMessage->SetValidator(histVal);

  HistoryValidator valHistory(HISTORY_COMMIT_LOG, 0, true);
  m_comboHistory->SetValidator(valHistory);

  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  Layout();
  CentreOnParent();
}


CommitLogDlg::~CommitLogDlg()
{
}


const wxString &
CommitLogDlg::GetMessage() const
{
  return m_message;
}


void
CommitLogDlg::OnComboHistory(wxCommandEvent &)
{
  // transfer from combobox to text control
  m_textMessage->SetValue(m_comboHistory->GetStringSelection());
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
