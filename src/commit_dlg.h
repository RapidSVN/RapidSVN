#ifndef _COMMIT_DLG_H_INCLUDED_
#define _COMMIT_DLG_H_INCLUDED_

#include "res/commit.h"

#if defined(__WXMSW__)
	static char *Commit_Dialog = NULL;
#else
#	include "res/commit.wxr"
#endif

class CommitDlg
:	public wxDialog
{

public:
	wxTextCtrl		*user;
	wxTextCtrl		*pass;
	wxTextCtrl		*logMsg;
	wxCheckBox		*recursive;

public:
	void OnOk( wxCommandEvent& event );
	void OnCancel( wxCommandEvent& event );

	void InitializeData();

private:
	DECLARE_EVENT_TABLE()
};

#endif