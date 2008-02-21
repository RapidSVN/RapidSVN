///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb  1 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>

#include "rapidsvn_generated.h"

///////////////////////////////////////////////////////////////////////////

PreferencesDlgBase::PreferencesDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_notebookMain = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelGeneral = new wxPanel( m_notebookMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer3;
	bSizer3 = new wxBoxSizer( wxVERTICAL );
	
	m_checkPurgeTempFiles = new wxCheckBox( m_panelGeneral, wxID_ANY, _("Purge temporary files on program exit"), wxDefaultPosition, wxDefaultSize, 0 );
	
	bSizer3->Add( m_checkPurgeTempFiles, 0, wxALL, 5 );
	
	m_checkUseLastCommitMessage = new wxCheckBox( m_panelGeneral, wxID_ANY, _("Commit log message: default to most recent"), wxDefaultPosition, wxDefaultSize, 0 );
	
	bSizer3->Add( m_checkUseLastCommitMessage, 0, wxALL, 5 );
	
	m_checkResetFlatMode = new wxCheckBox( m_panelGeneral, wxID_ANY, _("Reset Flat Mode on every program start"), wxDefaultPosition, wxDefaultSize, 0 );
	
	bSizer3->Add( m_checkResetFlatMode, 0, wxALL, 5 );
	
	m_panelGeneral->SetSizer( bSizer3 );
	m_panelGeneral->Layout();
	bSizer3->Fit( m_panelGeneral );
	m_notebookMain->AddPage( m_panelGeneral, _("General"), true );
	m_panelPrograms = new wxPanel( m_notebookMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* bSizer4;
	bSizer4 = new wxBoxSizer( wxVERTICAL );
	
	m_notebook2 = new wxNotebook( m_panelPrograms, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelEditor = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizerPanelEditor;
	sizerPanelEditor = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* sizerEditor;
	sizerEditor = new wxBoxSizer( wxHORIZONTAL );
	
	m_textEditor = new wxTextCtrl( m_panelEditor, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditor->Add( m_textEditor, 1, wxALL, 5 );
	
	m_buttonEditor = new wxButton( m_panelEditor, wxID_ANY, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerEditor->Add( m_buttonEditor, 0, wxALL, 5 );
	
	sizerPanelEditor->Add( sizerEditor, 0, wxEXPAND, 5 );
	
	wxStaticText* m_staticEditorArgs;
	m_staticEditorArgs = new wxStaticText( m_panelEditor, wxID_ANY, _("Program arguments (%1=selected file):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticEditorArgs->Wrap( -1 );
	sizerPanelEditor->Add( m_staticEditorArgs, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_textEditorArgs = new wxTextCtrl( m_panelEditor, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerPanelEditor->Add( m_textEditorArgs, 0, wxALL|wxEXPAND, 5 );
	
	m_checkEditorAlways = new wxCheckBox( m_panelEditor, wxID_ANY, _("Use always"), wxDefaultPosition, wxDefaultSize, 0 );
	
	sizerPanelEditor->Add( m_checkEditorAlways, 0, wxALL, 5 );
	
	m_panelEditor->SetSizer( sizerPanelEditor );
	m_panelEditor->Layout();
	sizerPanelEditor->Fit( m_panelEditor );
	m_notebook2->AddPage( m_panelEditor, _("Standard Editor"), true );
	m_panelExplorer = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizerPanelExplorer;
	sizerPanelExplorer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* sizerExplorer;
	sizerExplorer = new wxBoxSizer( wxHORIZONTAL );
	
	m_textExplorer = new wxTextCtrl( m_panelExplorer, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerExplorer->Add( m_textExplorer, 1, wxALL, 5 );
	
	m_buttonExplorer = new wxButton( m_panelExplorer, wxID_ANY, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerExplorer->Add( m_buttonExplorer, 0, wxALL, 5 );
	
	sizerPanelExplorer->Add( sizerExplorer, 0, wxEXPAND, 0 );
	
	wxStaticText* m_staticExplorerArgs;
	m_staticExplorerArgs = new wxStaticText( m_panelExplorer, wxID_ANY, _("Program arguments (%1=selected directory):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticExplorerArgs->Wrap( -1 );
	sizerPanelExplorer->Add( m_staticExplorerArgs, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_textExplorerArgs = new wxTextCtrl( m_panelExplorer, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerPanelExplorer->Add( m_textExplorerArgs, 0, wxALL|wxEXPAND, 5 );
	
	m_checkExplorerAlways = new wxCheckBox( m_panelExplorer, wxID_ANY, _("Use always"), wxDefaultPosition, wxDefaultSize, 0 );
	
	sizerPanelExplorer->Add( m_checkExplorerAlways, 0, wxALL, 5 );
	
	m_panelExplorer->SetSizer( sizerPanelExplorer );
	m_panelExplorer->Layout();
	sizerPanelExplorer->Fit( m_panelExplorer );
	m_notebook2->AddPage( m_panelExplorer, _("Standard Explorer"), false );
	m_panelDiffTool = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizerPanelDiffTool;
	sizerPanelDiffTool = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* sizerDiffTool;
	sizerDiffTool = new wxBoxSizer( wxHORIZONTAL );
	
	m_textDiffTool = new wxTextCtrl( m_panelDiffTool, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerDiffTool->Add( m_textDiffTool, 1, wxALL, 5 );
	
	m_buttonDiffTool = new wxButton( m_panelDiffTool, wxID_ANY, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerDiffTool->Add( m_buttonDiffTool, 0, wxALL, 5 );
	
	sizerPanelDiffTool->Add( sizerDiffTool, 0, wxEXPAND, 5 );
	
	wxStaticText* m_staticDiffToolArgs;
	m_staticDiffToolArgs = new wxStaticText( m_panelDiffTool, wxID_ANY, _("Program arguments (%1=file1, %2=file2):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticDiffToolArgs->Wrap( -1 );
	sizerPanelDiffTool->Add( m_staticDiffToolArgs, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_textDiffToolArgs = new wxTextCtrl( m_panelDiffTool, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerPanelDiffTool->Add( m_textDiffToolArgs, 0, wxALL|wxEXPAND, 5 );
	
	m_panelDiffTool->SetSizer( sizerPanelDiffTool );
	m_panelDiffTool->Layout();
	sizerPanelDiffTool->Fit( m_panelDiffTool );
	m_notebook2->AddPage( m_panelDiffTool, _("Diff Tool"), false );
	m_panelMergeTool = new wxPanel( m_notebook2, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizerPanelMergeTool;
	sizerPanelMergeTool = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* sizerMergeTool;
	sizerMergeTool = new wxBoxSizer( wxHORIZONTAL );
	
	m_textMergeTool = new wxTextCtrl( m_panelMergeTool, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerMergeTool->Add( m_textMergeTool, 1, wxALL, 5 );
	
	m_buttonMergeTool = new wxButton( m_panelMergeTool, wxID_ANY, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerMergeTool->Add( m_buttonMergeTool, 0, wxALL, 5 );
	
	sizerPanelMergeTool->Add( sizerMergeTool, 0, wxEXPAND, 5 );
	
	m_staticMergeToolArgs = new wxStaticText( m_panelMergeTool, wxID_ANY, _("Program arguments (%1=base, %2=theirs %3=mine %4=result):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticMergeToolArgs->Wrap( -1 );
	sizerPanelMergeTool->Add( m_staticMergeToolArgs, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_textMergeToolArgs = new wxTextCtrl( m_panelMergeTool, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	sizerPanelMergeTool->Add( m_textMergeToolArgs, 0, wxALL|wxEXPAND, 5 );
	
	m_panelMergeTool->SetSizer( sizerPanelMergeTool );
	m_panelMergeTool->Layout();
	sizerPanelMergeTool->Fit( m_panelMergeTool );
	m_notebook2->AddPage( m_panelMergeTool, _("Merge Tool"), false );
	
	bSizer4->Add( m_notebook2, 1, wxEXPAND | wxALL, 5 );
	
	m_panelPrograms->SetSizer( bSizer4 );
	m_panelPrograms->Layout();
	bSizer4->Fit( m_panelPrograms );
	m_notebookMain->AddPage( m_panelPrograms, _("Programs"), false );
	m_panelAuth = new wxPanel( m_notebookMain, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizerAuth;
	sizerAuth = new wxBoxSizer( wxVERTICAL );
	
	m_checkAuthPerBookmark = new wxCheckBox( m_panelAuth, wxID_ANY, _("Different login for each bookmark in the bookmarks list"), wxDefaultPosition, wxDefaultSize, 0 );
	
	sizerAuth->Add( m_checkAuthPerBookmark, 0, wxALL, 5 );
	
	m_checkUseAuthCache = new wxCheckBox( m_panelAuth, wxID_ANY, _("Store authentication credentials on hard disk"), wxDefaultPosition, wxDefaultSize, 0 );
	
	sizerAuth->Add( m_checkUseAuthCache, 0, wxALL, 5 );
	
	m_panelAuth->SetSizer( sizerAuth );
	m_panelAuth->Layout();
	sizerAuth->Fit( m_panelAuth );
	m_notebookMain->AddPage( m_panelAuth, _("Authentication"), false );
	
	mainSizer->Add( m_notebookMain, 1, wxEXPAND | wxALL, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOK->SetDefault(); 
	buttonSizer->Add( m_buttonOK, 0, wxALL, 10 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( m_buttonCancel, 0, wxALL, 10 );
	
	mainSizer->Add( buttonSizer, 0, wxALIGN_CENTER, 0 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );
	
	// Connect Events
	m_buttonEditor->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnButtonEditorClick ), NULL, this );
	m_buttonExplorer->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnButtonExplorerClick ), NULL, this );
	m_buttonDiffTool->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnButtonDiffToolClick ), NULL, this );
	m_buttonMergeTool->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnButtonEditorClick ), NULL, this );
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnOK ), NULL, this );
}

PreferencesDlgBase::~PreferencesDlgBase()
{
	// Disconnect Events
	m_buttonEditor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnButtonEditorClick ), NULL, this );
	m_buttonExplorer->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnButtonExplorerClick ), NULL, this );
	m_buttonDiffTool->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnButtonDiffToolClick ), NULL, this );
	m_buttonMergeTool->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnButtonEditorClick ), NULL, this );
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnOK ), NULL, this );
}
