#include "include.h"
#include "rapidsvn_app.h"



FolderBrowser::FolderBrowser( wxWindow* wnd,
						apr_pool_t *__pool,
						const wxWindowID id,
						const wxString& dir,
						const wxPoint& pos,
						const wxSize& size,
						long style, 
						const wxString& filter,
						int defaultFilter,
						const wxString& name )
:	wxGenericDirCtrl( wnd, id, dir, pos, size, style, filter, defaultFilter, name )
{
	pool = __pool;
}

void FolderBrowser::OnSelChanged(wxTreeEvent& event)
{
	// This should be propagated to the parent frame,
	// which will trigger the appropiate action.
	wxGetApp().GetAppFrame()->GetFileList()->UpdateFileList( GetPath() );
}

void FolderBrowser::OnKeyDown( wxTreeEvent& event )
{
	if( event.GetKeyEvent().GetKeyCode() == WXK_F5 )
	{
		// F5 was pressed, force a refresh
		wxGetApp().GetAppFrame()->GetFileList()->UpdateFileList( GetPath() );
	}
	else
	{
		event.Skip();
	}
}

BEGIN_EVENT_TABLE(FolderBrowser, wxGenericDirCtrl)
	EVT_TREE_SEL_CHANGED(-1, FolderBrowser::OnSelChanged)
	EVT_TREE_KEY_DOWN(-1,FolderBrowser::OnKeyDown) 
END_EVENT_TABLE()
