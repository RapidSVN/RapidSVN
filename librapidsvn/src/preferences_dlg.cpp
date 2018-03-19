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

// app
#include "ids.hpp"
#include "preferences_dlg.hpp"
#include "utils.hpp"
#include "preferences.hpp"

/****************************************************************************/

// platform dependant constants
#ifdef _WIN32
static const wxChar * EXECUTABLE_WILDCARD =
  _("Executable Files|*.exe;*.com;*.bat|All files (*.*)|*.*");

#else
// NOTE: (WX 2.4.0) On Motif, neither multiple extensions nor multiple file
// types are supported (wildcard is limited to a single "*.ext" string)
static const wxChar * EXECUTABLE_WILDCARD = _("All files|*");
#endif


PreferencesDlg::PreferencesDlg(wxWindow* parent, Preferences * prefs)
  : PreferencesDlgBase(parent, -1, _("Preferences"),
                       wxDefaultPosition, wxDefaultSize,
                       wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER), m_prefs(prefs)
{
  CentreOnParent();
}

PreferencesDlg::~PreferencesDlg()
{
}

/**
 * @return true if transfer succeeded.
 */
bool
PreferencesDlg::TransferDataFromWindow()
{
  // General
  m_prefs->purgeTempFiles = m_checkPurgeTempFiles->GetValue();
  m_prefs->useLastCommitMessage = m_checkUseLastCommitMessage->GetValue();
  m_prefs->resetFlatModeOnStart = m_checkResetFlatMode->GetValue();

  // Programs
  m_prefs->editor = m_textEditor->GetValue();
  m_prefs->editorArgs = m_textEditorArgs->GetValue();
  m_prefs->editorAlways = m_checkEditorAlways->GetValue();

  m_prefs->explorer = m_textExplorer->GetValue();
  m_prefs->explorerArgs = m_textExplorerArgs->GetValue();
  m_prefs->explorerAlways = m_checkExplorerAlways->GetValue();

  m_prefs->diffTool = m_textDiffTool->GetValue();
  m_prefs->diffToolArgs = m_textDiffToolArgs->GetValue();

  m_prefs->mergeTool = m_textMergeTool->GetValue();
  m_prefs->mergeToolArgs = m_textMergeToolArgs->GetValue();

  // Authentication
  m_prefs->authPerBookmark = m_checkAuthPerBookmark->GetValue();
  m_prefs->useAuthCache = m_checkUseAuthCache->GetValue();

  return true;
}

/**
 * @return true if transfer succeeded.
 */
bool
PreferencesDlg::TransferDataToWindow()
{
  // General
  m_checkPurgeTempFiles->SetValue(m_prefs->purgeTempFiles);
  m_checkUseLastCommitMessage->SetValue(m_prefs->useLastCommitMessage);
  m_checkResetFlatMode->SetValue(m_prefs->resetFlatModeOnStart);

  // Programs
  m_textEditor->SetValue(m_prefs->editor);
  m_textEditorArgs->SetValue(m_prefs->editorArgs);
  m_checkEditorAlways->SetValue(m_prefs->editorAlways);

  m_textExplorer->SetValue(m_prefs->explorer);
  m_textExplorerArgs->SetValue(m_prefs->explorerArgs);
  m_checkExplorerAlways->SetValue(m_prefs->explorerAlways);

  m_textDiffTool->SetValue(m_prefs->diffTool);
  m_textDiffToolArgs->SetValue(m_prefs->diffToolArgs);

  m_textMergeTool->SetValue(m_prefs->mergeTool);
  m_textMergeToolArgs->SetValue(m_prefs->mergeToolArgs);

  // Authentication
  m_checkAuthPerBookmark->SetValue(m_prefs->authPerBookmark);
  m_checkUseAuthCache->SetValue(m_prefs->useAuthCache);

  return true;
}

bool
PreferencesDlg::SelectExecutable(const wxString & title, wxTextCtrl * textCtrl)
{
  wxFileDialog dlg(this, title, wxEmptyString, wxEmptyString,
                   EXECUTABLE_WILDCARD, wxFD_OPEN);
  dlg.SetPath(textCtrl->GetValue());

  if (dlg.ShowModal() != wxID_OK)
    return false;

  textCtrl->SetValue(dlg.GetPath());
  return true;
}


void
PreferencesDlg::OnButtonEditorClick(wxCommandEvent& WXUNUSED(event))
{
  SelectExecutable(
    _("Select standard editor executable"),
    m_textEditor);
}

void
PreferencesDlg::OnButtonExplorerClick(wxCommandEvent& WXUNUSED(event))
{
  SelectExecutable(
    _("Select standard file explorer executable"),
    m_textExplorer);
}

void
PreferencesDlg::OnButtonDiffToolClick(wxCommandEvent& WXUNUSED(event))
{
  SelectExecutable(
    _("Select diff tool executable"),
    m_textDiffTool);
}

void
PreferencesDlg::OnButtonMergeToolClick(wxCommandEvent& WXUNUSED(event))
{
  SelectExecutable(
    _("Select merge tool executable"),
    m_textMergeTool);
}



/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
