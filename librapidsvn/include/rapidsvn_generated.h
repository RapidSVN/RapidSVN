///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 21 2016)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __RAPIDSVN_GENERATED_H__
#define __RAPIDSVN_GENERATED_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/intl.h>
class LogAffectedList;

#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/statbmp.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/sizer.h>
#include <wx/button.h>
#include <wx/dialog.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/statbox.h>
#include <wx/combobox.h>
#include <wx/checkbox.h>
#include <wx/checklst.h>
#include <wx/choice.h>
#include <wx/panel.h>
#include <wx/notebook.h>
#include <wx/radiobut.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/splitter.h>
#include <wx/treectrl.h>
#include <wx/statusbr.h>
#include <wx/frame.h>

#include "controls.hpp"

///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
/// Class AboutDlgBase
///////////////////////////////////////////////////////////////////////////////
class AboutDlgBase : public wxDialog 
{
	private:
		wxFlexGridSizer* topSizer;
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticBitmap* m_bitmapLogo;
		wxStaticText* m_staticCopy;
		wxStaticText* m_staticBuilt;
		wxStaticText* m_staticInfo;
		wxButton* m_buttonOK;
	
	public:
		
		AboutDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("About ..."), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~AboutDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class AnnotateDlgBase
///////////////////////////////////////////////////////////////////////////////
class AnnotateDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxListCtrl* m_list;
		wxButton* m_buttonOK;
	
	public:
		
		AnnotateDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Annotate"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 398,313 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~AnnotateDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class AuthDlgBase
///////////////////////////////////////////////////////////////////////////////
class AuthDlgBase : public wxDialog 
{
	private:
		wxFlexGridSizer* authSizer;
		wxBoxSizer* buttonSizer;
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticText* m_staticUsername;
		wxTextCtrl* m_textUsername;
		wxStaticText* m_staticPassword;
		wxTextCtrl* m_textPassword;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
	
	public:
		
		AuthDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~AuthDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CertDlgBase
///////////////////////////////////////////////////////////////////////////////
class CertDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticText* m_staticTitle;
		wxStaticBoxSizer* m_sizerFailures;
		wxStaticText* m_staticFailures;
		wxStaticText* m_labelHostname;
		wxStaticText* m_staticHostname;
		wxStaticText* m_labelIssuer;
		wxStaticText* m_staticIssuer;
		wxStaticText* m_labelValidFrom;
		wxStaticText* m_staticValidFrom;
		wxStaticText* m_labelValidUntil;
		wxStaticText* m_staticValidUntil;
		wxStaticText* m_labelFingerprint;
		wxStaticText* m_staticFingerprint;
		wxButton* m_buttonPerm;
		wxButton* m_buttonTemp;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnPerm( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTemp( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		CertDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("SSL Certificate"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 394,339 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~CertDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CheckoutDlgBase
///////////////////////////////////////////////////////////////////////////////
class CheckoutDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxComboBox* m_comboUrl;
		wxComboBox* m_comboDest;
		wxButton* m_buttonBrowse;
		wxTextCtrl* m_textRevision;
		wxCheckBox* m_checkUseLatest;
		wxTextCtrl* m_textPegRevision;
		wxCheckBox* m_checkPegNotSpecified;
		wxCheckBox* m_checkAddToBookmarks;
		wxCheckBox* m_checkRecursive;
		wxCheckBox* m_checkIgnoreExternals;
		wxBoxSizer* m_buttonSizer;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		wxButton* m_buttonHelp;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBrowse( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		CheckoutDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Checkout"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 389,333 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~CheckoutDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CommitDlgBase
///////////////////////////////////////////////////////////////////////////////
class CommitDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticBoxSizer* m_msgSizer;
		wxTextCtrl* m_textMessage;
		wxBoxSizer* m_histSizer;
		wxStaticText* m_labelHistory;
		wxComboBox* m_comboHistory;
		wxStaticBoxSizer* m_filesSizer;
		wxCheckListBox* m_checkListFiles;
		wxButton* m_buttonToggle;
		wxButton* m_buttonDiff;
		wxCheckBox* m_checkRecursive;
		wxCheckBox* m_checkKeepLocks;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnComboHistory( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCheckListFiles( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCheckListFilesDClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCheckListFilesToggle( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonToggle( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonDiff( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		CommitDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Commit Log Message"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 583,500 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxTAB_TRAVERSAL ); 
		~CommitDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CommitLogDlgBase
///////////////////////////////////////////////////////////////////////////////
class CommitLogDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticBoxSizer* m_msgSizer;
		wxTextCtrl* m_textMessage;
		wxBoxSizer* m_histSizer;
		wxStaticText* m_labelHistory;
		wxComboBox* m_comboHistory;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnComboHistory( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		CommitLogDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Commit Log Message"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 498,232 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~CommitLogDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class CreateReposDlgBase
///////////////////////////////////////////////////////////////////////////////
class CreateReposDlgBase : public wxDialog 
{
	private:
		wxBoxSizer* sizerGeneral;
		wxBoxSizer* sizerType;
		wxStaticText* m_staticDir;
		wxStaticText* m_staticFilename;
		wxBoxSizer* sizerExtended;
		wxBoxSizer* sizerCompatibility;
		wxBoxSizer* sizerConfigDir;
		wxBoxSizer* sizerButton;
	
	protected:
		wxBoxSizer* m_rootSizer;
		wxNotebook* m_notebook;
		wxPanel* m_panelGeneral;
		wxStaticText* m_staticType;
		wxChoice* m_choiceType;
		wxBoxSizer* sizerDir;
		wxComboBox* m_comboDir;
		wxButton* m_buttonBrowseDir;
		wxStaticText* m_staticName;
		wxComboBox* m_comboName;
		wxTextCtrl* m_textFilename;
		wxCheckBox* m_checkAddBookmark;
		wxPanel* m_panelExtended;
		wxStaticText* m_staticCompatibility;
		wxChoice* m_choiceCompat;
		wxStaticText* m_staticConfigDir;
		wxComboBox* m_comboConfigDir;
		wxButton* m_buttonBrowseConfigDir;
		wxCheckBox* m_checkBdbLogKeep;
		wxCheckBox* m_checkBdbTxnNoSync;
		wxStaticText* m_staticWarning;
		wxButton* m_buttonOk;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnChoiceType( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnComboDirText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonBrowseDirClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnComboNameText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnChoiceCompat( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnComboConfigDirText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonBrowseConfigDirClick( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		CreateReposDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Create Repository"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~CreateReposDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DeleteDlgBase
///////////////////////////////////////////////////////////////////////////////
class DeleteDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticText* m_staticQuestion;
		wxCheckBox* m_checkForce;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
	
	public:
		
		DeleteDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Delete"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~DeleteDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DestinationDlgBase
///////////////////////////////////////////////////////////////////////////////
class DestinationDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticText* m_staticQuestion;
		wxComboBox* m_comboDestination;
		wxCheckBox* m_checkForce;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnText( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DestinationDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Destination"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~DestinationDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DiffDlgBase
///////////////////////////////////////////////////////////////////////////////
class DiffDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxComboBox* m_comboCompare;
		wxRadioButton* m_radioUseRevision1;
		wxTextCtrl* m_textRevision1;
		wxCheckBox* m_checkUseLatest1;
		wxRadioButton* m_radioUseDate1;
		wxDatePickerCtrl* m_datePicker1;
		wxCheckBox* m_checkUsePath1;
		wxComboBox* m_comboPath1;
		wxRadioButton* m_radioUseRevision2;
		wxTextCtrl* m_textRevision2;
		wxCheckBox* m_checkUseLatest2;
		wxRadioButton* m_radioUseDate2;
		wxDatePickerCtrl* m_datePicker2;
		wxCheckBox* m_checkUsePath2;
		wxComboBox* m_comboPath2;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnComboCompare( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRadioUseRevision1( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTextRevision1( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCheckUseLatest1( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRadioUseDate1( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDatePicker1( wxDateEvent& event ) { event.Skip(); }
		virtual void OnUsePath1( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnComboPath1( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRadioUseRevision2( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnTextRevision2( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCheckUseLatest2( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnRadioUseDate2( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDatePicker2( wxDateEvent& event ) { event.Skip(); }
		virtual void OnCheckUsePath2( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnComboPath2( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonOK( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DiffDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Diff"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~DiffDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class DragAndDropDlgBase
///////////////////////////////////////////////////////////////////////////////
class DragAndDropDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticText* m_staticQuestion;
		wxButton* m_buttonImport;
		wxButton* m_buttonMove;
		wxButton* m_buttonCopy;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonImport( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonMove( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonCopy( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		DragAndDropDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Import"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~DragAndDropDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class EntryDlgBase
///////////////////////////////////////////////////////////////////////////////
class EntryDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticText* m_staticName;
		wxComboBox* m_comboName;
		wxStaticText* m_staticValue;
		wxTextCtrl* m_textValue;
		wxBoxSizer* m_buttonSizer;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnText( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		EntryDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Edit"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 489,331 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~EntryDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ExportDlgBase
///////////////////////////////////////////////////////////////////////////////
class ExportDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxComboBox* m_comboUrl;
		wxComboBox* m_comboDest;
		wxButton* m_buttonBrowse;
		wxTextCtrl* m_textRevision;
		wxCheckBox* m_checkUseLatest;
		wxTextCtrl* m_textPegRevision;
		wxCheckBox* m_checkPegNotSpecified;
		wxStaticText* m_staticEol;
		wxChoice* m_choice3;
		wxCheckBox* m_checkRecursive;
		wxCheckBox* m_checkOverwrite;
		wxCheckBox* m_checkIgnoreExternals;
		wxBoxSizer* m_buttonSizer;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		wxButton* m_buttonHelp;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBrowse( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnCheckBox( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnHelp( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ExportDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Export"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~ExportDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ListEditorDlgBase
///////////////////////////////////////////////////////////////////////////////
class ListEditorDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticBoxSizer* m_listSizer;
		wxBoxSizer* m_buttonSizer;
		wxButton* m_buttonNew;
		wxButton* m_buttonEdit;
		wxButton* m_buttonDelete;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnNew( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnEdit( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDelete( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ListEditorDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("ListEditor"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 602,372 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~ListEditorDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class LockDlgBase
///////////////////////////////////////////////////////////////////////////////
class LockDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticBoxSizer* m_msgSizer;
		wxTextCtrl* m_textMessage;
		wxCheckBox* m_checkStealLock;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
	
	public:
		
		LockDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Lock"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~LockDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class LogDlgBase
///////////////////////////////////////////////////////////////////////////////
class LogDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxSplitterWindow* m_splitter;
		wxPanel* m_upperPanel;
		wxStaticText* m_staticRevisions;
		LogRevList* m_listRevisions;
		wxButton* m_buttonMore;
		wxButton* m_buttonClose;
		wxButton* m_buttonView;
		wxButton* m_buttonGet;
		wxButton* m_buttonDiff;
		wxButton* m_buttonMerge;
		wxButton* m_buttonAnnotate;
		wxPanel* m_lowerPanel;
		wxNotebook* m_notebook;
		wxPanel* m_panelLog;
		wxTextCtrl* m_textLog;
		wxPanel* m_panelFiles;
		LogAffectedList* m_listFiles;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnRevDeselected( wxListEvent& event ) { event.Skip(); }
		virtual void OnRevSelected( wxListEvent& event ) { event.Skip(); }
		virtual void OnMore( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnView( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnGet( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnDiff( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnMerge( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAnnotate( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnAffectedFileOrDirRightClick( wxListEvent& event ) { event.Skip(); }
		
	
	public:
		
		LogDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Log History"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 709,456 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~LogDlgBase();
		
		void m_splitterOnIdle( wxIdleEvent& )
		{
			m_splitter->SetSashPosition( 0 );
			m_splitter->Disconnect( wxEVT_IDLE, wxIdleEventHandler( LogDlgBase::m_splitterOnIdle ), NULL, this );
		}
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ImportDlgBase
///////////////////////////////////////////////////////////////////////////////
class ImportDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxComboBox* m_comboUrl;
		wxTextCtrl* m_textPath;
		wxButton* m_buttonBrowse;
		wxStaticText* m_staticPathType;
		wxChoice* m_choicePathType;
		wxStaticBoxSizer* m_msgSizer;
		wxTextCtrl* m_textMessage;
		wxBoxSizer* m_buttonSizer;
		wxCheckBox* m_checkRecursive;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnCommand( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBrowse( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		ImportDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Import"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~ImportDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MainFrameBase
///////////////////////////////////////////////////////////////////////////////
class MainFrameBase : public wxFrame 
{
	private:
		wxPanel* panelFolderBrowser;
		wxBoxSizer* sizerFolderBrowser;
		wxPanel* panelListCtrl;
		wxBoxSizer* sizerListCtrl;
		wxPanel* panelBottom;
		wxBoxSizer* sizerBottom;
	
	protected:
		SplitterWindow* m_splitterHoriz;
		wxPanel* m_panelTop;
		SplitterWindow* m_splitterVert;
		FolderBrowser* m_folderBrowser;
		FileListCtrl* m_listCtrl;
		wxTextCtrl* m_log;
		wxStatusBar* m_statusBar;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnActivate( wxActivateEvent& event ) { event.Skip(); }
		virtual void OnSize( wxSizeEvent& event ) { event.Skip(); }
		virtual void OnFolderBrowserSetFocus( wxFocusEvent& event ) { event.Skip(); }
		virtual void OnListCtrlSetFocus( wxFocusEvent& event ) { event.Skip(); }
		
	
	public:
		
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 639,441 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		
		~MainFrameBase();
		
		void m_splitterVertOnIdle( wxIdleEvent& )
		{
			m_splitterVert->SetSashPosition( 0 );
			m_splitterVert->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainFrameBase::m_splitterVertOnIdle ), NULL, this );
		}
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class MergeDlgBase
///////////////////////////////////////////////////////////////////////////////
class MergeDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticText* m_staticRevision1;
		wxComboBox* m_comboUrl1;
		wxTextCtrl* m_textRevision1;
		wxStaticText* m_staticRevision2;
		wxComboBox* m_comboUrl2;
		wxTextCtrl* m_textRevision2;
		wxComboBox* m_comboDest;
		wxButton* m_buttonBrowse;
		wxCheckBox* m_checkRecursive;
		wxCheckBox* m_checkForce;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnBrowse( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		MergeDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Merge revisions"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 599,337 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~MergeDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class PreferencesDlgBase
///////////////////////////////////////////////////////////////////////////////
class PreferencesDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxNotebook* m_notebookMain;
		wxPanel* m_panelGeneral;
		wxCheckBox* m_checkPurgeTempFiles;
		wxCheckBox* m_checkUseLastCommitMessage;
		wxCheckBox* m_checkResetFlatMode;
		wxPanel* m_panelPrograms;
		wxNotebook* m_notebook2;
		wxPanel* m_panelEditor;
		wxTextCtrl* m_textEditor;
		wxButton* m_buttonEditor;
		wxTextCtrl* m_textEditorArgs;
		wxCheckBox* m_checkEditorAlways;
		wxPanel* m_panelExplorer;
		wxTextCtrl* m_textExplorer;
		wxButton* m_buttonExplorer;
		wxTextCtrl* m_textExplorerArgs;
		wxCheckBox* m_checkExplorerAlways;
		wxPanel* m_panelDiffTool;
		wxTextCtrl* m_textDiffTool;
		wxButton* m_buttonDiffTool;
		wxTextCtrl* m_textDiffToolArgs;
		wxPanel* m_panelMergeTool;
		wxTextCtrl* m_textMergeTool;
		wxButton* m_buttonMergeTool;
		wxStaticText* m_staticMergeToolArgs;
		wxTextCtrl* m_textMergeToolArgs;
		wxPanel* m_panelAuth;
		wxCheckBox* m_checkAuthPerBookmark;
		wxCheckBox* m_checkUseAuthCache;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnButtonEditorClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonExplorerClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonDiffToolClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnButtonMergeToolClick( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnOK( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		PreferencesDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~PreferencesDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class ReportDlgBase
///////////////////////////////////////////////////////////////////////////////
class ReportDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxTextCtrl* m_text;
		wxButton* m_buttonOK;
	
	public:
		
		ReportDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Report"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 567,409 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~ReportDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class RevertDlgBase
///////////////////////////////////////////////////////////////////////////////
class RevertDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticText* m_staticQuestion;
		wxCheckBox* m_checkRecursive;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
	
	public:
		
		RevertDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Revert"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE ); 
		~RevertDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class SwitchDlgBase
///////////////////////////////////////////////////////////////////////////////
class SwitchDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticBoxSizer* m_urlSizer;
		wxComboBox* m_comboUrl;
		wxStaticBoxSizer* m_revisionSizer;
		wxTextCtrl* m_textRevision;
		wxCheckBox* m_checkUseLatest;
		wxCheckBox* m_checkRecursive;
		wxCheckBox* m_checkRelocate;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUseLatest( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		SwitchDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Switch URL"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER ); 
		~SwitchDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class UnlockDlgBase
///////////////////////////////////////////////////////////////////////////////
class UnlockDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticText* m_staticQuestion;
		wxCheckBox* m_checkForce;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
	
	public:
		
		UnlockDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Unlock"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 440,143 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~UnlockDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class UpdateDlgBase
///////////////////////////////////////////////////////////////////////////////
class UpdateDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxBoxSizer* m_mainSizer;
		wxStaticBoxSizer* m_urlSizer;
		wxComboBox* m_comboUrl;
		wxStaticBoxSizer* m_revisionSizer;
		wxTextCtrl* m_textRevision;
		wxCheckBox* m_checkUseLatest;
		wxCheckBox* m_checkRecursive;
		wxCheckBox* m_checkIgnoreExternals;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnText( wxCommandEvent& event ) { event.Skip(); }
		virtual void OnUseLatest( wxCommandEvent& event ) { event.Skip(); }
		
	
	public:
		
		UpdateDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 440,309 ), long style = wxDEFAULT_DIALOG_STYLE ); 
		~UpdateDlgBase();
	
};

#endif //__RAPIDSVN_GENERATED_H__
