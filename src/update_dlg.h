#ifndef _UPDATE_DLG_H_INCLUDED_
#define _UPDATE_DLG_H_INCLUDED_

#include "res/update.h"

#if defined(__WXMSW__)
	static char *Update_Dialog = NULL;
#else
#	include "res/update.wxr"
#endif

class UpdateDlg
:	public wxDialog
{

public:
	wxTextCtrl		*user;
	wxTextCtrl		*pass;
	wxTextCtrl		*revision;

public:
	void OnOk( wxCommandEvent& event );
	void OnCancel( wxCommandEvent& event );
	void OnBrowse( wxCommandEvent& event );

	void InitializeData();

private:
	DECLARE_EVENT_TABLE()
};

#endif