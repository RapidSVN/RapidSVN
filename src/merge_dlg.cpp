#include "include.h"
#include "rapidsvn_app.h"
#include "merge_dlg.h"

BEGIN_EVENT_TABLE(MergeDlg, wxDialog)
	EVT_BUTTON(ID_BUTTON_OK, MergeDlg::OnOk)
	EVT_BUTTON(ID_BUTTON_CANCEL, MergeDlg::OnCancel)
END_EVENT_TABLE()

static int TestRev( wxString &val )
{
	unsigned long	rev = 0;

	TrimString( val );

	if( !val.ToULong( &rev, 10) && rev < 0 )
	{
		// could not convert revision to a number
		wxMessageBox( _T("Revision must be an unsigned number!"),
						_T("Error"),
						wxOK | wxCENTRE | wxICON_ERROR );
		return -1;
	}

	return 0;
}

void MergeDlg::OnOk( wxCommandEvent& WXUNUSED(event) )
{
	wxString		val1;
	wxString		val2;

	// test revision numbers
	val1 = rev_start->GetValue();
	if( TestRev( val1 ) < 0 )
		return;

	val1 = rev_end->GetValue();
	if( TestRev( val1 ) < 0 )
		return;

	val1 = rev_second->GetValue();
	if( TestRev( val1 ) < 0 )
		return;
	
	val1 = path1->GetValue();
	TrimString( val1 );
	if( val1.IsEmpty() )
	{
		wxMessageBox( _T("First path or URL is required for merge!"),
						_T("Error"),
						wxOK | wxCENTRE | wxICON_ERROR );

		// Do not allow the user to continue if the path is empty 
		// and the import is addressing a file.
		return;
	}

	val1 = rev_start->GetValue();
	TrimString( val1 );
	val2 = rev_end->GetValue();
	TrimString( val2 );
	if( val1.IsEmpty() && !val2.IsEmpty() )
	{
		wxMessageBox( _T("You should also specify start revision for first path!"),
						_T("Error"),
						wxOK | wxCENTRE | wxICON_ERROR );

		// Do not allow the user to continue if the start revision is not set, 
		// but the end revision is.
		return;
	}

	// If start revision for first path is emtpy... 
	// (the end revision should be empty also in this case - checked above)
	if( val1.IsEmpty() )
	{
		// the second path should be specified, as now there is 
		// no deductible interval from the first path
		val2 = path2->GetValue();
		TrimString( val2 );
		if( val2.IsEmpty() )
		{
			wxMessageBox( _T("Second path or URL is required for merge!"),
							_T("Error"),
							wxOK | wxCENTRE | wxICON_ERROR );

			// Do not allow the user to continue if the path is empty 
			// and the import is addressing a file.
			return;
		}
	}

	EndModal( ID_BUTTON_OK );
}

void MergeDlg::OnCancel( wxCommandEvent& WXUNUSED(event) )
{
	EndModal( ID_BUTTON_CANCEL );
}

void MergeDlg::InitializeData()
{	
	path1 = (wxTextCtrl *)wxFindWindowByName( "path1", this );
	path2 = (wxTextCtrl *)wxFindWindowByName( "path2", this );

	rev_start = (wxTextCtrl *)wxFindWindowByName( "rev_start", this );
	rev_end = (wxTextCtrl *)wxFindWindowByName( "rev_end", this );
	rev_second = (wxTextCtrl *)wxFindWindowByName( "rev_second", this );
	
	user = (wxTextCtrl *)wxFindWindowByName( "user", this );
	pass = (wxTextCtrl *)wxFindWindowByName( "pass", this );

	recursive = (wxCheckBox *)wxFindWindowByName( "recursive", this );
	force = (wxCheckBox *)wxFindWindowByName( "force", this );

	recursive->SetValue( TRUE );
	force->SetValue( FALSE );
}
