#include "include.h"
#include "rapidsvn_app.h"
#include "copy_dlg.h"

BEGIN_EVENT_TABLE(CopyDlg, wxDialog)
	EVT_BUTTON(ID_BUTTON_OK, CopyDlg::OnOk)
	EVT_BUTTON(ID_BUTTON_CANCEL, CopyDlg::OnCancel)
	EVT_BUTTON(ID_BUTTON_BROWSE_DEST, CopyDlg::OnBrowseDest)
	EVT_BUTTON(ID_BUTTON_BROWSE_SRC, CopyDlg::OnBrowseSrc)
END_EVENT_TABLE()

void CopyDlg::OnOk( wxCommandEvent& WXUNUSED(event) )
{
	wxString		val;
	unsigned long	rev = 0; 
	
	val = revision->GetValue();
	TrimString( val );

	if( !val.ToULong( &rev, 10) && rev < 0 )
	{
		// could not convert revision to a number
		wxMessageBox( _T("Revision must be an unsigned number!"),
						_T("Error"),
						wxOK | wxCENTRE | wxICON_ERROR );
		return;
	}

	EndModal( ID_BUTTON_OK );
}

void CopyDlg::OnCancel( wxCommandEvent& WXUNUSED(event) )
{
	EndModal( ID_BUTTON_CANCEL );
}

void CopyDlg::OnBrowseSrc( wxCommandEvent& event )
{
	wxDirDialog dialog( this, 
						_T("Select a source working copy"),
						wxGetApp().GetAppFrame()->GetFolderBrowser()->GetPath() );

	if( dialog.ShowModal() == wxID_OK )
		src->SetValue( dialog.GetPath() );
}

void CopyDlg::OnBrowseDest( wxCommandEvent& event )
{
	wxDirDialog dialog( this, 
						_T("Select a destination working copy"),
						wxGetApp().GetAppFrame()->GetFolderBrowser()->GetPath() );

	if( dialog.ShowModal() == wxID_OK )
		dest->SetValue( dialog.GetPath() );
}

void CopyDlg::InitializeData()
{
	dest = (wxTextCtrl *)wxFindWindowByName( "dest", this );
	src = (wxTextCtrl *)wxFindWindowByName( "src", this );
	logMsg = (wxTextCtrl *)wxFindWindowByName( "logMsg", this );
	user = (wxTextCtrl *)wxFindWindowByName( "user", this );
	pass = (wxTextCtrl *)wxFindWindowByName( "pass", this );
	revision = (wxTextCtrl *)wxFindWindowByName( "revision", this );
}