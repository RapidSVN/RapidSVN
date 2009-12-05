///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>

#include "rapidsvn_generated.h"

///////////////////////////////////////////////////////////////////////////

AboutDlgBase::AboutDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_mainSizer = new wxBoxSizer( wxVERTICAL );
	
	topSizer = new wxFlexGridSizer( 2, 2, 10, 10 );
	topSizer->SetFlexibleDirection( wxBOTH );
	topSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_bitmapLogo = new wxStaticBitmap( this, wxID_ANY, wxNullBitmap, wxDefaultPosition, wxDefaultSize, 0 );
	topSizer->Add( m_bitmapLogo, 0, wxALL, 5 );
	
	m_staticCopy = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticCopy->Wrap( -1 );
	topSizer->Add( m_staticCopy, 1, wxALL|wxEXPAND, 5 );
	
	m_staticBuilt = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticBuilt->Wrap( -1 );
	topSizer->Add( m_staticBuilt, 0, wxALL, 5 );
	
	m_staticInfo = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticInfo->Wrap( -1 );
	topSizer->Add( m_staticInfo, 0, wxALL, 5 );
	
	m_mainSizer->Add( topSizer, 0, wxEXPAND, 5 );
	
	m_buttonOK = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOK->SetDefault(); 
	m_mainSizer->Add( m_buttonOK, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	this->SetSizer( m_mainSizer );
	this->Layout();
	m_mainSizer->Fit( this );
}

AboutDlgBase::~AboutDlgBase()
{
}

AnnotateDlgBase::AnnotateDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_list = new wxListCtrl( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	m_mainSizer->Add( m_list, 1, wxALL|wxEXPAND, 5 );
	
	m_buttonOK = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOK->SetDefault(); 
	m_mainSizer->Add( m_buttonOK, 0, wxALIGN_CENTER|wxALL, 5 );
	
	this->SetSizer( m_mainSizer );
	this->Layout();
}

AnnotateDlgBase::~AnnotateDlgBase()
{
}

AuthDlgBase::AuthDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_mainSizer = new wxBoxSizer( wxVERTICAL );
	
	authSizer = new wxFlexGridSizer( 2, 2, 5, 5 );
	authSizer->AddGrowableCol( 1 );
	authSizer->SetFlexibleDirection( wxHORIZONTAL );
	authSizer->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_staticUsername = new wxStaticText( this, wxID_ANY, _("&Username"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticUsername->Wrap( -1 );
	authSizer->Add( m_staticUsername, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textUsername = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	authSizer->Add( m_textUsername, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	m_staticPassword = new wxStaticText( this, wxID_ANY, _("&Password"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticPassword->Wrap( -1 );
	authSizer->Add( m_staticPassword, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_textPassword = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD );
	authSizer->Add( m_textPassword, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	m_mainSizer->Add( authSizer, 0, wxEXPAND, 5 );
	
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOK->SetDefault(); 
	buttonSizer->Add( m_buttonOK, 0, wxALL, 10 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( m_buttonCancel, 0, wxALL, 10 );
	
	m_mainSizer->Add( buttonSizer, 0, wxALIGN_CENTER, 5 );
	
	this->SetSizer( m_mainSizer );
	this->Layout();
	m_mainSizer->Fit( this );
}

AuthDlgBase::~AuthDlgBase()
{
}

CertDlgBase::CertDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_staticTitle = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticTitle->Wrap( -1 );
	m_mainSizer->Add( m_staticTitle, 0, wxALL, 5 );
	
	m_sizerFailures = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("MyLabel") ), wxVERTICAL );
	
	m_staticFailures = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticFailures->Wrap( -1 );
	m_sizerFailures->Add( m_staticFailures, 0, wxALL, 5 );
	
	m_mainSizer->Add( m_sizerFailures, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* sizerCert;
	sizerCert = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Certificate information:") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizerFailures;
	fgSizerFailures = new wxFlexGridSizer( 2, 2, 0, 0 );
	fgSizerFailures->SetFlexibleDirection( wxBOTH );
	fgSizerFailures->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_labelHostname = new wxStaticText( this, wxID_ANY, _("Hostname:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labelHostname->Wrap( -1 );
	fgSizerFailures->Add( m_labelHostname, 0, wxALL, 5 );
	
	m_staticHostname = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticHostname->Wrap( -1 );
	fgSizerFailures->Add( m_staticHostname, 0, wxALL, 5 );
	
	m_labelIssuer = new wxStaticText( this, wxID_ANY, _("Issuer:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labelIssuer->Wrap( -1 );
	fgSizerFailures->Add( m_labelIssuer, 0, wxALL, 5 );
	
	m_staticIssuer = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticIssuer->Wrap( -1 );
	fgSizerFailures->Add( m_staticIssuer, 0, wxALL, 5 );
	
	m_labelValidFrom = new wxStaticText( this, wxID_ANY, _("Valid from:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labelValidFrom->Wrap( -1 );
	fgSizerFailures->Add( m_labelValidFrom, 0, wxALL, 5 );
	
	m_staticValidFrom = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticValidFrom->Wrap( -1 );
	fgSizerFailures->Add( m_staticValidFrom, 0, wxALL, 5 );
	
	m_labelValidUntil = new wxStaticText( this, wxID_ANY, _("Valid until:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labelValidUntil->Wrap( -1 );
	fgSizerFailures->Add( m_labelValidUntil, 0, wxALL, 5 );
	
	m_staticValidUntil = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticValidUntil->Wrap( -1 );
	fgSizerFailures->Add( m_staticValidUntil, 0, wxALL, 5 );
	
	m_labelFingerprint = new wxStaticText( this, wxID_ANY, _("Fingerprint:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labelFingerprint->Wrap( -1 );
	fgSizerFailures->Add( m_labelFingerprint, 0, wxALL, 5 );
	
	m_staticFingerprint = new wxStaticText( this, wxID_ANY, _("MyLabel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticFingerprint->Wrap( -1 );
	fgSizerFailures->Add( m_staticFingerprint, 0, wxALL, 5 );
	
	sizerCert->Add( fgSizerFailures, 1, wxEXPAND, 5 );
	
	m_mainSizer->Add( sizerCert, 1, wxEXPAND, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonPerm = new wxButton( this, wxID_ANY, _("&Permanently"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( m_buttonPerm, 0, wxALL, 5 );
	
	m_buttonTemp = new wxButton( this, wxID_ANY, _("&Temporarily"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( m_buttonTemp, 0, wxALL, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonCancel->SetDefault(); 
	buttonSizer->Add( m_buttonCancel, 0, wxALL, 5 );
	
	m_mainSizer->Add( buttonSizer, 0, wxALIGN_CENTER, 5 );
	
	this->SetSizer( m_mainSizer );
	this->Layout();
	
	// Connect Events
	m_buttonPerm->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CertDlgBase::OnPerm ), NULL, this );
	m_buttonTemp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CertDlgBase::OnTemp ), NULL, this );
}

CertDlgBase::~CertDlgBase()
{
	// Disconnect Events
	m_buttonPerm->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CertDlgBase::OnPerm ), NULL, this );
	m_buttonTemp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CertDlgBase::OnTemp ), NULL, this );
}

CheckoutDlgBase::CheckoutDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	this->SetExtraStyle( wxDIALOG_EX_CONTEXTHELP );
	
	m_mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxStaticBoxSizer* urlSizer;
	urlSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("URL") ), wxVERTICAL );
	
	m_comboUrl = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 235,-1 ), 0, NULL, 0 ); 
	urlSizer->Add( m_comboUrl, 0, wxALL|wxEXPAND, 5 );
	
	m_mainSizer->Add( urlSizer, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* destSizer;
	destSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Destination Directory") ), wxHORIZONTAL );
	
	m_comboDest = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 205,-1 ), 0, NULL, 0 ); 
	destSizer->Add( m_comboDest, 1, wxALL, 5 );
	
	m_buttonBrowse = new wxButton( this, wxID_ANY, _("..."), wxDefaultPosition, wxSize( 20,-1 ), 0 );
	destSizer->Add( m_buttonBrowse, 0, wxALL, 5 );
	
	m_mainSizer->Add( destSizer, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* revisionSizer;
	revisionSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Revision") ), wxHORIZONTAL );
	
	m_textRevision = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	revisionSizer->Add( m_textRevision, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_checkUseLatest = new wxCheckBox( this, wxID_ANY, _("Use latest"), wxDefaultPosition, wxDefaultSize, 0 );
	
	revisionSizer->Add( m_checkUseLatest, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_mainSizer->Add( revisionSizer, 0, wxEXPAND, 5 );
	
	wxStaticBoxSizer* pegSizer;
	pegSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Peg Revision") ), wxHORIZONTAL );
	
	m_textPegRevision = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 50,-1 ), 0 );
	pegSizer->Add( m_textPegRevision, 1, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_checkPegNotSpecified = new wxCheckBox( this, wxID_ANY, _("Not specified"), wxDefaultPosition, wxDefaultSize, 0 );
	
	pegSizer->Add( m_checkPegNotSpecified, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_mainSizer->Add( pegSizer, 0, wxEXPAND, 5 );
	
	wxBoxSizer* extrasSizer;
	extrasSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkAddToBookmarks = new wxCheckBox( this, wxID_ANY, _("Add to bookmarks"), wxDefaultPosition, wxDefaultSize, 0 );
	
	extrasSizer->Add( m_checkAddToBookmarks, 0, wxALL, 5 );
	
	m_checkRecursive = new wxCheckBox( this, wxID_ANY, _("Recursive"), wxDefaultPosition, wxDefaultSize, 0 );
	
	extrasSizer->Add( m_checkRecursive, 0, wxALL, 5 );
	
	m_checkIgnoreExternals = new wxCheckBox( this, wxID_ANY, _("Ignore externals"), wxDefaultPosition, wxDefaultSize, 0 );
	
	extrasSizer->Add( m_checkIgnoreExternals, 0, wxALL, 5 );
	
	m_mainSizer->Add( extrasSizer, 1, wxALIGN_CENTER, 5 );
	
	m_buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOK->SetDefault(); 
	m_buttonSizer->Add( m_buttonOK, 0, wxALL, 10 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonSizer->Add( m_buttonCancel, 0, wxALL, 10 );
	
	m_buttonHelp = new wxButton( this, wxID_ANY, _("Help"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonSizer->Add( m_buttonHelp, 0, wxALL, 10 );
	
	m_mainSizer->Add( m_buttonSizer, 0, wxALIGN_CENTER, 5 );
	
	this->SetSizer( m_mainSizer );
	this->Layout();
	
	// Connect Events
	m_comboUrl->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CheckoutDlgBase::OnText ), NULL, this );
	m_comboDest->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CheckoutDlgBase::OnText ), NULL, this );
	m_buttonBrowse->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CheckoutDlgBase::OnBrowse ), NULL, this );
	m_textRevision->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CheckoutDlgBase::OnText ), NULL, this );
	m_checkUseLatest->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CheckoutDlgBase::OnCheckBox ), NULL, this );
	m_textPegRevision->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CheckoutDlgBase::OnText ), NULL, this );
	m_checkPegNotSpecified->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CheckoutDlgBase::OnCheckBox ), NULL, this );
	m_buttonHelp->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CheckoutDlgBase::OnHelp ), NULL, this );
}

CheckoutDlgBase::~CheckoutDlgBase()
{
	// Disconnect Events
	m_comboUrl->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CheckoutDlgBase::OnText ), NULL, this );
	m_comboDest->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CheckoutDlgBase::OnText ), NULL, this );
	m_buttonBrowse->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CheckoutDlgBase::OnBrowse ), NULL, this );
	m_textRevision->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CheckoutDlgBase::OnText ), NULL, this );
	m_checkUseLatest->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CheckoutDlgBase::OnCheckBox ), NULL, this );
	m_textPegRevision->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CheckoutDlgBase::OnText ), NULL, this );
	m_checkPegNotSpecified->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( CheckoutDlgBase::OnCheckBox ), NULL, this );
	m_buttonHelp->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CheckoutDlgBase::OnHelp ), NULL, this );
}

CommitDlgBase::CommitDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_msgSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Enter &log message") ), wxHORIZONTAL );
	
	m_textMessage = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_msgSizer->Add( m_textMessage, 1, wxEXPAND, 5 );
	
	m_mainSizer->Add( m_msgSizer, 1, wxALL|wxEXPAND, 5 );
	
	m_histSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_labelHistory = new wxStaticText( this, wxID_ANY, _("&Recent entries:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labelHistory->Wrap( -1 );
	m_histSizer->Add( m_labelHistory, 0, wxALL, 5 );
	
	m_comboHistory = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	m_histSizer->Add( m_comboHistory, 1, 0, 5 );
	
	m_mainSizer->Add( m_histSizer, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_filesSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("&Files to commit") ), wxVERTICAL );
	
	wxArrayString m_checkListFilesChoices;
	m_checkListFiles = new wxCheckListBox( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_checkListFilesChoices, wxLB_EXTENDED );
	m_filesSizer->Add( m_checkListFiles, 1, wxEXPAND, 5 );
	
	wxBoxSizer* bSizerFileButtons;
	bSizerFileButtons = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonToggle = new wxButton( this, wxID_ANY, _("&Toggle"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerFileButtons->Add( m_buttonToggle, 0, wxALL, 5 );
	
	m_buttonDiff = new wxButton( this, wxID_ANY, _("&Diff"), wxDefaultPosition, wxDefaultSize, 0 );
	bSizerFileButtons->Add( m_buttonDiff, 0, wxALL, 5 );
	
	m_filesSizer->Add( bSizerFileButtons, 0, wxEXPAND, 5 );
	
	m_mainSizer->Add( m_filesSizer, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkRecursive = new wxCheckBox( this, wxID_ANY, _("Recursive"), wxDefaultPosition, wxDefaultSize, 0 );
	
	buttonSizer->Add( m_checkRecursive, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_checkKeepLocks = new wxCheckBox( this, wxID_ANY, _("Keep Locks"), wxDefaultPosition, wxDefaultSize, 0 );
	
	buttonSizer->Add( m_checkKeepLocks, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5 );
	
	m_buttonOK = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOK->SetDefault(); 
	buttonSizer->Add( m_buttonOK, 0, wxALL, 10 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( m_buttonCancel, 0, wxALL, 10 );
	
	m_mainSizer->Add( buttonSizer, 0, wxALIGN_RIGHT|wxLEFT|wxRIGHT, 5 );
	
	this->SetSizer( m_mainSizer );
	this->Layout();
	
	// Connect Events
	m_comboHistory->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( CommitDlgBase::OnComboHistory ), NULL, this );
	m_checkListFiles->Connect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CommitDlgBase::OnCheckListFiles ), NULL, this );
	m_checkListFiles->Connect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CommitDlgBase::OnCheckListFilesDClick ), NULL, this );
	m_checkListFiles->Connect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( CommitDlgBase::OnCheckListFilesToggle ), NULL, this );
	m_buttonToggle->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CommitDlgBase::OnButtonToggle ), NULL, this );
	m_buttonDiff->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CommitDlgBase::OnButtonDiff ), NULL, this );
}

CommitDlgBase::~CommitDlgBase()
{
	// Disconnect Events
	m_comboHistory->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( CommitDlgBase::OnComboHistory ), NULL, this );
	m_checkListFiles->Disconnect( wxEVT_COMMAND_LISTBOX_SELECTED, wxCommandEventHandler( CommitDlgBase::OnCheckListFiles ), NULL, this );
	m_checkListFiles->Disconnect( wxEVT_COMMAND_LISTBOX_DOUBLECLICKED, wxCommandEventHandler( CommitDlgBase::OnCheckListFilesDClick ), NULL, this );
	m_checkListFiles->Disconnect( wxEVT_COMMAND_CHECKLISTBOX_TOGGLED, wxCommandEventHandler( CommitDlgBase::OnCheckListFilesToggle ), NULL, this );
	m_buttonToggle->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CommitDlgBase::OnButtonToggle ), NULL, this );
	m_buttonDiff->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CommitDlgBase::OnButtonDiff ), NULL, this );
}

CommitLogDlgBase::CommitLogDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_msgSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("This action has resulted in a commit - please enter a &log message") ), wxHORIZONTAL );
	
	m_textMessage = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE );
	m_msgSizer->Add( m_textMessage, 1, wxEXPAND, 5 );
	
	m_mainSizer->Add( m_msgSizer, 1, wxALL|wxEXPAND, 5 );
	
	m_histSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_labelHistory = new wxStaticText( this, wxID_ANY, _("Recent entries:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_labelHistory->Wrap( -1 );
	m_histSizer->Add( m_labelHistory, 0, wxALL, 5 );
	
	m_comboHistory = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_READONLY ); 
	m_histSizer->Add( m_comboHistory, 1, 0, 5 );
	
	m_mainSizer->Add( m_histSizer, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOK->SetDefault(); 
	buttonSizer->Add( m_buttonOK, 0, wxALL, 10 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( m_buttonCancel, 0, wxALL, 10 );
	
	m_mainSizer->Add( buttonSizer, 0, wxALIGN_RIGHT|wxLEFT|wxRIGHT, 5 );
	
	this->SetSizer( m_mainSizer );
	this->Layout();
	
	// Connect Events
	m_comboHistory->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( CommitLogDlgBase::OnComboHistory ), NULL, this );
}

CommitLogDlgBase::~CommitLogDlgBase()
{
	// Disconnect Events
	m_comboHistory->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( CommitLogDlgBase::OnComboHistory ), NULL, this );
}

CreateReposDlgBase::CreateReposDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_rootSizer = new wxBoxSizer( wxVERTICAL );
	
	m_notebook = new wxNotebook( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, 0 );
	m_panelGeneral = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sizerGeneral = new wxBoxSizer( wxVERTICAL );
	
	sizerType = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticType = new wxStaticText( m_panelGeneral, wxID_ANY, _("&Type:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticType->Wrap( -1 );
	sizerType->Add( m_staticType, 0, wxALL, 5 );
	
	wxString m_choiceTypeChoices[] = { _("FSFS"), _("BerkeleyDB") };
	int m_choiceTypeNChoices = sizeof( m_choiceTypeChoices ) / sizeof( wxString );
	m_choiceType = new wxChoice( m_panelGeneral, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceTypeNChoices, m_choiceTypeChoices, 0 );
	m_choiceType->SetSelection( 0 );
	sizerType->Add( m_choiceType, 0, wxALL, 5 );
	
	sizerGeneral->Add( sizerType, 0, wxEXPAND, 5 );
	
	m_staticDir = new wxStaticText( m_panelGeneral, wxID_ANY, _("Create repository in &directory:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticDir->Wrap( -1 );
	sizerGeneral->Add( m_staticDir, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	sizerDir = new wxBoxSizer( wxHORIZONTAL );
	
	m_comboDir = new wxComboBox( m_panelGeneral, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	sizerDir->Add( m_comboDir, 1, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_buttonBrowseDir = new wxButton( m_panelGeneral, wxID_ANY, _("Browse"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerDir->Add( m_buttonBrowseDir, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	sizerGeneral->Add( sizerDir, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_staticName = new wxStaticText( m_panelGeneral, wxID_ANY, _("&Name of the new repository:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticName->Wrap( -1 );
	sizerGeneral->Add( m_staticName, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_comboName = new wxComboBox( m_panelGeneral, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	sizerGeneral->Add( m_comboName, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_staticFilename = new wxStaticText( m_panelGeneral, wxID_ANY, _("Resulting repository filename:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticFilename->Wrap( -1 );
	sizerGeneral->Add( m_staticFilename, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	m_textFilename = new wxTextCtrl( m_panelGeneral, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_READONLY );
	m_textFilename->SetForegroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNTEXT ) );
	m_textFilename->SetBackgroundColour( wxSystemSettings::GetColour( wxSYS_COLOUR_BTNFACE ) );
	
	sizerGeneral->Add( m_textFilename, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_checkAddBookmark = new wxCheckBox( m_panelGeneral, wxID_ANY, _("Add a bookmark for the new repository"), wxDefaultPosition, wxDefaultSize, 0 );
	
	sizerGeneral->Add( m_checkAddBookmark, 0, wxALL, 5 );
	
	m_panelGeneral->SetSizer( sizerGeneral );
	m_panelGeneral->Layout();
	sizerGeneral->Fit( m_panelGeneral );
	m_notebook->AddPage( m_panelGeneral, _("General"), false );
	m_panelExtended = new wxPanel( m_notebook, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sizerExtended = new wxBoxSizer( wxVERTICAL );
	
	sizerCompatibility = new wxBoxSizer( wxHORIZONTAL );
	
	m_staticCompatibility = new wxStaticText( m_panelExtended, wxID_ANY, _("&Compatible with:"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticCompatibility->Wrap( -1 );
	sizerCompatibility->Add( m_staticCompatibility, 0, wxALL, 5 );
	
	wxString m_choiceCompatChoices[] = { _("Default"), _("pre Subversion 1.6"), _("pre Subversion 1.5"), _("pre Subversion 1.4") };
	int m_choiceCompatNChoices = sizeof( m_choiceCompatChoices ) / sizeof( wxString );
	m_choiceCompat = new wxChoice( m_panelExtended, wxID_ANY, wxDefaultPosition, wxDefaultSize, m_choiceCompatNChoices, m_choiceCompatChoices, 0 );
	m_choiceCompat->SetSelection( 0 );
	sizerCompatibility->Add( m_choiceCompat, 0, wxALL, 5 );
	
	sizerExtended->Add( sizerCompatibility, 0, wxEXPAND, 5 );
	
	m_staticConfigDir = new wxStaticText( m_panelExtended, wxID_ANY, _("&Load configuration from directory (optional):"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticConfigDir->Wrap( -1 );
	sizerExtended->Add( m_staticConfigDir, 0, wxLEFT|wxRIGHT|wxTOP, 5 );
	
	sizerConfigDir = new wxBoxSizer( wxHORIZONTAL );
	
	m_comboConfigDir = new wxComboBox( m_panelExtended, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	sizerConfigDir->Add( m_comboConfigDir, 1, wxALIGN_CENTER_VERTICAL, 5 );
	
	m_buttonBrowseConfigDir = new wxButton( m_panelExtended, wxID_ANY, _("&Browse"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerConfigDir->Add( m_buttonBrowseConfigDir, 0, wxALIGN_CENTER_VERTICAL, 5 );
	
	sizerExtended->Add( sizerConfigDir, 0, wxBOTTOM|wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	m_checkBdbLogKeep = new wxCheckBox( m_panelExtended, wxID_ANY, _("Disable automatic Log removal"), wxDefaultPosition, wxDefaultSize, 0 );
	
	sizerExtended->Add( m_checkBdbLogKeep, 0, wxALL, 5 );
	
	m_checkBdbTxnNoSync = new wxCheckBox( m_panelExtended, wxID_ANY, _("Disable fsync when committing database transactions"), wxDefaultPosition, wxDefaultSize, 0 );
	
	sizerExtended->Add( m_checkBdbTxnNoSync, 0, wxALL, 5 );
	
	m_panelExtended->SetSizer( sizerExtended );
	m_panelExtended->Layout();
	sizerExtended->Fit( m_panelExtended );
	m_notebook->AddPage( m_panelExtended, _("More options"), false );
	
	m_rootSizer->Add( m_notebook, 1, wxEXPAND | wxALL, 5 );
	
	m_staticWarning = new wxStaticText( this, wxID_ANY, _("WARNING"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticWarning->Wrap( -1 );
	m_staticWarning->SetFont( wxFont( wxNORMAL_FONT->GetPointSize(), 70, 90, 92, false, wxEmptyString ) );
	
	m_rootSizer->Add( m_staticWarning, 0, wxEXPAND|wxLEFT|wxRIGHT, 5 );
	
	sizerButton = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOk = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOk->SetDefault(); 
	sizerButton->Add( m_buttonOk, 0, wxALL, 10 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	sizerButton->Add( m_buttonCancel, 0, wxALL, 10 );
	
	m_rootSizer->Add( sizerButton, 0, wxALIGN_CENTER, 5 );
	
	this->SetSizer( m_rootSizer );
	this->Layout();
	m_rootSizer->Fit( this );
	
	// Connect Events
	m_choiceType->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CreateReposDlgBase::OnChoiceType ), NULL, this );
	m_comboDir->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CreateReposDlgBase::OnComboDirText ), NULL, this );
	m_buttonBrowseDir->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CreateReposDlgBase::OnButtonBrowseDirClick ), NULL, this );
	m_comboName->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CreateReposDlgBase::OnComboNameText ), NULL, this );
	m_choiceCompat->Connect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CreateReposDlgBase::OnChoiceCompat ), NULL, this );
	m_comboConfigDir->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CreateReposDlgBase::OnComboConfigDirText ), NULL, this );
	m_buttonBrowseConfigDir->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CreateReposDlgBase::OnButtonBrowseConfigDirClick ), NULL, this );
}

CreateReposDlgBase::~CreateReposDlgBase()
{
	// Disconnect Events
	m_choiceType->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CreateReposDlgBase::OnChoiceType ), NULL, this );
	m_comboDir->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CreateReposDlgBase::OnComboDirText ), NULL, this );
	m_buttonBrowseDir->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CreateReposDlgBase::OnButtonBrowseDirClick ), NULL, this );
	m_comboName->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CreateReposDlgBase::OnComboNameText ), NULL, this );
	m_choiceCompat->Disconnect( wxEVT_COMMAND_CHOICE_SELECTED, wxCommandEventHandler( CreateReposDlgBase::OnChoiceCompat ), NULL, this );
	m_comboConfigDir->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( CreateReposDlgBase::OnComboConfigDirText ), NULL, this );
	m_buttonBrowseConfigDir->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( CreateReposDlgBase::OnButtonBrowseConfigDirClick ), NULL, this );
}

DeleteDlgBase::DeleteDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_staticQuestion = new wxStaticText( this, wxID_ANY, _("Do you want to delete the selected files/directories?"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticQuestion->Wrap( -1 );
	m_mainSizer->Add( m_staticQuestion, 1, wxALL|wxEXPAND, 5 );
	
	m_checkForce = new wxCheckBox( this, wxID_ANY, _("Force removal"), wxDefaultPosition, wxDefaultSize, 0 );
	
	m_mainSizer->Add( m_checkForce, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOK->SetDefault(); 
	buttonSizer->Add( m_buttonOK, 0, wxALL, 10 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( m_buttonCancel, 0, wxALL, 10 );
	
	m_mainSizer->Add( buttonSizer, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5 );
	
	this->SetSizer( m_mainSizer );
	this->Layout();
	m_mainSizer->Fit( this );
}

DeleteDlgBase::~DeleteDlgBase()
{
}

DiffDlgBase::DiffDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer( wxVERTICAL );
	
	wxBoxSizer* compareSizer;
	compareSizer = new wxBoxSizer( wxHORIZONTAL );
	
	wxStaticText* labelCompare;
	labelCompare = new wxStaticText( this, wxID_ANY, _("Compare:"), wxDefaultPosition, wxDefaultSize, 0 );
	labelCompare->Wrap( -1 );
	compareSizer->Add( labelCompare, 0, 0, 5 );
	
	m_comboCompare = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0, NULL, wxCB_DROPDOWN|wxCB_READONLY ); 
	compareSizer->Add( m_comboCompare, 1, 0, 5 );
	
	mainSizer->Add( compareSizer, 0, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer1;
	sbSizer1 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Revision or date #1:") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer1;
	fgSizer1 = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer1->SetFlexibleDirection( wxBOTH );
	fgSizer1->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_radioUseRevision1 = new wxRadioButton( this, wxID_ANY, _("Revision:"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	fgSizer1->Add( m_radioUseRevision1, 0, 0, 5 );
	
	wxBoxSizer* bSizer16;
	bSizer16 = new wxBoxSizer( wxHORIZONTAL );
	
	m_textRevision1 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer16->Add( m_textRevision1, 0, wxEXPAND, 5 );
	
	m_checkUseLatest1 = new wxCheckBox( this, wxID_ANY, _("Use latest"), wxDefaultPosition, wxDefaultSize, 0 );
	
	bSizer16->Add( m_checkUseLatest1, 0, 0, 5 );
	
	fgSizer1->Add( bSizer16, 1, wxEXPAND, 5 );
	
	m_radioUseDate1 = new wxRadioButton( this, wxID_ANY, _("Date:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer1->Add( m_radioUseDate1, 0, 0, 5 );
	
	m_datePicker1 = new wxDatePickerCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	fgSizer1->Add( m_datePicker1, 0, wxEXPAND, 5 );
	
	m_checkUsePath1 = new wxCheckBox( this, wxID_ANY, _("Use URL/path:"), wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer1->Add( m_checkUsePath1, 0, 0, 5 );
	
	m_comboPath1 = new wxComboBox( this, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer1->Add( m_comboPath1, 0, wxEXPAND, 5 );
	
	sbSizer1->Add( fgSizer1, 1, wxEXPAND, 5 );
	
	mainSizer->Add( sbSizer1, 1, wxALL|wxEXPAND, 5 );
	
	wxStaticBoxSizer* sbSizer2;
	sbSizer2 = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Revision or date #2:") ), wxVERTICAL );
	
	wxFlexGridSizer* fgSizer2;
	fgSizer2 = new wxFlexGridSizer( 3, 2, 0, 0 );
	fgSizer2->SetFlexibleDirection( wxBOTH );
	fgSizer2->SetNonFlexibleGrowMode( wxFLEX_GROWMODE_SPECIFIED );
	
	m_radioUseRevision2 = new wxRadioButton( this, wxID_ANY, _("Revision:"), wxDefaultPosition, wxDefaultSize, wxRB_GROUP );
	fgSizer2->Add( m_radioUseRevision2, 0, 0, 5 );
	
	wxBoxSizer* bSizer161;
	bSizer161 = new wxBoxSizer( wxHORIZONTAL );
	
	m_textRevision2 = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	bSizer161->Add( m_textRevision2, 0, wxEXPAND, 5 );
	
	m_checkUseLatest2 = new wxCheckBox( this, wxID_ANY, _("Use latest"), wxDefaultPosition, wxDefaultSize, 0 );
	
	bSizer161->Add( m_checkUseLatest2, 0, 0, 5 );
	
	fgSizer2->Add( bSizer161, 1, wxEXPAND, 5 );
	
	m_radioUseDate2 = new wxRadioButton( this, wxID_ANY, _("Date:"), wxDefaultPosition, wxDefaultSize, 0 );
	fgSizer2->Add( m_radioUseDate2, 0, 0, 5 );
	
	m_datePicker2 = new wxDatePickerCtrl( this, wxID_ANY, wxDefaultDateTime, wxDefaultPosition, wxDefaultSize, wxDP_DEFAULT );
	fgSizer2->Add( m_datePicker2, 0, wxEXPAND, 5 );
	
	m_checkUsePath2 = new wxCheckBox( this, wxID_ANY, _("Use URL/path:"), wxDefaultPosition, wxDefaultSize, 0 );
	
	fgSizer2->Add( m_checkUsePath2, 0, 0, 5 );
	
	m_comboPath2 = new wxComboBox( this, wxID_ANY, _("Combo!"), wxDefaultPosition, wxDefaultSize, 0, NULL, 0 ); 
	fgSizer2->Add( m_comboPath2, 0, wxEXPAND, 5 );
	
	sbSizer2->Add( fgSizer2, 1, wxEXPAND, 5 );
	
	mainSizer->Add( sbSizer2, 1, wxALL|wxEXPAND, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOK->SetDefault(); 
	buttonSizer->Add( m_buttonOK, 0, wxALL, 5 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( m_buttonCancel, 0, wxALL, 5 );
	
	mainSizer->Add( buttonSizer, 0, wxALIGN_CENTER, 5 );
	
	this->SetSizer( mainSizer );
	this->Layout();
	mainSizer->Fit( this );
	
	// Connect Events
	m_comboCompare->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DiffDlgBase::OnComboCompare ), NULL, this );
	m_radioUseRevision1->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DiffDlgBase::OnRadioUseRevision1 ), NULL, this );
	m_textRevision1->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DiffDlgBase::OnTextRevision1 ), NULL, this );
	m_checkUseLatest1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DiffDlgBase::OnCheckUseLatest1 ), NULL, this );
	m_radioUseDate1->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DiffDlgBase::OnRadioUseDate1 ), NULL, this );
	m_datePicker1->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( DiffDlgBase::OnDatePicker1 ), NULL, this );
	m_checkUsePath1->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DiffDlgBase::OnUsePath1 ), NULL, this );
	m_comboPath1->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DiffDlgBase::OnComboPath1 ), NULL, this );
	m_radioUseRevision2->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DiffDlgBase::OnRadioUseRevision2 ), NULL, this );
	m_textRevision2->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DiffDlgBase::OnTextRevision2 ), NULL, this );
	m_checkUseLatest2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DiffDlgBase::OnCheckUseLatest2 ), NULL, this );
	m_radioUseDate2->Connect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DiffDlgBase::OnRadioUseDate2 ), NULL, this );
	m_datePicker2->Connect( wxEVT_DATE_CHANGED, wxDateEventHandler( DiffDlgBase::OnDatePicker2 ), NULL, this );
	m_checkUsePath2->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DiffDlgBase::OnCheckUsePath2 ), NULL, this );
	m_comboPath2->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DiffDlgBase::OnComboPath2 ), NULL, this );
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiffDlgBase::OnButtonOK ), NULL, this );
}

DiffDlgBase::~DiffDlgBase()
{
	// Disconnect Events
	m_comboCompare->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DiffDlgBase::OnComboCompare ), NULL, this );
	m_radioUseRevision1->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DiffDlgBase::OnRadioUseRevision1 ), NULL, this );
	m_textRevision1->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DiffDlgBase::OnTextRevision1 ), NULL, this );
	m_checkUseLatest1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DiffDlgBase::OnCheckUseLatest1 ), NULL, this );
	m_radioUseDate1->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DiffDlgBase::OnRadioUseDate1 ), NULL, this );
	m_datePicker1->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( DiffDlgBase::OnDatePicker1 ), NULL, this );
	m_checkUsePath1->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DiffDlgBase::OnUsePath1 ), NULL, this );
	m_comboPath1->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DiffDlgBase::OnComboPath1 ), NULL, this );
	m_radioUseRevision2->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DiffDlgBase::OnRadioUseRevision2 ), NULL, this );
	m_textRevision2->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( DiffDlgBase::OnTextRevision2 ), NULL, this );
	m_checkUseLatest2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DiffDlgBase::OnCheckUseLatest2 ), NULL, this );
	m_radioUseDate2->Disconnect( wxEVT_COMMAND_RADIOBUTTON_SELECTED, wxCommandEventHandler( DiffDlgBase::OnRadioUseDate2 ), NULL, this );
	m_datePicker2->Disconnect( wxEVT_DATE_CHANGED, wxDateEventHandler( DiffDlgBase::OnDatePicker2 ), NULL, this );
	m_checkUsePath2->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( DiffDlgBase::OnCheckUsePath2 ), NULL, this );
	m_comboPath2->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( DiffDlgBase::OnComboPath2 ), NULL, this );
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( DiffDlgBase::OnButtonOK ), NULL, this );
}

MainFrameBase::MainFrameBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxFrame( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	wxBoxSizer* sizerRoot;
	sizerRoot = new wxBoxSizer( wxVERTICAL );
	
	m_splitterHoriz = new SplitterWindow( this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE );
	m_splitterHoriz->SetSashGravity( 1 );
	m_splitterHoriz->Connect( wxEVT_IDLE, wxIdleEventHandler( MainFrameBase::m_splitterHorizOnIdle ), NULL, this );
	m_panelTop = new wxPanel( m_splitterHoriz, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	wxBoxSizer* sizerTop;
	sizerTop = new wxBoxSizer( wxVERTICAL );
	
	m_splitterVert = new SplitterWindow( m_panelTop, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxSP_LIVE_UPDATE );
	m_splitterVert->Connect( wxEVT_IDLE, wxIdleEventHandler( MainFrameBase::m_splitterVertOnIdle ), NULL, this );
	panelFolderBrowser = new wxPanel( m_splitterVert, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sizerFolderBrowser = new wxBoxSizer( wxVERTICAL );
	
	m_folderBrowser = new FolderBrowser( panelFolderBrowser, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTR_DEFAULT_STYLE );
	sizerFolderBrowser->Add( m_folderBrowser, 1, wxEXPAND, 5 );
	
	panelFolderBrowser->SetSizer( sizerFolderBrowser );
	panelFolderBrowser->Layout();
	sizerFolderBrowser->Fit( panelFolderBrowser );
	panelListCtrl = new wxPanel( m_splitterVert, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sizerListCtrl = new wxBoxSizer( wxVERTICAL );
	
	m_listCtrl = new FileListCtrl( panelListCtrl, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT );
	sizerListCtrl->Add( m_listCtrl, 1, wxEXPAND, 5 );
	
	panelListCtrl->SetSizer( sizerListCtrl );
	panelListCtrl->Layout();
	sizerListCtrl->Fit( panelListCtrl );
	m_splitterVert->SplitVertically( panelFolderBrowser, panelListCtrl, 0 );
	sizerTop->Add( m_splitterVert, 1, wxEXPAND, 5 );
	
	m_panelTop->SetSizer( sizerTop );
	m_panelTop->Layout();
	sizerTop->Fit( m_panelTop );
	panelBottom = new wxPanel( m_splitterHoriz, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL );
	sizerBottom = new wxBoxSizer( wxVERTICAL );
	
	m_log = new wxTextCtrl( panelBottom, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxTE_RICH2|wxSUNKEN_BORDER );
	sizerBottom->Add( m_log, 1, wxEXPAND, 5 );
	
	panelBottom->SetSizer( sizerBottom );
	panelBottom->Layout();
	sizerBottom->Fit( panelBottom );
	m_splitterHoriz->SplitHorizontally( m_panelTop, panelBottom, -1 );
	sizerRoot->Add( m_splitterHoriz, 1, wxEXPAND, 5 );
	
	this->SetSizer( sizerRoot );
	this->Layout();
	m_statusBar = this->CreateStatusBar( 1, wxST_SIZEGRIP, wxID_ANY );
	
	// Connect Events
	this->Connect( wxEVT_ACTIVATE, wxActivateEventHandler( MainFrameBase::OnActivate ) );
	this->Connect( wxEVT_SIZE, wxSizeEventHandler( MainFrameBase::OnSize ) );
	m_folderBrowser->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( MainFrameBase::OnFolderBrowserSetFocus ), NULL, this );
	m_listCtrl->Connect( wxEVT_SET_FOCUS, wxFocusEventHandler( MainFrameBase::OnListCtrlSetFocus ), NULL, this );
}

MainFrameBase::~MainFrameBase()
{
	// Disconnect Events
	this->Disconnect( wxEVT_ACTIVATE, wxActivateEventHandler( MainFrameBase::OnActivate ) );
	this->Disconnect( wxEVT_SIZE, wxSizeEventHandler( MainFrameBase::OnSize ) );
	m_folderBrowser->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( MainFrameBase::OnFolderBrowserSetFocus ), NULL, this );
	m_listCtrl->Disconnect( wxEVT_SET_FOCUS, wxFocusEventHandler( MainFrameBase::OnListCtrlSetFocus ), NULL, this );
}

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
	m_notebookMain->AddPage( m_panelGeneral, _("General"), false );
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
	m_buttonMergeTool->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnButtonMergeToolClick ), NULL, this );
	m_buttonOK->Connect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnOK ), NULL, this );
}

PreferencesDlgBase::~PreferencesDlgBase()
{
	// Disconnect Events
	m_buttonEditor->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnButtonEditorClick ), NULL, this );
	m_buttonExplorer->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnButtonExplorerClick ), NULL, this );
	m_buttonDiffTool->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnButtonDiffToolClick ), NULL, this );
	m_buttonMergeTool->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnButtonMergeToolClick ), NULL, this );
	m_buttonOK->Disconnect( wxEVT_COMMAND_BUTTON_CLICKED, wxCommandEventHandler( PreferencesDlgBase::OnOK ), NULL, this );
}

SwitchDlgBase::SwitchDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_urlSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("URL") ), wxVERTICAL );
	
	m_comboUrl = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 235,-1 ), 0, NULL, wxCB_DROPDOWN ); 
	m_urlSizer->Add( m_comboUrl, 1, wxALL|wxEXPAND, 5 );
	
	m_mainSizer->Add( m_urlSizer, 0, wxEXPAND, 5 );
	
	m_revisionSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Revision") ), wxHORIZONTAL );
	
	m_textRevision = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_revisionSizer->Add( m_textRevision, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	m_checkUseLatest = new wxCheckBox( this, wxID_ANY, _("Use latest"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkUseLatest->SetValue(true);
	
	m_revisionSizer->Add( m_checkUseLatest, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );
	
	m_mainSizer->Add( m_revisionSizer, 0, wxEXPAND, 5 );
	
	wxBoxSizer* optionSizer;
	optionSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkRecursive = new wxCheckBox( this, wxID_ANY, _("Recursive"), wxDefaultPosition, wxDefaultSize, 0 );
	
	optionSizer->Add( m_checkRecursive, 0, 0, 5 );
	
	m_checkRelocate = new wxCheckBox( this, wxID_ANY, _("Relocate"), wxDefaultPosition, wxDefaultSize, 0 );
	
	optionSizer->Add( m_checkRelocate, 0, 0, 5 );
	
	m_mainSizer->Add( optionSizer, 1, wxALIGN_CENTER|wxBOTTOM|wxTOP, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOK->SetDefault(); 
	buttonSizer->Add( m_buttonOK, 0, wxALL, 10 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( m_buttonCancel, 0, wxALL, 10 );
	
	m_mainSizer->Add( buttonSizer, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5 );
	
	this->SetSizer( m_mainSizer );
	this->Layout();
	m_mainSizer->Fit( this );
	
	// Connect Events
	m_comboUrl->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( SwitchDlgBase::OnText ), NULL, this );
	m_textRevision->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SwitchDlgBase::OnText ), NULL, this );
	m_checkUseLatest->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SwitchDlgBase::OnUseLatest ), NULL, this );
}

SwitchDlgBase::~SwitchDlgBase()
{
	// Disconnect Events
	m_comboUrl->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( SwitchDlgBase::OnText ), NULL, this );
	m_textRevision->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( SwitchDlgBase::OnText ), NULL, this );
	m_checkUseLatest->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( SwitchDlgBase::OnUseLatest ), NULL, this );
}

UnlockDlgBase::UnlockDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_staticQuestion = new wxStaticText( this, wxID_ANY, _("Do you want to unlock the selected files/directories?"), wxDefaultPosition, wxDefaultSize, 0 );
	m_staticQuestion->Wrap( -1 );
	m_mainSizer->Add( m_staticQuestion, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	m_checkForce = new wxCheckBox( this, wxID_ANY, _("Force unlocking even if you are not the lock owner"), wxDefaultPosition, wxDefaultSize, 0 );
	
	m_mainSizer->Add( m_checkForce, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOK->SetDefault(); 
	buttonSizer->Add( m_buttonOK, 0, wxALL, 10 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( m_buttonCancel, 0, wxALL, 10 );
	
	m_mainSizer->Add( buttonSizer, 0, wxALIGN_CENTER_HORIZONTAL, 5 );
	
	this->SetSizer( m_mainSizer );
	this->Layout();
}

UnlockDlgBase::~UnlockDlgBase()
{
}

UpdateDlgBase::UpdateDlgBase( wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint& pos, const wxSize& size, long style ) : wxDialog( parent, id, title, pos, size, style )
{
	this->SetSizeHints( wxDefaultSize, wxDefaultSize );
	
	m_mainSizer = new wxBoxSizer( wxVERTICAL );
	
	m_urlSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("URL") ), wxVERTICAL );
	
	m_comboUrl = new wxComboBox( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize( 235,-1 ), 0, NULL, wxCB_DROPDOWN ); 
	m_urlSizer->Add( m_comboUrl, 1, wxALL|wxEXPAND, 5 );
	
	m_mainSizer->Add( m_urlSizer, 0, wxEXPAND, 5 );
	
	m_revisionSizer = new wxStaticBoxSizer( new wxStaticBox( this, wxID_ANY, _("Revision") ), wxHORIZONTAL );
	
	m_textRevision = new wxTextCtrl( this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxDefaultSize, 0 );
	m_revisionSizer->Add( m_textRevision, 1, wxALIGN_CENTER_VERTICAL|wxALL|wxEXPAND, 5 );
	
	m_checkUseLatest = new wxCheckBox( this, wxID_ANY, _("Use latest"), wxDefaultPosition, wxDefaultSize, 0 );
	m_checkUseLatest->SetValue(true);
	
	m_revisionSizer->Add( m_checkUseLatest, 0, wxALIGN_CENTER_VERTICAL|wxRIGHT, 5 );
	
	m_mainSizer->Add( m_revisionSizer, 0, wxEXPAND, 5 );
	
	wxBoxSizer* optionSizer;
	optionSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_checkForce = new wxCheckBox( this, wxID_ANY, _("Force"), wxDefaultPosition, wxDefaultSize, 0 );
	
	optionSizer->Add( m_checkForce, 0, 0, 5 );
	
	m_checkRecursive = new wxCheckBox( this, wxID_ANY, _("Recursive"), wxDefaultPosition, wxDefaultSize, 0 );
	
	optionSizer->Add( m_checkRecursive, 0, 0, 5 );
	
	m_checkIgnoreExternals = new wxCheckBox( this, wxID_ANY, _("Ignore Externals"), wxDefaultPosition, wxDefaultSize, 0 );
	
	optionSizer->Add( m_checkIgnoreExternals, 0, 0, 5 );
	
	m_mainSizer->Add( optionSizer, 1, wxALIGN_CENTER|wxBOTTOM|wxTOP, 5 );
	
	wxBoxSizer* buttonSizer;
	buttonSizer = new wxBoxSizer( wxHORIZONTAL );
	
	m_buttonOK = new wxButton( this, wxID_OK, _("OK"), wxDefaultPosition, wxDefaultSize, 0 );
	m_buttonOK->SetDefault(); 
	buttonSizer->Add( m_buttonOK, 0, wxALL, 10 );
	
	m_buttonCancel = new wxButton( this, wxID_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
	buttonSizer->Add( m_buttonCancel, 0, wxALL, 10 );
	
	m_mainSizer->Add( buttonSizer, 0, wxALIGN_CENTER|wxLEFT|wxRIGHT, 5 );
	
	this->SetSizer( m_mainSizer );
	this->Layout();
	
	// Connect Events
	m_comboUrl->Connect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( UpdateDlgBase::OnText ), NULL, this );
	m_textRevision->Connect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( UpdateDlgBase::OnText ), NULL, this );
	m_checkUseLatest->Connect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( UpdateDlgBase::OnUseLatest ), NULL, this );
}

UpdateDlgBase::~UpdateDlgBase()
{
	// Disconnect Events
	m_comboUrl->Disconnect( wxEVT_COMMAND_COMBOBOX_SELECTED, wxCommandEventHandler( UpdateDlgBase::OnText ), NULL, this );
	m_textRevision->Disconnect( wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler( UpdateDlgBase::OnText ), NULL, this );
	m_checkUseLatest->Disconnect( wxEVT_COMMAND_CHECKBOX_CLICKED, wxCommandEventHandler( UpdateDlgBase::OnUseLatest ), NULL, this );
}
