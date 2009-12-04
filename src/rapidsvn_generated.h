///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Dec 29 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __rapidsvn_generated__
#define __rapidsvn_generated__

#include <wx/intl.h>

class FileListCtrl;
class FolderBrowser;
class SplitterWindow;

#include <wx/string.h>
#include <wx/checkbox.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/sizer.h>
#include <wx/panel.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/textctrl.h>
#include <wx/button.h>
#include <wx/stattext.h>
#include <wx/notebook.h>
#include <wx/dialog.h>
#include <wx/combobox.h>
#include <wx/radiobut.h>
#include <wx/datectrl.h>
#include <wx/dateevt.h>
#include <wx/statbox.h>
#include <wx/checklst.h>
#include <wx/choice.h>
#include <wx/treectrl.h>
#include <wx/listctrl.h>
#include <wx/splitter.h>
#include <wx/statusbr.h>
#include <wx/frame.h>
#include <wx/statbmp.h>

#include "controls.hpp"

///////////////////////////////////////////////////////////////////////////

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
		virtual void OnButtonEditorClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonExplorerClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonDiffToolClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonMergeToolClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnOK( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		PreferencesDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~PreferencesDlgBase();
	
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
		virtual void OnComboCompare( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnRadioUseRevision1( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTextRevision1( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckUseLatest1( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnRadioUseDate1( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDatePicker1( wxDateEvent& event ){ event.Skip(); }
		virtual void OnUsePath1( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnComboPath1( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnRadioUseRevision2( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTextRevision2( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckUseLatest2( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnRadioUseDate2( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnDatePicker2( wxDateEvent& event ){ event.Skip(); }
		virtual void OnCheckUsePath2( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnComboPath2( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonOK( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		DiffDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Diff"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~DiffDlgBase();
	
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
		virtual void OnComboHistory( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		CommitLogDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Commit Log Message"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 498,232 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~CommitLogDlgBase();
	
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
		virtual void OnComboHistory( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckListFiles( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckListFilesDClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnCheckListFilesToggle( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonToggle( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonDiff( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		CommitDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Commit Log Message"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 583,500 ), long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER|wxTAB_TRAVERSAL );
		~CommitDlgBase();
	
};

///////////////////////////////////////////////////////////////////////////////
/// Class UpdateDlgBase
///////////////////////////////////////////////////////////////////////////////
class UpdateDlgBase : public wxDialog 
{
	private:
	
	protected:
		wxComboBox* m_comboUrl;
		wxTextCtrl* m_textRevision;
		wxCheckBox* m_checkUseLatest;
		wxCheckBox* m_checkForce;
		wxCheckBox* m_checkRecursive;
		wxCheckBox* m_checkIgnoreExternals;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnComboRevision( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnComboRevisionText( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnTextRevision( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		UpdateDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~UpdateDlgBase();
	
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
		virtual void OnChoiceType( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnComboDirText( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonBrowseDirClick( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnComboNameText( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnChoiceCompat( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnComboConfigDirText( wxCommandEvent& event ){ event.Skip(); }
		virtual void OnButtonBrowseConfigDirClick( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		CreateReposDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Create Repository"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE|wxRESIZE_BORDER );
		~CreateReposDlgBase();
	
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
		virtual void OnActivate( wxActivateEvent& event ){ event.Skip(); }
		virtual void OnSize( wxSizeEvent& event ){ event.Skip(); }
		virtual void OnFolderBrowserSetFocus( wxFocusEvent& event ){ event.Skip(); }
		virtual void OnListCtrlSetFocus( wxFocusEvent& event ){ event.Skip(); }
		
	
	public:
		MainFrameBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxSize( 639,441 ), long style = wxDEFAULT_FRAME_STYLE|wxTAB_TRAVERSAL );
		~MainFrameBase();
		void m_splitterHorizOnIdle( wxIdleEvent& )
		{
		m_splitterHoriz->SetSashPosition( -1 );
		m_splitterHoriz->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainFrameBase::m_splitterHorizOnIdle ), NULL, this );
		}
		
		void m_splitterVertOnIdle( wxIdleEvent& )
		{
		m_splitterVert->SetSashPosition( 0 );
		m_splitterVert->Disconnect( wxEVT_IDLE, wxIdleEventHandler( MainFrameBase::m_splitterVertOnIdle ), NULL, this );
		}
		
	
};

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

#endif //__rapidsvn_generated__
