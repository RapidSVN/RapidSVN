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
//TODO #include "include.hpp"
//TODO #include "rapidsvn_app.hpp"

// wxwindows
#include "wx/sizer.h"
#include <wx/valgen.h>

// app
#include "checkout_dlg.hpp"
#include "utils.hpp"

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

CheckoutDlg::CheckoutDlg (wxWindow * parent)
  : wxDialog (parent, -1, _T("Check out module"), wxDefaultPosition, 
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  InitControls ();
  CentreOnParent();
}

void
CheckoutDlg::OnOK (wxCommandEvent & event)
{
  unsigned long rev = 0;
  
  // Transfer data from controls into m_pData:
  TransferDataFromWindow();
  
  // Validate the data.
  if (!m_data.UseLatest)
  {
    TrimString(m_data.Revision);
    if (!m_data.Revision.ToULong (&rev, 10))
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
    m_data.DestFolder = dialog.GetPath ();
    // Transfer data from m_pData back into controls:
    TransferDataToWindow();
  }
}

/**
 * Set the dialog controls and layout.
 */
void
CheckoutDlg::InitControls ()
{
  // create controls
  wxTextCtrl* moduleName = new wxTextCtrl (this, -1, _T(""), wxPoint(-1,-1),
    wxSize(235, -1), 0, wxTextValidator(wxFILTER_NONE, &m_data.ModuleName));
  wxStaticBox* destBox = new wxStaticBox(this, 0, _T("Destination Folder"));
  m_destFolderText = new wxTextCtrl (this, -1, _T(""), wxDefaultPosition, 
    wxSize(205, -1), 0, wxTextValidator(wxFILTER_NONE, &m_data.DestFolder));
  wxButton* browse = new wxButton(this, ID_BUTTON_BROWSE, _T("..."), 
                                  wxPoint(-1,-1), wxSize(20, -1) );
  m_revisionLabel = new wxStaticText (this, -1, _T("Number"));
  wxStaticBox* revisionBox = new wxStaticBox(this, -1, _T("Revision"));
  m_revisionText = new wxTextCtrl (this, -1, _T(""), wxDefaultPosition, 
    wxSize(50, -1), 0, wxTextValidator(wxFILTER_NUMERIC, &m_data.Revision));                             
  m_useLatestCheck = new wxCheckBox(this, ID_USELATEST, "Use latest",
    wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_data.UseLatest));
  wxTextCtrl* user = new wxTextCtrl (this, -1, _T(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NONE, &m_data.User));
  wxTextCtrl* pass = 
    new wxTextCtrl (this, -1, _T(""), wxPoint(-1,-1), 
                    wxDefaultSize, wxTE_PASSWORD, 
                    wxTextValidator(wxFILTER_NONE, &m_data.Password));
  wxCheckBox* recursive = 
    new wxCheckBox (this, -1, _T("Recursive"),
                    wxDefaultPosition, wxDefaultSize, 0, 
                    wxGenericValidator(&m_data.Recursive));
  wxCheckBox* workbench = 
    new wxCheckBox (this, -1, _T("Add to workbench"),
                    wxDefaultPosition, wxDefaultSize, 0,
                    wxGenericValidator(&m_data.Workbench));
  wxStaticBox* authBox = new wxStaticBox(this, -1, _T("Authentication"));
  wxStaticText* userLabel = new wxStaticText (this, -1, _T("User"));
  wxStaticText* passLabel = new wxStaticText (this, -1, _T("Password"));
  wxButton* ok = new wxButton( this, wxID_OK, _T("OK" ));
  wxButton* cancel = new wxButton( this, wxID_CANCEL, _T("Cancel"));

  // place controls
  // Module row
  wxStaticBoxSizer *moduleSizer = new wxStaticBoxSizer (
          new wxStaticBox(this, -1, _T("URL")), 
          wxHORIZONTAL);
  moduleSizer->Add (moduleName, 1, wxALL | wxEXPAND, 5);

  // Destination row
  wxStaticBoxSizer *destSizer = 
    new wxStaticBoxSizer (destBox, wxHORIZONTAL);
  destSizer->Add (m_destFolderText, 1, wxALL | wxEXPAND, 5);
  destSizer->Add (browse, 0, wxALL, 5);

  // Revision row
  wxBoxSizer *reSizer = new wxBoxSizer (wxHORIZONTAL);
  wxStaticBoxSizer *revisionSizer = 
    new wxStaticBoxSizer (revisionBox, wxHORIZONTAL);
  revisionSizer->Add (m_revisionLabel, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);  
  revisionSizer->Add (m_revisionText, 1, wxALL | wxEXPAND, 5);
  revisionSizer->Add (m_useLatestCheck, 0, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
  reSizer->Add (revisionSizer, 1, wxALL | wxEXPAND, 0);

  // Authentication row
  wxStaticBoxSizer *authSizer = 
    new wxStaticBoxSizer (authBox, wxHORIZONTAL);
  authSizer->Add (userLabel, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  authSizer->Add (user, 1, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  authSizer->Add (passLabel, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);  
  authSizer->Add (pass, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5 );

  // Button row
  wxBoxSizer *buttonSizer  = new wxBoxSizer (wxHORIZONTAL);
  buttonSizer->Add(ok, 0, wxALL, 10);
  buttonSizer->Add(cancel, 0, wxALL, 10);

  // Extras sizer
  wxBoxSizer *extrasSizer = new wxBoxSizer (wxHORIZONTAL);
  extrasSizer->Add(workbench, 0, wxALL | wxCENTER, 5);
  extrasSizer->Add(recursive, 0, wxALL | wxCENTER, 5);

  // Add all sizers to main sizer
  wxBoxSizer *mainSizer    = new wxBoxSizer (wxVERTICAL);
  mainSizer->Add (moduleSizer, 0, wxALL | wxEXPAND, 5);
  mainSizer->Add (destSizer, 0, wxALL | wxEXPAND, 5);
  mainSizer->Add (reSizer, 0, wxALL | wxEXPAND, 5);
  mainSizer->Add (authSizer, 0, wxALL | wxEXPAND, 5);
  mainSizer->Add (extrasSizer, 0, wxALL | wxCENTER, 5);
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
  EnableControls();
}

void 
CheckoutDlg::EnableControls()
{
  bool checked = m_useLatestCheck->IsChecked();
  m_revisionLabel->Enable (checked);
  m_revisionText->Enable (checked);
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */