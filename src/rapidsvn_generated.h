///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Feb  1 2008)
// http://www.wxformbuilder.org/
//
// PLEASE DO "NOT" EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __rapidsvn_generated__
#define __rapidsvn_generated__

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
#include <wx/filepicker.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/notebook.h>
#include <wx/button.h>
#include <wx/dialog.h>

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
		wxFilePickerCtrl* m_filePickerEditor;
		wxTextCtrl* m_textEditorArgs;
		wxCheckBox* m_checkEditorAlways;
		wxPanel* m_panelExplorer;
		wxFilePickerCtrl* m_filePickerExplorer;
		wxTextCtrl* m_textExplorerArgs;
		wxCheckBox* m_checkExplorerAlways;
		wxPanel* m_panelDiffTool;
		wxFilePickerCtrl* m_filePickerDiffTool;
		wxTextCtrl* m_textDiffToolArgs;
		wxPanel* m_panelMergeTool;
		wxFilePickerCtrl* m_filePickeMergeTool;
		wxStaticText* m_staticMergeToolArgs;
		wxTextCtrl* m_textMergeToolArgs;
		wxPanel* m_panelAuth;
		wxCheckBox* m_checkAuthPerBookmark;
		wxCheckBox* m_checkUseAuthCache;
		wxButton* m_buttonOK;
		wxButton* m_buttonCancel;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnOK( wxCommandEvent& event ){ event.Skip(); }
		
	
	public:
		PreferencesDlgBase( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = _("Preferences"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE );
		~PreferencesDlgBase();
	
};

#endif //__rapidsvn_generated__
