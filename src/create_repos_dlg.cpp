/*
 * ====================================================================
 * Copyright (c) 2002-2009 The RapidSvn Group.  All rights reserved.
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
#include "utils.hpp"

struct CreateReposDlg::Data
{
public:
  bool svnadminAvailable;

  Data()
  {
    svnadminAvailable = !FindExecutableInPath(wxT("svnadmin")).IsEmpty();
  }
};


CreateReposDlg::CreateReposDlg(wxWindow * parent)
  : CreateReposDlgBase(parent)
{
  m = new Data();

  m_comboType->SetSelection(TYPE_FSFS);
  m_comboCompatibility->SetSelection(COMPAT_DEFAULT);
  m_checkCreateBookmark->SetValue(true);
  m_notebook->SetSelection(0);

  {
    HistoryValidator val(HISTORY_CREATE_REPOS_DIRECTORY);
    m_comboDirectory->SetValidator(val);
  }
  {
    HistoryValidator val(HISTORY_CREATE_REPOS_NAME);
    m_comboName->SetValidator(val);
  }
  {
    HistoryValidator val(HISTORY_CREATE_REPOS_CONFIG_DIRECTORY);
    m_comboConfigDir->SetValidator(val);
  }

  //SetSizer(m_rootSizer);

  m_comboDirectory->SetMinSize(wxSize(GetCharWidth() * 60, GetCharHeight()));
  m_rootSizer->SetSizeHints(this);
  m_rootSizer->Fit(this);

  Layout();
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
  wxASSERT(0 != m_comboDirectory);
  wxASSERT(0 != m_comboType);
  wxASSERT(0 != m_checkBdbLogKeep);
  wxASSERT(0 != m_checkBdbTxnNoSync);
  wxASSERT(0 != m_buttonOk);
  wxASSERT(0 != m_comboName);
  wxASSERT(0 != m_comboConfigDir);

  wxString msg;
  bool valid=true;

  bool isBdb = TYPE_BDB == m_comboType->GetSelection();

  m_checkBdbLogKeep->Enable(isBdb);
  m_checkBdbTxnNoSync->Enable(isBdb);

  // Now check whether we can proceed

  wxString name(m_comboName->GetValue());
  TrimString(name);
  wxString dir(m_comboDirectory->GetValue());
  TrimString(dir);
  wxString configDir(m_comboConfigDir->GetValue());
  TrimString(configDir);

  wxFileName filename(dir, name);

  if (!m->svnadminAvailable)
  {
    valid = false;
    msg = _("svnadmin could not be found");
  }
  else if (!::wxDirExists(dir))
  {
    valid = false;
    msg = _("Select an existing directory for the repository");
  }
  else if (name.IsEmpty())
  {
    valid = false;
    msg = _("Enter a name for the repository");
  }
  else if (::wxDirExists(filename.GetFullPath()))
  {
    valid = false;
    msg = _("A directory of this name exists already");
  }
  else if (::wxFileExists(filename.GetFullPath()))
  {
    valid = false;
    msg = _("A file of this name exists already");
  }
  else if (!configDir.IsEmpty() && !::wxDirExists(configDir))
  {
    valid = false;
    msg = _("The configuration directory does not exist");
  }

  m_staticWarning->SetLabel(msg);

  m_buttonOk->Enable(valid);
}


void 
CreateReposDlg::OnComboType(wxCommandEvent& event)
{
  CheckValues();
}

void 
CreateReposDlg::OnComboDirText(wxCommandEvent& event)
{
  CheckValues();
}

void 
CreateReposDlg::OnComboNameText(wxCommandEvent& event)
{
  CheckValues();
}

void 
CreateReposDlg::OnComboCompatibility(wxCommandEvent& event)
{
  CheckValues();
}

void 
CreateReposDlg::OnComboConfigDirText(wxCommandEvent& event)
{
  CheckValues();
}

void 
CreateReposDlg::OnButtonBrowseDirClick(wxCommandEvent& event)
{
  wxASSERT(0 != m_comboDirectory);

  wxDirDialog dialog(this, _("Select a directory"),
                     m_comboDirectory->GetValue(),
                     wxDD_NEW_DIR_BUTTON);

  if (dialog.ShowModal() == wxID_OK)
  {
    m_comboDirectory->SetValue(dialog.GetPath());
    CheckValues();
  }
}

void 
CreateReposDlg::OnButtonBrowseConfigDirClick(wxCommandEvent& event)
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



/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
