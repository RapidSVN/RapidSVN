#ifndef _COPY_DLG_H_INCLUDED_
#define _COPY_DLG_H_INCLUDED_

#include "res/copy.h"

#if defined(__WXMSW__)
	static char *Copy_Dialog = NULL;
#else
#	include "res/copy.wxr"
#endif

class CopyDlg
:	public wxDialog
{

public:
	wxTextCtrl		*dest;
	wxTextCtrl		*src;
	wxTextCtrl		*user;
	wxTextCtrl		*pass;
	wxTextCtrl		*logMsg;
	wxTextCtrl		*revision;

public:
	void OnOk( wxCommandEvent& event );
	void OnCancel( wxCommandEvent& event );
	void OnBrowseSrc( wxCommandEvent& event );
	void OnBrowseDest( wxCommandEvent& event );

	void InitializeData();

private:
	DECLARE_EVENT_TABLE()
};


#endif