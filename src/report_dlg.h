#ifndef _REPORT_DLG_H_INCLUDED_
#define _REPORT_DLG_H_INCLUDED_

enum
{
	NORMAL_REPORT,
	ERROR_REPORT
};

class Report_Dlg
:	public wxDialog
{
private:
	wxButton		*button;
	wxTextCtrl		*text;

	const wxString	&str;
public:
				Report_Dlg( wxWindow *parent, 
							const wxString& caption, 
							const wxString& data, int type = NORMAL_REPORT  );
				
	void		OnButton( wxCommandEvent& event );

private:
    DECLARE_EVENT_TABLE()
};

#endif