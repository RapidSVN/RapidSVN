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
#include "wx/cshelp.h"
#include "wx/sizer.h"
#include "wx/valgen.h"

// svncpp
#include "svncpp/path.hpp"

// app
#include "hist_entries.hpp"
#include "hist_val.hpp"
#include "checkout_dlg.hpp"
#include "utils.hpp"

struct CheckoutDlg::Data
{
public:
  CheckoutData data;

  Data(const svn::Path selectedUrl)
  {
    data.RepUrl = Utf8ToLocal(selectedUrl.native().c_str());
  }

};

CheckoutDlg::CheckoutDlg(wxWindow * parent,
                         const svn::Path & selectedUrl)
  : CheckoutDlgBase(parent)
{
  m = new Data(selectedUrl);

  m_comboUrl->SetValidator(HistoryValidator(HISTORY_REPOSITORY, &m->data.RepUrl));
  m_comboUrl->SetHelpText(_("Enter the repository URL (not local path) here."));
  m_comboDest->SetValidator(HistoryValidator(HISTORY_WORKING_DIRECTORY, &m->data.DestFolder));
  m_comboDest->SetHelpText(_("Enter the local path where you want the code checked out to here."));
  m_textRevision->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &m->data.Revision));
  m_textRevision->SetHelpText(
    _("If not using the latest version of the files, specify which revision to use here."));
  m_checkUseLatest->SetValidator(wxGenericValidator(&m->data.UseLatest));
  m_checkUseLatest->SetHelpText(_("Set this to get the latest version of the files in the repository."));
  m_textPegRevision->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &m->data.PegRevision));
  m_textPegRevision->SetHelpText(_("If the files were renamed or moved some time, specify which peg revision to use here."));
  m_checkPegNotSpecified->SetValidator(wxGenericValidator(&m->data.NotSpecified));
  m_checkPegNotSpecified->SetHelpText(_("Set this to use BASE/HEAD (current) peg revision of the files."));
  m_checkRecursive->SetValidator(wxGenericValidator(&m->data.Recursive));
  m_checkRecursive->SetHelpText(_("Set to get all subdirectories from the URL also."));
  m_checkAddToBookmarks->SetValidator(wxGenericValidator(&m->data.Bookmarks));
  m_checkAddToBookmarks->SetHelpText(_("Set to automatically create a new working copy bookmark."));
  m_checkIgnoreExternals->SetValidator(wxGenericValidator(&m->data.IgnoreExternals));
  m_checkIgnoreExternals->SetHelpText(_("Set to ignore external definitions and the external working copies managed by them."));

  // Add explicit context-sensitive help button for non-MSW
#ifndef __WXMSW__
  m_buttonSizer->Add(new wxContextHelpButton(this), 0, wxALL, 10);
#endif
  
  // TODO: online help. Help button doesn't work yet, so hide it.
  m_buttonHelp->Show(false);

  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  Layout();
  CentreOnParent();

  CheckControls();
}

CheckoutDlg::~CheckoutDlg()
{
  delete m;
}

/**
 * Brings up a directory dialog defaulted to the user's home directory.
 */
void
CheckoutDlg::OnBrowse(wxCommandEvent & WXUNUSED(event))
{
  // Transfer data from controls into m_pData:
  TransferDataFromWindow();
  wxDirDialog dialog(this,
                     _("Select a destination directory"),
                     wxGetHomeDir(), wxDD_NEW_DIR_BUTTON);

  if (dialog.ShowModal() == wxID_OK)
  {
    m->data.DestFolder = dialog.GetPath();
    // Transfer data from m_pData back into controls:
    TransferDataToWindow();

    CheckControls();
  }
}

void
CheckoutDlg::OnCheckBox(wxCommandEvent & WXUNUSED(event))
{
  CheckControls();
}

const CheckoutData &
CheckoutDlg::GetData() const
{
  return m->data;
}

void
CheckoutDlg::OnText(wxCommandEvent &)
{
  CheckControls();
}

void
CheckoutDlg::OnHelp(wxCommandEvent &)
{
  // @todo Has to be re-integrated for Online Help
  // ::wxGetApp ().GetHelpController().Display(wxT("Checkout dialog"));
}

void
CheckoutDlg::CheckControls()
{
  bool useLatest = m_checkUseLatest->IsChecked();
  bool notSpecified = m_checkPegNotSpecified->IsChecked();
  
  m_textRevision->Enable(!useLatest);
  m_textPegRevision->Enable(!notSpecified);
  
  bool ok = true;
  EnableCtrl(m_textRevision, !useLatest);
  if (!useLatest)
    ok = CheckRevision(m_textRevision->GetValue());

  EnableCtrl(m_textPegRevision, !notSpecified);
  if (!notSpecified)
    ok = CheckRevision(m_textPegRevision->GetValue());
  
  if (m_comboDest->GetValue().Length() <= 0)
    ok = false;
  
  if (m_comboUrl->GetValue().Length() <= 0)
    ok = false;

  EnableCtrl(m_buttonOK, ok);
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
