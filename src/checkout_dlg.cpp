/*
 * ====================================================================
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
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
#include "checkout_dlg.h"
#include "wx/sizer.h"
#include <wx/valgen.h>

enum
{
  ID_USELATEST = 100,
  ID_BUTTON_BROWSE,
};

BEGIN_EVENT_TABLE (CheckoutDlg, wxDialog)
EVT_BUTTON (ID_BUTTON_BROWSE, CheckoutDlg::OnBrowse)
EVT_BUTTON (wxID_OK, CheckoutDlg::OnOK)
EVT_CHECKBOX (ID_USELATEST, CheckoutDlg::OnUseLatest)
END_EVENT_TABLE ()

CheckoutDlg::sData::sData()
{
  // Default values go here:
  UseLatest = true;
  Recursive = true;
}

CheckoutDlg::CheckoutDlg (wxWindow * parent, sData* pData)
           : wxDialog (parent, -1, "Check out module", wxDefaultPosition, 
             wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
             m_pData(pData)
{
  InitializeData ();
  CentreOnParent();
}

void
CheckoutDlg::OnOK (wxCommandEvent & event)
{
  unsigned long rev = 0;
  
  // Transfer data from controls into m_pData:
  TransferDataFromWindow();
  
  // Validate the data.
  if (!m_pData->UseLatest)
  {
    TrimString(m_pData->Revision);
    if (!m_pData->Revision.ToULong (&rev, 10))
    {
      // could not convert revision to a number
      wxMessageBox (_T ("Revision must be an unsigned number!"),
                    _T ("Error"), wxOK | wxCENTRE | wxICON_ERROR);
      return;
    }
  }

  wxDialog::OnOK(event);
}

/**
 * Brings up a directory dialog defaulted to the user's home directory.
 */
void
CheckoutDlg::OnBrowse (wxCommandEvent & event)
{
  // Transfer data from controls into m_pData:
  TransferDataFromWindow();
  wxDirDialog dialog (this,
                      _T ("Select a destination folder to checkout to"),
                      wxGetHomeDir(), wxDD_NEW_DIR_BUTTON);

  if (dialog.ShowModal () == wxID_OK)
  {
    m_pData->DestFolder = dialog.GetPath ();
    // Transfer data from m_pData back into controls:
    TransferDataToWindow();
  }
}

/**
 * Set the dialog controls and layout.
 */
void
CheckoutDlg::InitializeData ()
{
  wxBoxSizer *mainSizer    = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *buttonSizer  = new wxBoxSizer (wxHORIZONTAL);

  // Module row
  wxStaticBoxSizer *moduleSizer = new wxStaticBoxSizer (
          new wxStaticBox(this, -1, _T("URL")), 
          wxHORIZONTAL);
  wxTextCtrl* moduleName = new wxTextCtrl (this, -1, _T(""), wxPoint(-1,-1),
    wxSize(235, -1), 0,
    wxTextValidator(wxFILTER_NONE, &m_pData->ModuleName));
  moduleSizer->Add (moduleName, 1, wxALL | wxEXPAND, 5);

  // Destination row
  wxStaticBoxSizer *destSizer = new wxStaticBoxSizer (
          new wxStaticBox(this, 0, _T("Destination Folder")), 
          wxHORIZONTAL);
  destFolder = new wxTextCtrl (this, -1, _T(""), wxDefaultPosition, 
    wxSize(205, -1), 0,
    wxTextValidator(wxFILTER_NONE, &m_pData->DestFolder));
  destSizer->Add (destFolder, 1, wxALL | wxEXPAND, 5);
  destSizer->Add (new wxButton( this, ID_BUTTON_BROWSE, _T("..."), 
                               wxPoint(-1,-1), wxSize(20, -1) ),
                               0, wxALL, 5);

  // Revision row
  wxBoxSizer *reSizer = new wxBoxSizer (wxHORIZONTAL);
  wxStaticBoxSizer *revisionSizer = new wxStaticBoxSizer (
          new wxStaticBox(this, -1, _T("Revision")), wxHORIZONTAL);
  pRevisionLabel = new wxStaticText (this, -1, _T("Number"));
  revisionSizer->Add (pRevisionLabel, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);  
  revision = new wxTextCtrl (this, -1, _T(""), wxDefaultPosition, 
    wxSize(50, -1), 0,
    wxTextValidator(wxFILTER_NUMERIC, &m_pData->Revision));                             
                             
  revisionSizer->Add (revision, 1, wxALL | wxEXPAND, 5);
  pUseLatest = new wxCheckBox(this, ID_USELATEST, "Use latest",
    wxDefaultPosition, wxDefaultSize, 0,
    wxGenericValidator(&m_pData->UseLatest));
  revisionSizer->Add (pUseLatest, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);


  reSizer->Add (revisionSizer, 1, wxALL | wxEXPAND, 0);

  // Authentication row
  wxStaticBoxSizer *authSizer = new wxStaticBoxSizer (
          new wxStaticBox(this, -1, _T("Authentication")), wxHORIZONTAL);

  authSizer->Add (new wxStaticText (this, -1, _T("User")), 0, 
                              wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  wxTextCtrl* user = new wxTextCtrl (this, -1, _T(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NONE, &m_pData->User));
  authSizer->Add (user, 1, 
                  wxLEFT | wxALIGN_CENTER_VERTICAL, 5);

  authSizer->Add (new wxStaticText (this, -1, _T("Password")), 0,
                           wxLEFT | wxALIGN_CENTER_VERTICAL, 5);  
  wxTextCtrl* pass = new wxTextCtrl (this, -1, _T(""), wxPoint(-1,-1), 
    wxDefaultSize, wxTE_PASSWORD, wxTextValidator(wxFILTER_NONE, &m_pData->Password));
  authSizer->Add (pass, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5 );
 
  wxCheckBox* recursive = new wxCheckBox (this, -1, _T("Recursive"),
    wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_pData->Recursive));
  

  // Button row
  buttonSizer->Add(new wxButton( this, wxID_OK, _T("OK" )), 0, 
                   wxALL, 10);
  buttonSizer->Add(new wxButton( this, wxID_CANCEL, _T("Cancel")), 0, 
                   wxALL, 10);

  // Add all sizers to main sizer
  mainSizer->Add (moduleSizer, 0, wxALL | wxEXPAND, 5);
  mainSizer->Add (destSizer, 0, wxALL | wxEXPAND, 5);
  mainSizer->Add (reSizer, 0, wxALL | wxEXPAND, 5);
  mainSizer->Add (authSizer, 0, wxALL | wxEXPAND, 5);
  mainSizer->Add (recursive, 0, wxALL | wxCENTER, 5);
  mainSizer->Add (buttonSizer, 0, wxALL | wxCENTER, 5);

  SetAutoLayout(true);
  SetSizer(mainSizer);

  mainSizer->SetSizeHints(this);
  mainSizer->Fit(this);
}

void
CheckoutDlg::InitDialog()
{
  wxDialog::InitDialog();
  EnableControls();
}

void 
CheckoutDlg::OnUseLatest(wxCommandEvent &)
{
  CheckoutDlg::EnableControls();
}

void 
CheckoutDlg::EnableControls()
{
  pRevisionLabel->Enable(!pUseLatest->IsChecked());
  revision->Enable(!pUseLatest->IsChecked());
}

