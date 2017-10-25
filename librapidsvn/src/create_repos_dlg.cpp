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
#include "wx/filename.h"

// app
#include "create_repos_dlg.hpp"
#include "hist_val.hpp"
#include "hist_entries.hpp"
#include "svn_executables.hpp"
#include "utils.hpp"


struct CreateReposDlg::Data
{
public:
  bool svnadminAvailable;

  Data()
  {
    svnadminAvailable = !FindExecutableInPath(SVNADMIN_EXECUTABLE).IsEmpty();
  }
};


CreateReposDlg::CreateReposDlg(wxWindow * parent)
  : CreateReposDlgBase(parent)
{
  m = new Data();

  m_choiceType->SetSelection(TYPE_FSFS);
  m_choiceCompat->SetSelection(COMPAT_DEFAULT);
  m_checkAddBookmark->SetValue(true);
  m_notebook->SetSelection(0);

  {
    HistoryValidator val(HISTORY_CREATE_REPOS_DIRECTORY);
    m_comboDir->SetValidator(val);
  }
  {
    HistoryValidator val(HISTORY_CREATE_REPOS_NAME);
    m_comboName->SetValidator(val);
  }
  {
    HistoryValidator val(HISTORY_CREATE_REPOS_CONFIG_DIRECTORY);
    m_comboConfigDir->SetValidator(val);
  }

  // TODO all this stuff doesnt work. Dont know why :-(
  // m_comboDirectory->SetMinSize(wxSize(GetCharWidth() * 60, GetCharHeight()));
  // m_rootSizer->SetSizeHints(this);
  // m_rootSizer->Fit(this);
  //
  // Layout();
  CentreOnParent();

  CheckValues();
}


CreateReposDlg::~CreateReposDlg()
{
  delete m;
}


void
CreateReposDlg::CheckValues()
{
  wxASSERT(0 != m_checkBdbLogKeep);
  wxASSERT(0 != m_checkBdbTxnNoSync);
  wxASSERT(0 != m_buttonOk);
  wxASSERT(0 != m_textFilename);

  wxString msg;
  bool valid=true;

  bool isBdb = TYPE_BDB == GetType();

  m_checkBdbLogKeep->Enable(isBdb);
  m_checkBdbTxnNoSync->Enable(isBdb);

  // Now check whether we can proceed

  wxString name(GetName());
  wxString dir(GetDir());
  wxString configDir(GetConfigDir());
  wxString filename(GetFilename());

  if (!m->svnadminAvailable)
  {
    valid = false;
    msg = _("svnadmin could not be found");
  }
  else if (dir.IsEmpty() || !::IsValidDir(dir))
  {
    valid = false;
    msg = _("Select an existing directory for the repository");
  }
  else if (name.IsEmpty())
  {
    valid = false;
    msg = _("Enter a name for the repository");
  }
  else if (::IsValidDir(filename))
  {
    valid = false;
    msg = _("A directory of this name exists already");
  }
  else if (::wxFileExists(filename))
  {
    valid = false;
    msg = _("A file of this name exists already");
  }
  else if (!configDir.IsEmpty() && !::IsValidDir(configDir))
  {
    valid = false;
    msg = _("The configuration directory does not exist");
  }

  m_textFilename->SetValue(filename);
  m_staticWarning->SetLabel(msg);
  m_buttonOk->Enable(valid);
}


void
CreateReposDlg::OnChoiceType(wxCommandEvent & WXUNUSED(event))
{
  CheckValues();
}

void
CreateReposDlg::OnComboDirText(wxCommandEvent & WXUNUSED(event))
{
  CheckValues();
}

void
CreateReposDlg::OnComboNameText(wxCommandEvent & WXUNUSED(event))
{
  CheckValues();
}

void
CreateReposDlg::OnChoiceCompat(wxCommandEvent & WXUNUSED(event))
{
  CheckValues();
}

void
CreateReposDlg::OnComboConfigDirText(wxCommandEvent & WXUNUSED(event))
{
  CheckValues();
}

void
CreateReposDlg::OnButtonBrowseDirClick(wxCommandEvent & WXUNUSED(event))
{
  wxASSERT(0 != m_comboDir);

  wxDirDialog dialog(this, _("Select a directory"),
                     GetDir(),
                     wxDD_NEW_DIR_BUTTON);

  if (dialog.ShowModal() == wxID_OK)
  {
    m_comboDir->SetValue(dialog.GetPath());
    CheckValues();
  }
}

void
CreateReposDlg::OnButtonBrowseConfigDirClick(wxCommandEvent & WXUNUSED(event))
{
  wxASSERT(0 != m_comboConfigDir);

  wxDirDialog dialog(this, _("Select a directory"),
                     m_comboConfigDir->GetValue(),
                     wxDD_NEW_DIR_BUTTON);

  if (dialog.ShowModal() == wxID_OK)
  {
    m_comboConfigDir->SetValue(dialog.GetPath());
    CheckValues();
  }
}

bool
CreateReposDlg::TransferDataFromWindow()
{
  // without wxWS_EX_VALIDATE_RECURSIVELY we need to make
  // sure the history gets saved. Settings this flasg doesnt work
  // since then the dialog gets shown in the taskbar (on Windows)
  return CreateReposDlgBase::TransferDataFromWindow() &&
         m_panelGeneral->TransferDataFromWindow() &&
         m_panelExtended->TransferDataFromWindow();
}


bool
CreateReposDlg::TransferDataToWindow()
{
  return CreateReposDlgBase::TransferDataToWindow() &&
         m_panelGeneral->TransferDataToWindow() &&
         m_panelExtended->TransferDataToWindow();
}


wxString
CreateReposDlg::GetDir() const
{
  wxASSERT(0 != m_comboDir);

  wxString dir(m_comboDir->GetValue());
  TrimString(dir);

  return dir;
}


wxString
CreateReposDlg::GetName() const
{
  wxASSERT(0 != m_comboName);

  wxString name(m_comboName->GetValue());
  TrimString(name);

  return name;
}


wxString
CreateReposDlg::GetFilename() const
{
  wxString dir(GetDir());
  wxString name(GetName());

  if (dir.IsEmpty() || name.IsEmpty())
    return wxEmptyString;

  wxFileName filename(GetDir(), GetName());

  filename.Normalize(wxPATH_NORM_ENV_VARS |
                     wxPATH_NORM_DOTS |
                     wxPATH_NORM_ABSOLUTE |
                     wxPATH_NORM_LONG |
                     wxPATH_NORM_TILDE);

  return filename.GetFullPath();
}


wxString
CreateReposDlg::GetConfigDir() const
{
  wxASSERT(0 != m_comboConfigDir);

  wxString configDir(m_comboConfigDir->GetValue());
  TrimString(configDir);

  return configDir;
}


int
CreateReposDlg::GetType() const
{
  wxASSERT(0 != m_choiceType);

  return m_choiceType->GetSelection();
}


int
CreateReposDlg::GetCompat() const
{
  wxASSERT(0 != m_choiceCompat);

  return m_choiceCompat->GetSelection();
}


bool
CreateReposDlg::GetBdbLogKeep() const
{
  wxASSERT(0 != m_checkBdbLogKeep);

  return m_checkBdbLogKeep->GetValue();
}


bool
CreateReposDlg::GetBdbTxnNoSync() const
{
  wxASSERT(0 != m_checkBdbTxnNoSync);

  return m_checkBdbTxnNoSync->GetValue();
}


bool
CreateReposDlg::GetAddBookmark() const
{
  wxASSERT(0 != m_checkAddBookmark);

  return m_checkAddBookmark->GetValue();
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
