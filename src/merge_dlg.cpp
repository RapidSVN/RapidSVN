/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#include "include.h"
#include "rapidsvn_app.h"
#include "merge_dlg.h"
#include <wx/valgen.h>

enum
{
  ID_USELATEST = 100,
  ID_BUTTON_BROWSE,
};

BEGIN_EVENT_TABLE (MergeDlg, wxDialog)
EVT_BUTTON (wxID_OK, MergeDlg::OnOK)
EVT_BUTTON (ID_BUTTON_BROWSE, MergeDlg::OnBrowse)
END_EVENT_TABLE ()

MergeDlg::sData::sData()
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

MergeDlg::MergeDlg (wxWindow * parent, sData* pData)
           : wxDialog (parent, -1, "Merge revisions", wxDefaultPosition, 
             wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
             m_pData(pData)
{
  InitializeData ();
  CentreOnParent();
}

void
MergeDlg::OnOK (wxCommandEvent & event)
{
  // Transfer data from controls into m_pData:
  TransferDataFromWindow();

  TrimString(m_pData->Path1);
  TrimString(m_pData->Path1Rev);
  TrimString(m_pData->Path2);
  TrimString(m_pData->Path2Rev);
  TrimString(m_pData->Destination);
  
  // test revision numbers
  if (TestRev (m_pData->Path1Rev) < 0)
    return;

  if (TestRev (m_pData->Path2Rev) < 0)
    return;

  if (m_pData->Path1.IsEmpty ())
  {
    wxMessageBox (_T ("First path or URL is required for merge!"),
                  _T ("Error"), wxOK | wxCENTRE | wxICON_ERROR);

    // Do not allow the user to continue if the path is empty 
    // and the import is addressing a file.
    return;
  }

  // If start revision for first path is emtpy... 
  // (the end revision should be empty also in this case - checked above)
  if (m_pData->Path1Rev.IsEmpty ())
  {
    // the second path should be specified, as now there is 
    // no deductible interval from the first path
    if (m_pData->Path2.IsEmpty ())
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
  wxStaticBoxSizer *MergeSizer = new wxStaticBoxSizer(
    new wxStaticBox(this, -1, _T("Merge")), wxHORIZONTAL);
    
  wxFlexGridSizer* Grid = new wxFlexGridSizer(6, 2, 0, 0);
  Grid->AddGrowableCol(0);  // The first column can be expanded.

  // Row 0:  
  Grid->Add(new wxStaticText(this, -1, _T("First working copy or URL")), 0, 
    0, 5);
  Grid->Add(new wxStaticText(this, -1, _T("Revision")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 20);
    
  // Row 1:  
  wxTextCtrl *Path1 = new wxTextCtrl(this, -1, _T(""),
    wxDefaultPosition, wxSize(300, -1), 0,
    wxTextValidator(wxFILTER_NONE, &m_pData->Path1));
  Grid->Add(Path1, 1, wxBOTTOM | wxEXPAND, 10);
  
  wxTextCtrl *Path1Rev = new wxTextCtrl(this, -1, _T(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NUMERIC, &m_pData->Path1Rev));
  Grid->Add(Path1Rev, 0, wxLEFT, 20);
    
  // Row 2:
  Grid->Add(new wxStaticText(this, -1, _T("Second working copy or URL")), 0, 
    0, 5);
  Grid->Add(new wxStaticText(this, -1, _T("Revision")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 20);

  // Row 3:  
  wxTextCtrl *Path2 = new wxTextCtrl(this, -1, _T(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NONE, &m_pData->Path2));
  Grid->Add(Path2, 1, wxBOTTOM | wxEXPAND, 10);

  wxTextCtrl *Path2Rev = new wxTextCtrl(this, -1, _T(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NUMERIC, &m_pData->Path2Rev));
  Grid->Add(Path2Rev, 0, wxLEFT, 20);

  // Row 4:
  Grid->Add(new wxStaticText(this, -1, _T("Destination path")), 0, 
    0, 5);
  Grid->Add(new wxStaticText(this, -1, _T("")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 20);

  // Row 5:
  wxTextCtrl *Destination = new wxTextCtrl(this, -1, _T(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NONE, &m_pData->Destination));
  Grid->Add(Destination, 1, wxBOTTOM | wxEXPAND, 5);
  
  wxButton* BrowseButton = new wxButton(this, ID_BUTTON_BROWSE, _T("..."), 
    wxPoint(-1,-1), wxSize(20, -1));
  Grid->Add(BrowseButton, 0, wxALL, 5);

  MergeSizer->Add(Grid, 1, wxALL | wxEXPAND, 5);

  // Sundry items row:
  wxBoxSizer *SundrySizer = new wxBoxSizer(wxHORIZONTAL);
  wxCheckBox* Recursive = new wxCheckBox (this, -1, _T("Recursive"),
    wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_pData->Recursive));
  SundrySizer->Add(Recursive, 0, wxALL, 5);
  wxCheckBox* Force = new wxCheckBox (this, -1, _T("Force"),
    wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_pData->Force));
  SundrySizer->Add(Force, 0, wxALL, 5);

  // Authentication row:
  wxStaticBoxSizer *AuthSizer = new wxStaticBoxSizer(
    new wxStaticBox(this, -1, _T("Authentication")), wxHORIZONTAL);

  AuthSizer->Add(new wxStaticText(this, -1, _T("User")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  wxTextCtrl* user = new wxTextCtrl(this, -1, _T(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NONE, &m_pData->User));
  AuthSizer->Add(user, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);

  AuthSizer->Add(new wxStaticText(this, -1, _T("Password")), 0,
    wxLEFT | wxALIGN_CENTER_VERTICAL, 5);  
  wxTextCtrl* pass = new wxTextCtrl(this, -1, _T(""), wxDefaultPosition, 
    wxDefaultSize, wxTE_PASSWORD, wxTextValidator(wxFILTER_NONE, &m_pData->Password));
  AuthSizer->Add(pass, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
 
  // Button row
  wxBoxSizer *ButtonSizer = new wxBoxSizer(wxHORIZONTAL);
  ButtonSizer->Add(new wxButton(this, wxID_OK, _T("OK" )), 0, 
    wxALL, 10);
  ButtonSizer->Add(new wxButton(this, wxID_CANCEL, _T("Cancel")), 0, 
    wxALL, 10);

  // Add all sizers to main sizer
  mainSizer->Add(MergeSizer, 0, wxALL | wxEXPAND, 5);
  mainSizer->Add(AuthSizer, 0, wxALL | wxEXPAND, 5);
  mainSizer->Add(SundrySizer, 0, wxALL | wxCENTER, 5);
  mainSizer->Add(ButtonSizer, 0, wxALL | wxCENTER, 5);

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
    m_pData->Destination = dialog.GetPath ();
    // Transfer data from m_pData back into controls:
    TransferDataToWindow();
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
