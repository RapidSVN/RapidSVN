#ifndef _MKDIR_DLG_H_INCLUDED_
#define _MKDIR_DLG_H_INCLUDED_

#include "res/mkdir.h"

#if defined(__WXMSW__)
	static char *Mkdir_Dialog = NULL;
#else
#	include "res/mkdir.wxr"
#endif

class MkdirDlg
:	public wxDialog
{

public:
	wxTextCtrl		*target;
	wxTextCtrl		*user;
	wxTextCtrl		*pass;
	wxTextCtrl		*logMsg;

public:
	void OnOk( wxCommandEvent& event );
	void OnCancel( wxCommandEvent& event );

	void InitializeData();

private:
	DECLARE_EVENT_TABLE()
};

#endif