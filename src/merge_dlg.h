#ifndef _IMPORT_DLG_H_INCLUDED_
#define _IMPORT_DLG_H_INCLUDED_

#include "res/merge.h"

#if defined(__WXMSW__)
// Under Windows, some compilers can't include
// a whole .wxr file. import_dlg will point to the whole .wxr 'file'.
static char *Merge_Dialog = NULL;
#else
// Other platforms should have sensible compilers that
// cope with long strings.
#include "res/merge.wxr"
#endif


class MergeDlg
:	public wxDialog
{

public:
	wxTextCtrl		*path1;
	wxTextCtrl		*path2;

	wxTextCtrl		*rev_start;
	wxTextCtrl		*rev_end;
	
	wxTextCtrl		*rev_second;

	wxTextCtrl		*user;
	wxTextCtrl		*pass;
	wxCheckBox		*recursive;	
	wxCheckBox		*force;	

public:
	void OnOk( wxCommandEvent& event );
	void OnCancel( wxCommandEvent& event );

	void InitializeData();

private:
	DECLARE_EVENT_TABLE()
};

#endif