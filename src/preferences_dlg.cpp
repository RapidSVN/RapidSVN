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
#include "wx/confbase.h"
#include "wx/notebook.h"
#include "wx/valgen.h"
#include "wx/filedlg.h"
#include "wx/button.h"

// app
#include "ids.hpp"
#include "preferences_dlg.hpp"
#include "utils.hpp"

/****************************************************************************/

#ifdef _WIN32
static const wxChar *EXECUTABLE_WILDCARD = _("Executable Files|*.exe;*.com;*.bat|All files (*.*)|*.*");
#else
// NOTE: (WX 2.4.0) On Motif, neither multiple extensions nor multiple file
// types are supported (wildcard is limited to a single "*.ext" string)
static const wxChar *EXECUTABLE_WILDCARD = "";
#endif

/**
 * Configuration key names
 */
static const char STANDARD_EDITOR_KEY[] = "/Preferences/StandardEditor";
static const char STANDARD_EDITOR_ALWAYS_KEY[] = "/Preferences/AlwaysStandardEditor";
static const char STANDARD_FILE_EXPLORER_KEY[] = "/Preferences/StandardFileExplorer";
static const char STANDARD_FILE_EXPLORER_ALWAYS_KEY[] = "/Preferences/AlwaysStandardFileExplorer";
/*
static const char* szChoiceKey = "/Preferences/Choice";
*/

/* PreferencesDlg::SData ***************************************************/

PreferencesDlg::SData::SData ()
{
  // Default values which will be used if data could not be read from a
  // wxConfig object by Read ().
  m_standard_editor = "";
  m_standard_editor_always = false;

#ifdef _WIN32
  m_standard_file_explorer = _("explorer.exe");
#else
  m_standard_file_explorer = "";
#endif
  m_standard_file_explorer_always = false;

  /*
  Choice = 1;
  */
}  

void
PreferencesDlg::SData::Read (wxConfigBase* pConfig)
{
  if (pConfig)
  {
    // If these reads fail, the default values from the constructor remain
    m_standard_editor = pConfig->Read (STANDARD_EDITOR_KEY, m_standard_editor);
    pConfig->Read (STANDARD_EDITOR_ALWAYS_KEY, &m_standard_editor_always);
    
    m_standard_file_explorer = pConfig->Read (STANDARD_FILE_EXPLORER_KEY, m_standard_file_explorer);
    pConfig->Read (STANDARD_FILE_EXPLORER_ALWAYS_KEY, &m_standard_file_explorer_always);
    
    /*
    long l = Choice;
    pConfig->Read (szChoiceKey, (long *) &l);
    Choice = l;
    */
  }
}

void
PreferencesDlg::SData::Write (wxConfigBase* pConfig) const
{
  if (pConfig)
  {
    pConfig->Write (STANDARD_EDITOR_KEY, m_standard_editor);
    pConfig->Write (STANDARD_EDITOR_ALWAYS_KEY, m_standard_editor_always);

    pConfig->Write (STANDARD_FILE_EXPLORER_KEY, m_standard_file_explorer);
    pConfig->Write (STANDARD_FILE_EXPLORER_ALWAYS_KEY, m_standard_file_explorer_always);
    
    /*
    pConfig->Write (szEnableYKey, EnableY);
    pConfig->Write (szEnable1Key, Enable1);
    pConfig->Write (szEnable2Key, Enable2);
    pConfig->Write (szEnable3Key, Enable3);
    pConfig->Write (szChoiceKey, (long) Choice);  
    // int not supported - an oversight in wxConfig?
    */
  }
}

/* PreferencesDlg *********************************************************/
 
BEGIN_EVENT_TABLE (PreferencesDlg, wxDialog)
END_EVENT_TABLE ()

PreferencesDlg::SData PreferencesDlg::Data;

PreferencesDlg*
PreferencesDlg::CreateInstance (wxWindow* parent)
{
  PreferencesDlg* pInstance = new PreferencesDlg (parent);
  pInstance->InitializeData ();
  pInstance->CentreOnParent (); // Looks more natural.
  return pInstance;
}

PreferencesDlg::PreferencesDlg (wxWindow* parent)
  : wxDialog (parent, -1, _("Preferences"),
      wxDefaultPosition, wxDefaultSize,
      wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_pNB (NULL)
{
}

void
PreferencesDlg::InitializeData ()
{
  // Create the top-level controls in the dialog.
  // Each page will create its own controls.
 
  wxBoxSizer *topsizer = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *button_sizer = new wxBoxSizer (wxHORIZONTAL);

  button_sizer->Add (
    new wxButton (this, wxID_OK, "OK"),
    0, wxALL, 10);
  button_sizer->Add (
    new wxButton (this, wxID_CANCEL, "Cancel"),
    0, wxALL, 10);

  m_pNB = new wxNotebook (this, -1, wxDefaultPosition, wxDefaultSize); 
  wxNotebookSizer *nbs = new wxNotebookSizer (m_pNB);
  
  GeneralPanel *pGeneralPanel = GeneralPanel::Create (m_pNB);
  m_pNB->AddPage (pGeneralPanel, _("General"));
  ExternalsPanel *pExternalsPanel = ExternalsPanel::Create (m_pNB);
  m_pNB->AddPage (pExternalsPanel, _("Externals"));
  
  topsizer->Add (nbs, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 6);
  topsizer->Add (button_sizer, 0, wxALIGN_CENTER);

  // Order is important here:  
  SetSizer (topsizer);
  SetAutoLayout (true);  // Must precede call to Fit to work on Windows.
  topsizer->SetSizeHints (this); 
  topsizer->Fit (this);
}

bool
PreferencesDlg::TransferDataToWindow ()
{
  bool b = wxDialog::TransferDataToWindow ();
  // Transfer data for every page. This doesn't happen by default.
  if (m_pNB)
  {
    for (int i = 0; i < m_pNB->GetPageCount (); i++)
      b = b && m_pNB->GetPage (i)->TransferDataToWindow ();
  }
  return b;  
}

bool
PreferencesDlg::TransferDataFromWindow ()
{
  bool b = wxDialog::TransferDataFromWindow ();
  // Transfer data for every page. This doesn't happen by default.
  if (m_pNB)
  {
    for (int i = 0; i < m_pNB->GetPageCount (); i++)
      b = b && m_pNB->GetPage (i)->TransferDataFromWindow ();
  }
  return b;  
}

/* GeneralPanel *********************************************************/

BEGIN_EVENT_TABLE (GeneralPanel, wxPanel)
  EVT_BUTTON (ID_Preferences_GeneralPanel_StandardEditorLookup, 
    GeneralPanel::OnStandardEditorLookup)
  EVT_BUTTON (ID_Preferences_GeneralPanel_StandardFileExplorerLookup, 
    GeneralPanel::OnStandardFileExplorerLookup)
END_EVENT_TABLE ()

GeneralPanel*
GeneralPanel::Create (wxWindow* parent)
{
  GeneralPanel* p = new GeneralPanel (parent);
  p->InitializeData ();
  return p;
}

GeneralPanel::GeneralPanel (wxWindow* parent)
  : wxPanel (parent) 
{
}

void 
GeneralPanel::OnStandardEditorLookup (wxCommandEvent & event)
{
  wxFileDialog file_dialog (this, _("Select standard editor executable"));

  file_dialog.SetStyle (wxHIDE_READONLY | wxOPEN);
  file_dialog.SetWildcard (EXECUTABLE_WILDCARD);
  file_dialog.SetPath (m_standard_editor_textctrl->GetValue ());

  if (file_dialog.ShowModal () == wxID_OK)
    m_standard_editor_textctrl->SetValue (file_dialog.GetPath ());
}

void 
GeneralPanel::OnStandardFileExplorerLookup (wxCommandEvent & event)
{
  wxFileDialog file_dialog (this, _("Select standard file explorer executable"));

  file_dialog.SetStyle (wxHIDE_READONLY | wxOPEN);
  file_dialog.SetWildcard (EXECUTABLE_WILDCARD);
  file_dialog.SetPath (m_standard_file_explorer_textctrl->GetValue ());

  if (file_dialog.ShowModal () == wxID_OK)
    m_standard_file_explorer_textctrl->SetValue (file_dialog.GetPath ());
}

void
GeneralPanel::InitializeData ()
{
  wxBoxSizer *panelsizer = new wxBoxSizer (wxHORIZONTAL);

  // Left column
  wxBoxSizer *leftsizer = new wxBoxSizer (wxVERTICAL);
  panelsizer->Add (leftsizer, 1, wxALL | wxALIGN_TOP, 10);

  // Standard Editor
  wxBoxSizer *standard_editor_label_sizer = new wxBoxSizer (wxHORIZONTAL);
  leftsizer->Add (standard_editor_label_sizer, 1, wxEXPAND | wxALIGN_LEFT);
  standard_editor_label_sizer->Add (
    new wxStaticText (this, -1, _("Standard editor:")), 0, wxALIGN_CENTER);

  wxBoxSizer *standard_editor_sizer = new wxBoxSizer (wxHORIZONTAL);
  leftsizer->Add (standard_editor_sizer, 1, wxEXPAND | wxALIGN_LEFT);
  // TODO: File existence validation when the entire string has been entered
  m_standard_editor_textctrl = new wxTextCtrl (this, -1, "", wxDefaultPosition, 
    wxSize (200, -1), 0, 
    wxTextValidator (wxFILTER_NONE, &PreferencesDlg::Data.m_standard_editor));
  standard_editor_sizer->Add (m_standard_editor_textctrl, 1, wxALIGN_CENTER);
  
  standard_editor_sizer->Add (CreateEllipsisButton(this, 
    ID_Preferences_GeneralPanel_StandardEditorLookup), 0, wxALIGN_CENTER);

  // Use Standard Editor Always
  wxBoxSizer *standard_editor_always_sizer = new wxBoxSizer (wxHORIZONTAL);
  leftsizer->Add (standard_editor_always_sizer, 1, wxALIGN_LEFT);
  standard_editor_always_sizer->Add ( 
    new wxCheckBox (this, -1, _("Use always"), wxDefaultPosition, wxDefaultSize, 0, 
      wxGenericValidator (&PreferencesDlg::Data.m_standard_editor_always)), 
    0, wxALIGN_CENTER);

  // Standard File Explorer
  wxBoxSizer *standard_file_explorer_label_sizer = new wxBoxSizer (wxHORIZONTAL);
  leftsizer->Add (standard_file_explorer_label_sizer, 1, wxEXPAND | wxALIGN_LEFT);
  standard_file_explorer_label_sizer->Add (
    new wxStaticText (this, -1, _("Standard file explorer:")), 0, wxALIGN_CENTER);

  wxBoxSizer *standard_file_explorer_sizer = new wxBoxSizer (wxHORIZONTAL);
  leftsizer->Add (standard_file_explorer_sizer, 1, wxEXPAND | wxALIGN_LEFT);
  // TODO: File existence validation when the entire string has been entered
  m_standard_file_explorer_textctrl = new wxTextCtrl (this, -1, "", wxDefaultPosition, 
    wxSize (200, -1), 0, wxTextValidator (wxFILTER_NONE, &PreferencesDlg::Data.m_standard_file_explorer));
  standard_file_explorer_sizer->Add (m_standard_file_explorer_textctrl, 1, wxALL | wxALIGN_CENTER);
  
  standard_file_explorer_sizer->Add (CreateEllipsisButton(this, 
    ID_Preferences_GeneralPanel_StandardFileExplorerLookup), 0, wxALIGN_CENTER);

  // Use Standard File Explorer Always
  wxBoxSizer *standard_file_explorer_always_sizer = new wxBoxSizer (wxHORIZONTAL);
  leftsizer->Add (standard_file_explorer_always_sizer, 1, wxALIGN_LEFT);
  standard_file_explorer_always_sizer->Add ( 
    new wxCheckBox (
      this, -1, _("Use always"), wxDefaultPosition, wxDefaultSize, 0, 
      wxGenericValidator (&PreferencesDlg::Data.m_standard_file_explorer_always)
    ), 
    0, wxALIGN_CENTER
  );

  /*
  wxBoxSizer *rightsizer = new wxBoxSizer (wxVERTICAL);
  panelsizer->Add (rightsizer, 1, wxLEFT | wxALIGN_TOP, 10);
  
  wxBoxSizer *whichsizer = new wxBoxSizer (wxHORIZONTAL);
  rightsizer->Add (whichsizer, 1, wxALIGN_LEFT);
  whichsizer->Add (
    new wxStaticText (this, -1, _("Select one:")),
    0, wxALIGN_CENTER);
  whichsizer->Add (
    new wxRadioButton (this, -1, _("1"), wxDefaultPosition, wxDefaultSize, 0,
      wxGenericValidator (&PreferencesDlg::Data.Enable1)),
    0, wxALIGN_CENTER);
  whichsizer->Add (
    new wxRadioButton (this, -1, _("2"), wxDefaultPosition, wxDefaultSize, 0,
      wxGenericValidator (&PreferencesDlg::Data.Enable2)),
    0, wxALIGN_CENTER);
  whichsizer->Add (
    new wxRadioButton (this, -1, _("3"), wxDefaultPosition, wxDefaultSize, 0,
      wxGenericValidator (&PreferencesDlg::Data.Enable3)),
    0, wxALIGN_CENTER);
    
  wxBoxSizer *picksizer = new wxBoxSizer (wxHORIZONTAL);
  rightsizer->Add (picksizer, 1, wxALIGN_LEFT);
  picksizer->Add (
    new wxStaticText (this, -1, _("Pick one:")),
    0, wxALL | wxALIGN_CENTER, 5);
  static wxString s1 (_("First option"));
  static wxString s2 (_("Second option"));
  static wxString s3 (_("Third option"));
  static wxString ss[] = { s1, s2, s3 };
  
  picksizer->Add (
    new wxChoice (this, -1, wxDefaultPosition, wxDefaultSize, 3, ss, 0,
      wxGenericValidator (&PreferencesDlg::Data.Choice)),
    0, wxALL | wxALIGN_CENTER, 5);
  */
    
  SetSizer (panelsizer);
  SetAutoLayout (true);
}

/* ExternalsPanel *********************************************************/

BEGIN_EVENT_TABLE (ExternalsPanel, wxPanel)
END_EVENT_TABLE ()

ExternalsPanel*
ExternalsPanel::Create (wxWindow* parent)
{
  ExternalsPanel* p = new ExternalsPanel (parent);
  p->InitializeData ();
  return p;
}

ExternalsPanel::ExternalsPanel (wxWindow* parent)
  : wxPanel (parent) 
{
}

void
ExternalsPanel::InitializeData ()
{
  wxBoxSizer *panelsizer = new wxBoxSizer (wxVERTICAL);
  panelsizer->Add (
    new wxStaticText (this, -1, _("External settings go here.")),
    1, wxEXPAND | wxALL, 10);

  SetSizer (panelsizer);
  SetAutoLayout (TRUE);
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
