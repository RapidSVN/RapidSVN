#include "include.h"
#include "commit_dlg.h"
#include <wx/sizer.h>
#include <wx/valgen.h>

BEGIN_EVENT_TABLE (CommitDlg, wxDialog)
END_EVENT_TABLE ()

CommitDlg::sData::sData()
{
  // Default values go here:
  Recursive = false;
}

CommitDlg::CommitDlg(wxWindow* parent, sData* pData)
: wxDialog(parent, -1, _T("Commit changes to repository"),
  wxDefaultPosition, wxDefaultSize,
    wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
  m_pData(pData)
{
  InitializeData();
  CentreOnParent();
}

void
CommitDlg::InitializeData ()
{
  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *middleSizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

  // The message field:
  wxStaticBoxSizer *messageSizer = new wxStaticBoxSizer(
    new wxStaticBox(this, -1, _T("Enter log message")), wxHORIZONTAL);
    
  wxTextCtrl* Log = new wxTextCtrl(this, -1, _T(""), wxDefaultPosition, 
    wxSize(-1, 50), wxTE_MULTILINE,
    wxTextValidator(wxFILTER_NONE, &m_pData->LogMessage));
    
  messageSizer->Add(Log, 1, wxALL | wxEXPAND, 5);
  
  topSizer->Add(messageSizer, 1, wxALL | wxEXPAND, 5);
  
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
  
  middleSizer->Add(authSizer, 1, wxALL | wxEXPAND, 5);

  // The "recursive" check box:
  wxCheckBox* pCheck = new wxCheckBox(this, -1, "Recursive",
    wxDefaultPosition, wxDefaultSize, 0,
    wxGenericValidator(&m_pData->Recursive));
    
  // The buttons:
  buttonSizer->Add(new wxButton( this, wxID_OK, _T("OK" )), 0, 
                   wxALL, 10);
  buttonSizer->Add(new wxButton( this, wxID_CANCEL, _T("Cancel")), 0, 
                   wxALL, 10);

  // Add all the sizers to the main sizer
  mainSizer->Add (topSizer, 1, wxLEFT | wxRIGHT | wxEXPAND, 5);
  mainSizer->Add (middleSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
  mainSizer->Add (pCheck, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
  mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

  SetAutoLayout(true);
  SetSizer(mainSizer);

  mainSizer->SetSizeHints(this);
  mainSizer->Fit(this);
}
