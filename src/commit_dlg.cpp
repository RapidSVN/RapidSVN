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

// wxwindows
#include "wx/wx.h"
#include "wx/valgen.h"

// app
#include "commit_dlg.hpp"

BEGIN_EVENT_TABLE (CommitDlg, wxDialog)
END_EVENT_TABLE ()

CommitDlg::CommitDlg(wxWindow* parent)
  : wxDialog(parent, -1, _T("Commit changes to repository"),
             wxDefaultPosition, wxDefaultSize,
             wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  InitializeData();
  CentreOnParent();
}

void
CommitDlg::InitializeData ()
{
  // create controls
  wxStaticBox* msgBox = new wxStaticBox(this, -1, _T("Enter log message"));
  wxTextCtrl* msg = 
    new wxTextCtrl(this, -1, _T(""), wxDefaultPosition, 
                   wxSize(-1, 50), wxTE_MULTILINE,
                   wxTextValidator(wxFILTER_NONE, &m_data.LogMessage));
  wxStaticBox* authBox = new wxStaticBox(this, -1, _T("Authentication"));
  wxStaticText* userLabel = new wxStaticText(this, -1, _T("User"));
  wxTextCtrl* user = new wxTextCtrl (this, -1, _T(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NONE, &m_data.User));
  wxStaticText* passLabel = 
    new wxStaticText (this, -1, _T("Password"));
  wxTextCtrl* pass = 
    new wxTextCtrl (this, -1, _T(""), wxPoint(-1,-1), 
                    wxDefaultSize, wxTE_PASSWORD, 
                    wxTextValidator(wxFILTER_NONE, &m_data.Password));
  wxCheckBox* recursive  = 
    new wxCheckBox(this, -1, "Recursive",
                   wxDefaultPosition, wxDefaultSize, 0,
                   wxGenericValidator(&m_data.Recursive));
  wxButton* ok =  new wxButton( this, wxID_OK, _T("OK" ));
  wxButton* cancel = new wxButton( this, wxID_CANCEL, _T("Cancel"));

  // position controls
  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

  // The message field:
  wxStaticBoxSizer *msgSizer = 
    new wxStaticBoxSizer(msgBox, wxHORIZONTAL);
  msgSizer->Add(msg, 1, wxALL | wxEXPAND, 5);
  
  // The authentication fields:
  wxStaticBoxSizer *authSizer = new wxStaticBoxSizer(
    authBox, wxHORIZONTAL);
    
  authSizer->Add(userLabel, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  authSizer->Add (user, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);
  authSizer->Add(passLabel, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);  
  authSizer->Add(pass, 1, wxALL | wxALIGN_CENTER_VERTICAL, 5);
  
  wxBoxSizer *middleSizer = new wxBoxSizer(wxHORIZONTAL);
  middleSizer->Add(authSizer, 1, wxALL | wxEXPAND, 5);

  // The "recursive" check box:
    
  // The buttons:
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  buttonSizer->Add(ok, 0, wxALL, 10);
  buttonSizer->Add(cancel, 0, wxALL, 10);

  wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
  topSizer->Add(msgSizer, 1, wxALL | wxEXPAND, 5);

  // Add all the sizers to the main sizer
  mainSizer->Add (topSizer, 1, wxLEFT | wxRIGHT | wxEXPAND, 5);
  mainSizer->Add (middleSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
  mainSizer->Add (recursive, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
  mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

  SetAutoLayout(true);
  SetSizer(mainSizer);

  mainSizer->SetSizeHints(this);
  mainSizer->Fit(this);
}

const CommitData &
CommitDlg::GetData () const
{
  return m_data;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
