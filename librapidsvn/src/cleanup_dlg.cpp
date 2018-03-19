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
#include "cleanup_dlg.hpp"
#include "utils.hpp"

struct CleanupDlg::Data
{
public:
  CleanupData data;

  Data(const svn::Path selectedPath)
  {
    data.Path = Utf8ToLocal(selectedPath.native().c_str());
  }

};

CleanupDlg::CleanupDlg(wxWindow * parent,
                       const svn::Path & selectedPath)
  : CleanupDlgBase(parent)
{
  m = new Data(selectedPath);
  m_staticPath->SetLabel(m->data.Path);

  m_checkExternals->SetValidator(wxGenericValidator(&m->data.IncludeExternals));
  m_checkExternals->SetHelpText(_("Also clean up any external items that are located below the selected path."));

  m_checkWCStatus->SetValidator(wxGenericValidator(&m->data.CleanupWCStatus));
  m_checkWCStatus->SetHelpText(_("Clean up the working copy status, necessary if some operation couldn't be finished before."));
  m_checkBreakLocks->SetValidator(wxGenericValidator(&m->data.BreakLocks));
  m_checkBreakLocks->SetHelpText(_("Remove any filesystem locks on the working copy left over by previous unfinished operations."));
  m_checkTimeStamps->SetValidator(wxGenericValidator(&m->data.FixTimestamps));
  m_checkTimeStamps->SetHelpText(_("Fix timestamps of all unmodified files in the working copy."));
  m_checkVacuum->SetValidator(wxGenericValidator(&m->data.VacuumPristines));
  m_checkVacuum->SetHelpText(_("Vacuum pristine copies, removing unnecessary files from the .svn directory."));

  m_checkDeleteUnversioned->SetValidator(wxGenericValidator(&m->data.DeleteUnversioned));
  m_checkDeleteUnversioned->SetHelpText(_("Delete all unversioned files that are located within this working copy."));
  m_checkDeleteIgnored->SetValidator(wxGenericValidator(&m->data.DeleteIgnored));
  m_checkDeleteIgnored->SetHelpText(_("Delete all unversioned and ignored files that are located within this working copy."));

  // Add explicit context-sensitive help button for non-MSW
#ifndef __WXMSW__
  m_buttonSizer->Add(new wxContextHelpButton(this), 0, wxALL, 10);
#endif

  // TODO: online help. Help button doesn't work yet, so hide it.
  //m_buttonHelp->Show(false);

  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  Layout();
  CentreOnParent();

  //CheckControls();
}

CleanupDlg::~CleanupDlg()
{
  delete m;
}

void
CleanupDlg::OnCleanupChecked(wxCommandEvent & WXUNUSED(event))
{
  CheckControls();
}

void
CleanupDlg::OnHelp(wxCommandEvent &)
{
  // @todo Has to be re-integrated for Online Help
  // ::wxGetApp ().GetHelpController().Display(wxT("Cleanup dialog"));
}

const CleanupData &
CleanupDlg::GetData() const
{
  return m->data;
}

void CleanupDlg::OnInitDialog(wxInitDialogEvent &event)
{
  TransferDataToWindow();
  CheckControls();
  event.Skip();
}


void
CleanupDlg::CheckControls()
{
  bool wcStatus = m_checkWCStatus->IsChecked();

  m_checkBreakLocks->Enable(wcStatus);
  m_checkTimeStamps->Enable(wcStatus);
  m_checkVacuum->Enable(wcStatus);
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
