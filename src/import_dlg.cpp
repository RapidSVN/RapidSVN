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
#include "include.hpp"
#include "rapidsvn_app.hpp"
#include "import_dlg.hpp"
#include <wx/valgen.h>

enum
{
  ID_BUTTON_BROWSE = 100,
};

BEGIN_EVENT_TABLE (ImportDlg, wxDialog)
EVT_BUTTON (wxID_OK, ImportDlg::OnOk)
EVT_BUTTON (ID_BUTTON_BROWSE, ImportDlg::OnBrowse)
END_EVENT_TABLE ()

ImportDlg::sData::sData()
{
  // Default values go here:
  TreeType = true;
  FileType = !TreeType;
  Recursive = true;
}

ImportDlg::ImportDlg (wxWindow * parent, sData* pData)
           : wxDialog (parent, -1, "Import an unversioned file or tree", wxDefaultPosition, 
             wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
             m_pData(pData)
{
  InitializeData ();
  CentreOnParent();
}

void
ImportDlg::OnOk (wxCommandEvent &event)
{
  wxString val;

    // Transfer data from controls into m_pData:
  TransferDataFromWindow();

  TrimString (m_pData->Repository);
  TrimString (m_pData->Path);
  TrimString (m_pData->NewEntry);
  TrimString (m_pData->LogMessage);
  TrimString (m_pData->User);
  TrimString (m_pData->Password);
  
  if (m_pData->Repository.IsEmpty ())
  {
    wxMessageBox (_T ("Repository URL is required for import!"),
                  _T ("Error"), wxOK | wxCENTRE | wxICON_ERROR);

    // Do not allow the user to continue if the path is empty 
    // and the import is addressing a file.
    return;
  }

  if (m_pData->FileType)
  {

    if (m_pData->Path.IsEmpty ())
    {
      wxMessageBox (_T ("File path required when importing a file!"),
                    _T ("Error"), wxOK | wxCENTRE | wxICON_ERROR);

      // Do not allow the user to continue if the reposURL is empty.
      return;
    }

    if (m_pData->NewEntry.IsEmpty ())
    {
      wxMessageBox (_T ("New entry name required when importing a file!"),
                    _T ("Error"), wxOK | wxCENTRE | wxICON_ERROR);

      // Do not allow the user to continue if the new_entry is empty 
      // and the import is addressing a file.
      return;
    }
  }

  wxDialog::OnOK(event);
}

void
ImportDlg::OnBrowse (wxCommandEvent & WXUNUSED (event))
{
  // Transfer data from controls into m_pData:
  TransferDataFromWindow();
  
  if (m_pData->TreeType)
  {
    wxDirDialog dialog (this,
                        _T ("Select a directory to import"),
                        wxGetApp ().GetAppFrame ()->GetFolderBrowser ()->
                        GetPath ());

    if (dialog.ShowModal () == wxID_OK)
      m_pData->Path = dialog.GetPath ();
  }
  else                          // it's a file 
  {
    wxFileDialog dialog (this,
                         _T ("Select a file to import"),
                         wxGetApp ().GetAppFrame ()->GetFolderBrowser ()->
                         GetPath ());

    if (dialog.ShowModal () == wxID_OK)
      m_pData->Path = dialog.GetPath ();
  }
  // Transfer data from m_pData back into controls:
  TransferDataToWindow();
}

void
ImportDlg::InitializeData ()
{
  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

  wxFlexGridSizer* Grid = new wxFlexGridSizer(2, 3, 5, 0);
  Grid->AddGrowableCol(1);  // The second column can be expanded.

  // Row 0:  
  Grid->Add(new wxStaticText(this, -1, _T("Repository URL")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  wxTextCtrl *Repository = new wxTextCtrl(this, -1, _T(""),
    wxDefaultPosition, wxSize(300, -1), 0,
    wxTextValidator(wxFILTER_NONE, &m_pData->Repository));
  Grid->Add(Repository, 1, wxLEFT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);
  Grid->Add(new wxStaticText(this, -1, _T("")), 0, 
    0, 5);

  // Row 1:  
  Grid->Add(new wxStaticText(this, -1, _T("Path")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  wxTextCtrl *Path = new wxTextCtrl(this, -1, _T(""),
    wxDefaultPosition, wxSize(300, -1), 0,
    wxTextValidator(wxFILTER_NONE, &m_pData->Path));
  Grid->Add(Path, 1, wxLEFT | wxEXPAND, 5);
  wxButton* BrowseButton = new wxButton(this, ID_BUTTON_BROWSE, _T("..."), 
    wxPoint(-1,-1), wxSize(20, -1));
  Grid->Add(BrowseButton, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  
  // Row 2:  
  Grid->Add(new wxStaticText(this, -1, _T("New entry")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  wxTextCtrl *NewEntry = new wxTextCtrl(this, -1, _T(""),
    wxDefaultPosition, wxSize(300, -1), 0,
    wxTextValidator(wxFILTER_NONE, &m_pData->NewEntry));
  Grid->Add(NewEntry, 1, wxLEFT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);
  Grid->Add(new wxStaticText(this, -1, _T("")), 0, 
    0, 5);

  mainSizer->Add(Grid, 0, wxALL | wxEXPAND, 5);

  
  // The message field:
  wxStaticBoxSizer *messageSizer = new wxStaticBoxSizer(
    new wxStaticBox(this, -1, _T("Enter log message")), wxHORIZONTAL);
    
  wxTextCtrl* Log = new wxTextCtrl(this, -1, _T(""), wxDefaultPosition, 
    wxSize(-1, 50), wxTE_MULTILINE,
    wxTextValidator(wxFILTER_NONE, &m_pData->LogMessage));
    
  messageSizer->Add(Log, 1, wxALL | wxEXPAND, 5);
  
  mainSizer->Add(messageSizer, 1, wxALL | wxEXPAND, 5);

  // Sundry items row:
  wxBoxSizer *SundrySizer = new wxBoxSizer(wxHORIZONTAL);
  
  wxCheckBox* Recursive = new wxCheckBox (this, -1, _T("Recursive"),
    wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&m_pData->Recursive));
  SundrySizer->Add(Recursive, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
  
  SundrySizer->Add(new wxStaticText(this, -1, _T("Path type:")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 30);

  SundrySizer->Add(
    new wxRadioButton(this, -1, _T("Tree"), wxDefaultPosition, wxDefaultSize, 0,
      wxGenericValidator(&m_pData->TreeType)),
    0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
  SundrySizer->Add(
    new wxRadioButton(this, -1, _T("File"), wxDefaultPosition, wxDefaultSize, 0,
      wxGenericValidator(&m_pData->FileType)),
    0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

  mainSizer->Add(SundrySizer, 0, wxALL | wxCENTER, 5);

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

  mainSizer->Add(AuthSizer, 0, wxALL | wxEXPAND, 5);

  // Button row
  wxBoxSizer *ButtonSizer = new wxBoxSizer(wxHORIZONTAL);
  ButtonSizer->Add(new wxButton(this, wxID_OK, _T("OK" )), 0, 
    wxALL, 10);
  ButtonSizer->Add(new wxButton(this, wxID_CANCEL, _T("Cancel")), 0, 
    wxALL, 10);

  mainSizer->Add(ButtonSizer, 0, wxALL | wxCENTER, 5);

  SetAutoLayout(true);
  SetSizer(mainSizer);

  mainSizer->SetSizeHints(this);
  mainSizer->Fit(this);
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
