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
#include "include.hpp"
#include "update_dlg.hpp"
#include <wx/sizer.h>
#include <wx/valgen.h>

enum
{
  ID_USELATEST = 100
};

BEGIN_EVENT_TABLE (UpdateDlg, wxDialog)
EVT_CHECKBOX (ID_USELATEST, UpdateDlg::OnUseLatest)
END_EVENT_TABLE ()

UpdateDlg::sData::sData()
{
  // Default values go here:
  UseLatest = true;
}

UpdateDlg::UpdateDlg(wxWindow* parent, sData* pData)
: wxDialog(parent, -1, _T("Update"),
  wxDefaultPosition, wxDefaultSize,
    wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
  m_pData(pData)
{
  InitializeData();
  CentreOnParent();
}

void
UpdateDlg::InitializeData ()
{
  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *middleSizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

  // The revision fields:
  wxStaticBoxSizer *revSizer = new wxStaticBoxSizer(
    new wxStaticBox(this, -1, _T("Revision")), wxHORIZONTAL);
    
  pRevisionLabel = new wxStaticText(this, -1, _T("Number")); 
  revSizer->Add(pRevisionLabel, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
   
  pRevision = new wxTextCtrl (this, -1, _T(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NUMERIC, &m_pData->Revision));
  revSizer->Add (pRevision, 1, 
    wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

  pUseLatest = new wxCheckBox(this, ID_USELATEST, "Use latest",
    wxDefaultPosition, wxDefaultSize, 0,
    wxGenericValidator(&m_pData->UseLatest));
  revSizer->Add (pUseLatest, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);

  topSizer->Add(revSizer, 1, wxALL, 5);
  
  // The authentication fields:
  wxStaticBoxSizer *authSizer = new wxStaticBoxSizer(
    new wxStaticBox(this, -1, _T("Authentication")), wxHORIZONTAL);
    
  authSizer->Add(new wxStaticText(this, -1, _T("User")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
   
  wxTextCtrl* pUser = new wxTextCtrl (this, -1, _T(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NONE, &m_pData->User));
  authSizer->Add (pUser, 1, 
    wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

  authSizer->Add(new wxStaticText (this, -1, _T("Password")), 0,
    wxLEFT | wxALIGN_CENTER_VERTICAL, 5);  
  wxTextCtrl* pass = new wxTextCtrl (this, -1, _T(""), wxPoint(-1,-1), 
    wxDefaultSize, wxTE_PASSWORD, wxTextValidator(wxFILTER_NONE, &m_pData->Password));
  authSizer->Add(pass, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
  
  middleSizer->Add(authSizer, 1, wxALL, 5);

  // The buttons:
  buttonSizer->Add(new wxButton( this, wxID_OK, _T("OK" )), 0, 
                   wxALL, 10);
  buttonSizer->Add(new wxButton( this, wxID_CANCEL, _T("Cancel")), 0, 
                   wxALL, 10);

  // Add all the sizers to the main sizer
  mainSizer->Add (topSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
  mainSizer->Add (middleSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
  mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

  SetAutoLayout(true);
  SetSizer(mainSizer);

  mainSizer->SetSizeHints(this);
  mainSizer->Fit(this);
}

void
UpdateDlg::InitDialog()
{
  wxDialog::InitDialog();
  EnableControls();
}

void 
UpdateDlg::OnUseLatest(wxCommandEvent &)
{
  UpdateDlg::EnableControls();
}

void 
UpdateDlg::EnableControls()
{
  pRevisionLabel->Enable(!pUseLatest->IsChecked());
  pRevision->Enable(!pUseLatest->IsChecked());
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
