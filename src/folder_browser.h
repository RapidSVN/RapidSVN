#ifndef _FOLDER_BROWSER_H_INCLUDED_
#define _FOLDER_BROWSER_H_INCLUDED_

#include <wx/generic/dirctrlg.h>

class FolderBrowser
:	public wxGenericDirCtrl
{
private:
	apr_pool_t			*pool;

public:
	FolderBrowser( wxWindow* wnd,
				apr_pool_t *__pool,
				const wxWindowID id = -1,
				const wxString& dir = wxDirDialogDefaultFolderStr,
				const wxPoint& pos = wxDefaultPosition,
				const wxSize& size = wxDefaultSize,
				long style = wxDIRCTRL_DIR_ONLY,
				const wxString& filter = wxEmptyString, 
				int defaultFilter = 0, 
				const wxString& name = wxTreeCtrlNameStr );

	void OnSelChanged(wxTreeEvent& event);
	void OnKeyDown( wxTreeEvent& event );		

private:
	DECLARE_EVENT_TABLE()
};

#endif
