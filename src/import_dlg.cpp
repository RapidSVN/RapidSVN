#include "include.h"
#include "rapidsvn_app.h"
#include "import_dlg.h"

BEGIN_EVENT_TABLE(ImportDlg, wxDialog)
	EVT_BUTTON(ID_BUTTON_OK, ImportDlg::OnOk)
	EVT_BUTTON(ID_BUTTON_CANCEL, ImportDlg::OnCancel)
	EVT_BUTTON(ID_BUTTON_BROWSE, ImportDlg::OnBrowse)
	EVT_RADIOBOX(ID_RADIOBOX_PATH_TYPE, ImportDlg::OnRadio)
END_EVENT_TABLE()

#define PATH_TYPE_TREE	0
#define PATH_TYPE_FILE	1

void ImportDlg::OnOk( wxCommandEvent& WXUNUSED(event) )
{
	wxString		val;
	
	val = reposURL->GetValue();
	TrimString( val );
	if( val.IsEmpty() )
	{
		wxMessageBox( _T("Repository URL is required for import!"),
						_T("Error"),
						wxOK | wxCENTRE | wxICON_ERROR );

		// Do not allow the user to continue if the path is empty 
		// and the import is addressing a file.
		return;
		}

	if( path_type->GetSelection() == PATH_TYPE_FILE )
	{

		val = path->GetValue();
		TrimString( val );

		if( val.IsEmpty() )
		{
			wxMessageBox( _T("File path required when importing a file!"),
							_T("Error"),
							wxOK | wxCENTRE | wxICON_ERROR );

			// Do not allow the user to continue if the reposURL is empty.
			return;
		}

		val = new_entry->GetValue();
		TrimString( val );

		if( val.IsEmpty() )
		{
			wxMessageBox( _T("New entry name required when importing a file!"),
							_T("Error"),
							wxOK | wxCENTRE | wxICON_ERROR );

			// Do not allow the user to continue if the new_entry is empty 
			// and the import is addressing a file.
			return;
		}
	}

	EndModal( ID_BUTTON_OK );
}

void ImportDlg::OnCancel( wxCommandEvent& WXUNUSED(event) )
{
	EndModal( ID_BUTTON_CANCEL );
}

void ImportDlg::OnBrowse( wxCommandEvent& WXUNUSED(event) )
{
	if( path_type->GetSelection() == PATH_TYPE_TREE )	// it's a tree
	{
		wxDirDialog dialog( this, 
							_T("Select a directory to import"),
							wxGetApp().GetAppFrame()->GetFolderBrowser()->GetPath() );

		if( dialog.ShowModal() == wxID_OK )
			path->SetValue( dialog.GetPath() );
	}
	else		// it's a file 
	{
		wxFileDialog dialog( this,
							_T("Select a file to import"),
							wxGetApp().GetAppFrame()->GetFolderBrowser()->GetPath() );

		if( dialog.ShowModal() == wxID_OK )
		{
			path->SetValue( dialog.GetPath() );
		}
	}
}

void ImportDlg::OnRadio( wxCommandEvent& WXUNUSED(event) )
{
	if( path_type->GetSelection() == PATH_TYPE_TREE )
		recursive->Enable( TRUE );
	else
		recursive->Enable( FALSE );
}

void ImportDlg::InitializeData()
{	
	reposURL = (wxTextCtrl *)wxFindWindowByName( "reposURL", this );
	path = (wxTextCtrl *)wxFindWindowByName( "path", this );
	new_entry = (wxTextCtrl *)wxFindWindowByName( "new_entry", this );
	logMsg = (wxTextCtrl *)wxFindWindowByName( "logMsg", this );
	path_type = (wxRadioBox *)wxFindWindowByName( "path_type", this );
	user = (wxTextCtrl *)wxFindWindowByName( "user", this );
	pass = (wxTextCtrl *)wxFindWindowByName( "pass", this );
	recursive = (wxCheckBox *)wxFindWindowByName( "recursive", this );

	path_type->SetSelection( PATH_TYPE_TREE );
	recursive->SetValue( TRUE );
	reposURL->SetValue( wxString( _T("http://mtp/svn/repos/proj1") ) );
}
