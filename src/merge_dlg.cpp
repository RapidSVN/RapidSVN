/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxwindows
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

MergeData::MergeData()
{
  // Default values go here:
  Recursive = true;
  Force = false;
}

int
MergeDlg::TestRev (wxString & val)
{
  unsigned long rev = 0;

  if (!val.ToULong (&rev, 10) && rev < 0)
  {
    // could not convert revision to a number
    wxMessageBox (_T ("Revision must be an unsigned number!"),
                  _T ("Error"), wxOK | wxCENTRE | wxICON_ERROR);
    return -1;
  }

  return 0;
}

MergeDlg::MergeDlg (wxWindow * parent, MergeData & data)
           : wxDialog (parent, -1, _T("Merge revisions"), wxDefaultPosition, 
             wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
             m_data(data)
{
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
    wxMessageBox (_T ("First path or URL is required for merge!"),
                  _T ("Error"), wxOK | wxCENTRE | wxICON_ERROR);

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
      wxMessageBox (_T ("Second path or URL is required for merge!"),
                    _T ("Error"), wxOK | wxCENTRE | wxICON_ERROR);

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
    new wxStaticBox(this, -1, _T("Merge")), wxHORIZONTAL);
    
  wxFlexGridSizer* grid = new wxFlexGridSizer(6, 2, 0, 0);
  grid->AddGrowableCol(0);  // The first column can be expanded.

  // Row 0:  
  grid->Add(new wxStaticText(this, -1, _T("First working copy or URL")), 0, 
    0, 5);
  grid->Add(new wxStaticText(this, -1, _T("Revision")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 20);
    
  // Row 1:  
  wxTextCtrl *Path1 = new wxTextCtrl(this, -1, "",
    wxDefaultPosition, wxSize(300, -1), 0,
    wxTextValidator(wxFILTER_NONE, &m_data.Path1));
  grid->Add(Path1, 1, wxBOTTOM | wxEXPAND, 10);
  
  wxTextCtrl *Path1Rev = new wxTextCtrl(this, -1, "",
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NUMERIC, &m_data.Path1Rev));
  grid->Add(Path1Rev, 0, wxLEFT, 20);
    
  // Row 2:
  grid->Add(new wxStaticText(this, -1, _T("Second working copy or URL")), 0, 
    0, 5);
  grid->Add(new wxStaticText(this, -1, _T("Revision")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 20);

  // Row 3:  
  wxTextCtrl *Path2 = new wxTextCtrl(this, -1, "",
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NONE, &m_data.Path2));
  grid->Add(Path2, 1, wxBOTTOM | wxEXPAND, 10);

  wxTextCtrl *Path2Rev = new wxTextCtrl(this, -1, "",
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NUMERIC, &m_data.Path2Rev));
  grid->Add(Path2Rev, 0, wxLEFT, 20);

  // Row 4:
  grid->Add(new wxStaticText(this, -1, _T("Destination path")), 0, 
    0, 5);
  grid->Add(new wxStaticText(this, -1, ""), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 20);

  // Row 5:
  wxTextCtrl *Destination = new wxTextCtrl(this, -1, "",
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NONE, &m_data.Destination));
  grid->Add(Destination, 1, wxBOTTOM | wxEXPAND, 5);
  
  wxButton* BrowseButton = new wxButton(this, ID_BUTTON_BROWSE, _T("..."), 
    wxPoint(-1,-1), wxSize(20, -1));
  grid->Add(BrowseButton, 0, wxALL, 5);

  mergeSizer->Add(grid, 1, wxALL | wxEXPAND, 5);

  // Sundry items row:
  wxBoxSizer *sundrySizer = new wxBoxSizer(wxHORIZONTAL);
  wxCheckBox* Recursive = new wxCheckBox (this, -1, _T("Recursive"),
    wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_data.Recursive));
  sundrySizer->Add(Recursive, 0, wxALL, 5);
  wxCheckBox* Force = new wxCheckBox (this, -1, _T("Force"),
    wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_data.Force));
  sundrySizer->Add(Force, 0, wxALL, 5);

  // Authentication row:
  wxStaticBoxSizer *authSizer = new wxStaticBoxSizer(
    new wxStaticBox(this, -1, _T("Authentication")), wxHORIZONTAL);

  authSizer->Add(new wxStaticText(this, -1, _T("User")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  wxTextCtrl* user = new wxTextCtrl(this, -1, "",
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NONE, &m_data.User));
  authSizer->Add(user, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);

  authSizer->Add(new wxStaticText(this, -1, _T("Password")), 0,
    wxLEFT | wxALIGN_CENTER_VERTICAL, 5);  
  wxTextCtrl* pass = new wxTextCtrl(this, -1, "", wxDefaultPosition, 
    wxDefaultSize, wxTE_PASSWORD, wxTextValidator(wxFILTER_NONE, &m_data.Password));
  authSizer->Add(pass, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
 
  // Button row
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  buttonSizer->Add(new wxButton(this, wxID_OK, _T("OK" )), 0, 
    wxALL, 10);
  buttonSizer->Add(new wxButton(this, wxID_CANCEL, _T("Cancel")), 0, 
    wxALL, 10);

  // Add all sizers to main sizer
  mainSizer->Add(mergeSizer, 0, wxALL | wxEXPAND, 5);
  mainSizer->Add(authSizer, 0, wxALL | wxEXPAND, 5);
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
                      _T ("Select a destination folder to merge to"),
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
