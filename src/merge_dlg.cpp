#include "include.h"
#include "rapidsvn_app.h"
#include "merge_dlg.h"
#include <wx/valgen.h>

BEGIN_EVENT_TABLE (MergeDlg, wxDialog)
EVT_BUTTON (wxID_OK, MergeDlg::OnOK)
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
  TrimString(m_pData->Path1Rev1);
  TrimString(m_pData->Path1Rev2);
  TrimString(m_pData->Path2);
  TrimString(m_pData->Path2Rev);
  
  // test revision numbers
  if (TestRev (m_pData->Path1Rev1) < 0)
    return;

  if (TestRev (m_pData->Path1Rev2) < 0)
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

  if (m_pData->Path1Rev1.IsEmpty () && !m_pData->Path1Rev2.IsEmpty ())
  {
    wxMessageBox (_T
                  ("You should also specify start revision for first path!"),
                  _T ("Error"), wxOK | wxCENTRE | wxICON_ERROR);

    // Do not allow the user to continue if the start revision is not set, 
    // but the end revision is.
    return;
  }

  // If start revision for first path is emtpy... 
  // (the end revision should be empty also in this case - checked above)
  if (m_pData->Path1Rev1.IsEmpty ())
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
    
  wxFlexGridSizer* Grid = new wxFlexGridSizer(4, 3, 0, 0);
  Grid->AddGrowableCol(0);  // The first column can be expanded.

  // Row 0:  
  Grid->Add(new wxStaticText(this, -1, _T("First working copy or URL")), 0, 
    0, 5);
  Grid->Add(new wxStaticText(this, -1, _T("Start revision")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 20);
  Grid->Add(new wxStaticText(this, -1, _T("End revision")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 20);
    
  // Row 1:  
  wxTextCtrl *Path1 = new wxTextCtrl(this, -1, _T(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NONE, &m_pData->Path1));
  Grid->Add(Path1, 1, wxBOTTOM | wxEXPAND, 20);
  
  wxTextCtrl *Path1Rev1 = new wxTextCtrl(this, -1, _T(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NUMERIC, &m_pData->Path1Rev1));
  Grid->Add(Path1Rev1, 0, wxLEFT, 20);
    
  wxTextCtrl *Path1Rev2 = new wxTextCtrl(this, -1, _T(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NUMERIC, &m_pData->Path1Rev2));
  Grid->Add(Path1Rev2, 0, wxLEFT, 20);
    
  // Row 2:
  Grid->Add(new wxStaticText(this, -1, _T("Second working copy or URL")), 0, 
    0, 5);
  Grid->Add(new wxStaticText(this, -1, _T("Revision")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 20);
  Grid->Add(new wxStaticText(this, -1, _T("")), 0, 
    wxLEFT | wxALIGN_CENTER_VERTICAL, 20);

  // Row 3:  
  wxTextCtrl *Path2 = new wxTextCtrl(this, -1, _T(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NONE, &m_pData->Path2));
  Grid->Add(Path2, 1, wxBOTTOM | wxEXPAND, 5);
  
  wxTextCtrl *Path2Rev = new wxTextCtrl(this, -1, _T(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NUMERIC, &m_pData->Path2Rev));
  Grid->Add(Path2Rev, 0, wxLEFT, 20);

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
