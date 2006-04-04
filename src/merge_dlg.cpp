/*
 * ====================================================================
 * Copyright (c) 2002-2006 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxWidgets
#include "wx/wx.h"
#include "wx/valgen.h"

// app
#include "merge_dlg.hpp"
#include "utils.hpp"

enum
{
  ID_USELATEST = 100,
  ID_BUTTON_BROWSE,
};

BEGIN_EVENT_TABLE (MergeDlg, wxDialog)
  EVT_BUTTON (wxID_OK, MergeDlg::OnOK)
  EVT_BUTTON (ID_BUTTON_BROWSE, MergeDlg::OnBrowse)
END_EVENT_TABLE ()

int
MergeDlg::TestRev (wxString & val)
{
  unsigned long rev = 0;

  if (!val.ToULong (&rev, 10) && rev < 0)
  {
    // could not convert revision to a number
    wxMessageBox (_("Revision must be an unsigned number!"),
                  _("Error"), wxOK | wxCENTRE | wxICON_ERROR);
    return -1;
  }

  return 0;
}

MergeDlg::MergeDlg (wxWindow * parent, bool calledByLogDlg, MergeData & data)
           : wxDialog (parent, -1, _("Merge revisions"), wxDefaultPosition,
             wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
             m_data(data)
{
  m_data.calledByLogDlg = calledByLogDlg;
  InitializeData ();
  CentreOnParent();
}

void
MergeDlg::OnOK (wxCommandEvent & event)
{
  // Transfer data from controls into m_pData:
  TransferDataFromWindow();

  TrimString (m_data.Path1);
  TrimString (m_data.Path1Rev);
  TrimString (m_data.Path2);
  TrimString (m_data.Path2Rev);
  TrimString (m_data.Destination);

  // test revision numbers
  if (TestRev (m_data.Path1Rev) < 0)
    return;

  if (TestRev (m_data.Path2Rev) < 0)
    return;

  if (m_data.Path1.IsEmpty ())
  {
    wxMessageBox (_("First path or URL is required for merge!"),
                  _("Error"), wxOK | wxCENTRE | wxICON_ERROR);

    // Do not allow the user to continue if the path is empty
    // and the import is addressing a file.
    return;
  }

  // If start revision for first path is emtpy...
  // (the end revision should be empty also in this case - checked above)
  if (m_data.Path1Rev.IsEmpty ())
  {
    // the second path should be specified, as now there is
    // no deductible interval from the first path
    if (m_data.Path2.IsEmpty ())
    {
      wxMessageBox (_("Second path or URL is required for merge!"),
                    _("Error"), wxOK | wxCENTRE | wxICON_ERROR);

      // Do not allow the user to continue if the path is empty
      // and the import is addressing a file.
      return;
    }
  }

  wxDialog::OnOK(event);
}

void
MergeDlg::InitializeData ()
{
  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

  // Merge paths grid:
  wxStaticBoxSizer *mergeSizer = new wxStaticBoxSizer(
    new wxStaticBox(this, -1, _("Merge")), wxHORIZONTAL);

  wxFlexGridSizer* grid = new wxFlexGridSizer(6, 2, 0, 0);
  grid->AddGrowableCol(0);  // The first column can be expanded.

  // Row 0:
  grid->Add(new wxStaticText(this, -1, _("First working copy or URL")), 0,
    0, 5);
  grid->Add(new wxStaticText(this, -1, _("Revision")), 0,
    wxLEFT | wxALIGN_CENTER_VERTICAL, 20);

  // Row 1:
  wxTextCtrl *Path1 = new wxTextCtrl(this, -1, wxEmptyString,
    wxDefaultPosition, wxSize(300, -1), 0,
    wxTextValidator(wxFILTER_NONE, &m_data.Path1));
  grid->Add(Path1, 1, wxBOTTOM | wxEXPAND, 10);

  wxTextCtrl *Path1Rev = new wxTextCtrl(this, -1, wxEmptyString,
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NUMERIC, &m_data.Path1Rev));
  grid->Add(Path1Rev, 0, wxLEFT, 20);

  // Row 2:
  grid->Add(new wxStaticText(this, -1, _("Second working copy or URL")), 0,
    0, 5);
  grid->Add(new wxStaticText(this, -1, _("Revision")), 0,
    wxLEFT | wxALIGN_CENTER_VERTICAL, 20);

  // Row 3:
  wxTextCtrl *Path2 = new wxTextCtrl(this, -1, wxEmptyString,
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NONE, &m_data.Path2));
  grid->Add(Path2, 1, wxBOTTOM | wxEXPAND, 10);

  wxTextCtrl *Path2Rev = new wxTextCtrl(this, -1, wxEmptyString,
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NUMERIC, &m_data.Path2Rev));
  grid->Add(Path2Rev, 0, wxLEFT, 20);

  // Row 4:
  if (m_data.calledByLogDlg)
    grid->Add(new wxStaticText(this, -1, _("Destination file")), 0, 0, 5);
  else
    grid->Add(new wxStaticText(this, -1, _("Destination path")), 0, 0, 5);
  grid->Add(new wxStaticText(this, -1, wxEmptyString), 0,
    wxLEFT | wxALIGN_CENTER_VERTICAL, 20);

  // Row 5:
  wxTextCtrl *Destination = new wxTextCtrl(this, -1, wxEmptyString,
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NONE, &m_data.Destination));
  grid->Add(Destination, 1, wxBOTTOM | wxEXPAND, 5);

  // If called by the log dialogue, the source path and revision is
  // already given by the selected entries
  if (m_data.calledByLogDlg)
  {
    Path1->Disable();
    Path2->Disable();
    Path1Rev->Disable();
    Path2Rev->Disable();
  }

  wxButton* BrowseButton = new wxButton(this, ID_BUTTON_BROWSE, wxT("..."),
    wxPoint(-1,-1), wxSize(20, -1));
  grid->Add(BrowseButton, 0, wxALL, 5);

  mergeSizer->Add(grid, 1, wxALL | wxEXPAND, 5);

  // Sundry items row:
  wxBoxSizer *sundrySizer = new wxBoxSizer(wxHORIZONTAL);
  wxCheckBox* Recursive = new wxCheckBox (this, -1, _("Recursive"),
    wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_data.Recursive));
  sundrySizer->Add(Recursive, 0, wxALL, 5);
  wxCheckBox* Force = new wxCheckBox (this, -1, _("Force"),
    wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_data.Force));
  sundrySizer->Add(Force, 0, wxALL, 5);

  // Button row
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  buttonSizer->Add(new wxButton(this, wxID_OK, _("OK" )), 0,
    wxALL, 10);
  buttonSizer->Add(new wxButton(this, wxID_CANCEL, _("Cancel")), 0,
    wxALL, 10);

  // Add all sizers to main sizer
  mainSizer->Add(mergeSizer, 0, wxALL | wxEXPAND, 5);
  mainSizer->Add(sundrySizer, 0, wxALL | wxCENTER, 5);
  mainSizer->Add(buttonSizer, 0, wxALL | wxCENTER, 5);

  SetAutoLayout(true);
  SetSizer(mainSizer);

  mainSizer->SetSizeHints(this);
  mainSizer->Fit(this);
}

/**
 * Brings up a directory dialog defaulted to the user's home directory.
 */
void
MergeDlg::OnBrowse (wxCommandEvent & event)
{
  // Transfer data from controls into m_pData:
  TransferDataFromWindow();
  wxDirDialog dialog (this,
                      _("Select a destination folder to merge to"),
                      wxGetHomeDir());

  if (dialog.ShowModal () == wxID_OK)
  {
    m_data.Destination = dialog.GetPath ();
    // Transfer data from m_pData back into controls:
    TransferDataToWindow();
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
