#include "include.h"
#include "preferences_dlg.h"

#include <wx/notebook.h>
#include <wx/panel.h>
#include <wx/resource.h>
#include <wx/valgen.h>

/* PreferencesDlg::SData ***************************************************/

PreferencesDlg::SData::SData()
{
  // Default values which will be used if data could not be 
  // read from a wxConfig object by Read().
  Text = _T("Default text");
  EnableX = true;
  EnableY = false;
  Enable1 = Enable2 = false;
  Enable3 = true;
  Choice = 1;
}  

// Config file key for preference data:
static const wxString ConfigPath("/Preferences");

void
PreferencesDlg::SData::Read(wxConfigBase* pConfig)
{
  if (pConfig)
  {
    pConfig->SetPath(ConfigPath);
    // If these reads fail, the default values from the 
    // constructor remain:
    Text = pConfig->Read("Text", Text);
    pConfig->Read("EnableX", &EnableX);
    pConfig->Read("EnableY", &EnableY);
    pConfig->Read("Enable1", &Enable1);
    pConfig->Read("Enable2", &Enable2);
    pConfig->Read("Enable3", &Enable3);
    long l = Choice;
    pConfig->Read("Choice", (long *) &l);
    Choice = l;
  }
}

void
PreferencesDlg::SData::Write(wxConfigBase* pConfig) const
{
  if (pConfig)
  {
    pConfig->SetPath(ConfigPath);
    pConfig->Write("Text", Text);
    pConfig->Write("EnableX", EnableX);
    pConfig->Write("EnableY", EnableY);
    pConfig->Write("Enable1", Enable1);
    pConfig->Write("Enable2", Enable2);
    pConfig->Write("Enable3", Enable3);
    pConfig->Write("Choice", (long) Choice);  
      // int not supported - an oversight in wxConfig?
  }
}

/* PreferencesDlg *********************************************************/
 
BEGIN_EVENT_TABLE (PreferencesDlg, wxDialog)
END_EVENT_TABLE ()

PreferencesDlg::SData PreferencesDlg::Data;

PreferencesDlg*
PreferencesDlg::CreateInstance(wxWindow* parent)
{
  PreferencesDlg* pInstance = new PreferencesDlg(parent);
  pInstance->InitializeData();
  pInstance->CentreOnParent(); // Looks more natural.
  return pInstance;
}

PreferencesDlg::PreferencesDlg(wxWindow* parent)
  : wxDialog(parent, -1, _T("Preferences"),
      wxDefaultPosition, wxDefaultSize,
      wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_pNB(NULL)
{
}

void
PreferencesDlg::InitializeData ()
{
  // Create the top-level controls in the dialog.
  // Each page will create its own controls.
 
  wxBoxSizer *topsizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *button_sizer = new wxBoxSizer(wxHORIZONTAL);

  button_sizer->Add(
    new wxButton(this, wxID_OK, "OK"),
    0, wxALL, 10);
  button_sizer->Add(
    new wxButton(this, wxID_CANCEL, "Cancel"),
    0, wxALL, 10);

  m_pNB = new wxNotebook(this, -1, wxDefaultPosition, wxDefaultSize); 
  wxNotebookSizer *nbs = new wxNotebookSizer(m_pNB);
  
  GeneralPanel *pGeneralPanel = GeneralPanel::Create(m_pNB);
  m_pNB->AddPage(pGeneralPanel, _T("General"));
  ExternalsPanel *pExternalsPanel = ExternalsPanel::Create(m_pNB);
  m_pNB->AddPage(pExternalsPanel, _T("Externals"));
  
  topsizer->Add(nbs, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 10);
  topsizer->Add(button_sizer, 0, wxALIGN_CENTER);

// Order is important here:  
  SetSizer(topsizer);
  SetAutoLayout(true);  // Must precede call to Fit to work on Windows.
  topsizer->SetSizeHints(this); 
  topsizer->Fit(this);
}

bool
PreferencesDlg::TransferDataToWindow()
{
  bool b = wxDialog::TransferDataToWindow();
  // Transfer data for every page. This doesn't happen by default.
  if (m_pNB)
  {
    for (int i = 0; i < m_pNB->GetPageCount(); i++)
      b = b && m_pNB->GetPage(i)->TransferDataToWindow();
  }
  return b;  
}

bool
PreferencesDlg::TransferDataFromWindow()
{
  bool b = wxDialog::TransferDataFromWindow();
  // Transfer data for every page. This doesn't happen by default.
  if (m_pNB)
  {
    for (int i = 0; i < m_pNB->GetPageCount(); i++)
      b = b && m_pNB->GetPage(i)->TransferDataFromWindow();
  }
  return b;  
}

/* GeneralPanel *********************************************************/

BEGIN_EVENT_TABLE (GeneralPanel, wxPanel)
END_EVENT_TABLE ()

GeneralPanel*
GeneralPanel::Create(wxWindow* parent)
{
  GeneralPanel* p = new GeneralPanel(parent);
  p->InitializeData();
  return p;
}

GeneralPanel::GeneralPanel(wxWindow* parent)
  : wxPanel(parent) 
{
}

void
GeneralPanel::InitializeData()
{
  wxBoxSizer *panelsizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *leftsizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *rightsizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *textsizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *enable1sizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *enable2sizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *whichsizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *picksizer = new wxBoxSizer(wxHORIZONTAL);
  
  panelsizer->Add(leftsizer, 1, wxALL | wxALIGN_TOP, 0);
  panelsizer->Add(rightsizer, 1, wxLEFT | wxALIGN_TOP, 20);
  
  leftsizer->Add(textsizer, 1, wxEXPAND | wxALIGN_LEFT);
  leftsizer->Add(enable1sizer, 1, wxALIGN_LEFT);
  leftsizer->Add(enable2sizer, 1, wxALIGN_LEFT);
  leftsizer->Add(
    new wxStaticText(this, -1,
      _T("(Replace with real preferences in due course)")),
      0, wxALL | wxALIGN_LEFT, 5);
    
  rightsizer->Add(whichsizer, 1, wxALIGN_LEFT);
  rightsizer->Add(picksizer, 1, wxALIGN_LEFT);

  textsizer->Add(
    new wxStaticText(this, -1, _T("Enter some text:")),
    0, wxALL | wxALIGN_CENTER, 5);
  textsizer->Add(
    new wxTextCtrl(this, -1, _T(""), wxDefaultPosition, wxSize(100, -1), 0,
      wxTextValidator(wxFILTER_ALPHA, &PreferencesDlg::Data.Text)),
    1, wxALL | wxALIGN_CENTER, 5);
    
  enable1sizer->Add(
    new wxCheckBox(this, -1, _T("Enable feature X"),
      wxDefaultPosition, wxDefaultSize, 0,
      wxGenericValidator(&PreferencesDlg::Data.EnableX)),
    0, wxALL | wxALIGN_CENTER, 5);
    
  enable2sizer->Add(
    new wxCheckBox(this, -1, _T("Enable feature Y"), 
      wxDefaultPosition, wxDefaultSize, 0,
      wxGenericValidator(&PreferencesDlg::Data.EnableY)),
    0, wxALL | wxALIGN_CENTER, 5);
  
  whichsizer->Add(
    new wxStaticText(this, -1, _T("Select one:")),
    0, wxALL | wxALIGN_CENTER, 5);
  whichsizer->Add(
    new wxRadioButton(this, -1, _T("1"), wxDefaultPosition, wxDefaultSize, 0,
      wxGenericValidator(&PreferencesDlg::Data.Enable1)),
    0, wxALL | wxALIGN_CENTER, 5);
  whichsizer->Add(
    new wxRadioButton(this, -1, _T("2"), wxDefaultPosition, wxDefaultSize, 0,
      wxGenericValidator(&PreferencesDlg::Data.Enable2)),
    0, wxALL | wxALIGN_CENTER, 5);
  whichsizer->Add(
    new wxRadioButton(this, -1, _T("3"), wxDefaultPosition, wxDefaultSize, 0,
      wxGenericValidator(&PreferencesDlg::Data.Enable3)),
    0, wxALL | wxALIGN_CENTER, 5);
    
  picksizer->Add(
    new wxStaticText(this, -1, _T("Pick one:")),
    0, wxALL | wxALIGN_CENTER, 5);
  static wxString s1(_T("First option"));
  static wxString s2(_T("Second option"));
  static wxString s3(_T("Third option"));
  static wxString ss[] = { s1, s2, s3 };
  
  picksizer->Add(
    new wxChoice(this, -1, wxDefaultPosition, wxDefaultSize, 3, ss, 0,
      wxGenericValidator(&PreferencesDlg::Data.Choice)),
    0, wxALL | wxALIGN_CENTER, 5);
    
  SetSizer(panelsizer);
  SetAutoLayout(true);
}

/* ExternalsPanel *********************************************************/

BEGIN_EVENT_TABLE (ExternalsPanel, wxPanel)
END_EVENT_TABLE ()

ExternalsPanel*
ExternalsPanel::Create(wxWindow* parent)
{
  ExternalsPanel* p = new ExternalsPanel(parent);
  p->InitializeData();
  return p;
}

ExternalsPanel::ExternalsPanel(wxWindow* parent)
  : wxPanel(parent) 
{
}

void
ExternalsPanel::InitializeData()
{
  wxBoxSizer *panelsizer = new wxBoxSizer(wxVERTICAL);
  panelsizer->Add(
    new wxStaticText (this, -1, _T("External settings go here.")),
    1, wxEXPAND | wxALL, 10);

  SetSizer(panelsizer);
  SetAutoLayout(TRUE);
}
