#include "include.h"
#include "delete_dlg.h"

BEGIN_EVENT_TABLE(DeleteDlg, wxDialog)
	EVT_BUTTON(ID_BUTTON_OK, DeleteDlg::OnOk)
	EVT_BUTTON(ID_BUTTON_CANCEL, DeleteDlg::OnCancel)
END_EVENT_TABLE()

void DeleteDlg::OnOk( wxCommandEvent& WXUNUSED(event) )
{
	EndModal( ID_BUTTON_OK );
}

void DeleteDlg::OnCancel( wxCommandEvent& WXUNUSED(event) )
{
	EndModal( ID_BUTTON_CANCEL );
}

void DeleteDlg::InitializeData()
{
	user = (wxTextCtrl *)wxFindWindowByName( "user", this );
	pass = (wxTextCtrl *)wxFindWindowByName( "pass", this );
	logMsg = (wxTextCtrl *)wxFindWindowByName( "logMsg", this );
	force = (wxCheckBox *)wxFindWindowByName( "force", this );

	force->SetValue( FALSE );
}