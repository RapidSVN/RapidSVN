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
#include "delete_dlg.hpp"

BEGIN_EVENT_TABLE (DeleteDlg, wxDialog)
END_EVENT_TABLE ()

DeleteDlg::sData::sData()
{
  // Default values go here:
  Force = false;
}

DeleteDlg::DeleteDlg(wxWindow* parent, sData* pData)
: wxDialog(parent, -1, _("Remove files and directories from version control"),
  wxDefaultPosition, wxDefaultSize,
    wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
  m_pData(pData)
{
  InitializeData();
  CentreOnParent();
}

void
DeleteDlg::InitializeData ()
{
  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *middleSizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

  // The message field:
  wxStaticBoxSizer *messageSizer = new wxStaticBoxSizer(
    new wxStaticBox(this, -1, _("Enter log message")), wxHORIZONTAL);
    
  wxTextCtrl* Log = new wxTextCtrl(this, -1, _(""), wxDefaultPosition, 
    wxSize(-1, 50), wxTE_MULTILINE,
    wxTextValidator(wxFILTER_NONE, &m_pData->LogMessage));
    
  messageSizer->Add(Log, 1, wxALL | wxEXPAND, 5);
  
  topSizer->Add(messageSizer, 1, wxALL | wxEXPAND, 5);
  
  // The "force" check box:
  wxCheckBox* pCheck = new wxCheckBox(this, -1, "Force removal",
    wxDefaultPosition, wxDefaultSize, 0,
    wxGenericValidator(&m_pData->Force));
    
  // The authentication fields:
  wxStaticBoxSizer *authSizer = new wxStaticBoxSizer(
    new wxStaticBox(this, -1, _("Authentication")), wxHORIZONTAL);
    
  authSizer->Add(new wxStaticText(this, -1, _("User")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
    
  wxTextCtrl* pUser = new wxTextCtrl (this, -1, _(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NONE, &m_pData->User));
  authSizer->Add (pUser, 1, 
    wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

  authSizer->Add(new wxStaticText (this, -1, _("Password")), 0,
    wxLEFT | wxALIGN_CENTER_VERTICAL, 5);  
  wxTextCtrl* pass = new wxTextCtrl (this, -1, _(""), wxPoint(-1,-1), 
    wxDefaultSize, wxTE_PASSWORD, wxTextValidator(wxFILTER_NONE, &m_pData->Password));
  authSizer->Add(pass, 1, wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
  
  middleSizer->Add(authSizer, 1, wxALL | wxEXPAND, 5);

  // The buttons:
  buttonSizer->Add(new wxButton( this, wxID_OK, _("OK" )), 0, 
                   wxALL, 10);
  buttonSizer->Add(new wxButton( this, wxID_CANCEL, _("Cancel")), 0, 
                   wxALL, 10);

  // Add all the sizers to the main sizer
  mainSizer->Add (topSizer, 1, wxLEFT | wxRIGHT | wxEXPAND, 5);
  mainSizer->Add (pCheck, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
  mainSizer->Add (middleSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
  mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

  SetAutoLayout(true);
  SetSizer(mainSizer);

  mainSizer->SetSizeHints(this);
  mainSizer->Fit(this);}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
