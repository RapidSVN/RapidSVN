#include "include.h"
#include "rapidsvn_app.h"
#include "report_dlg.h"
#include "tracer.h"

TextCtrlTracer::TextCtrlTracer( wxTextCtrl* ctrl )
:	m_txtCtrl( ctrl )
{
}

void TextCtrlTracer::Trace( const wxString& str )
{
	wxMutexGuiEnter();
	m_txtCtrl->AppendText( str + _T("\n") );
	wxMutexGuiLeave();
}


EventTracer::EventTracer( wxFrame* fr )
:	frame( fr )
{
}

void EventTracer::Trace( const wxString& str )
{
	wxCommandEvent event( wxEVT_COMMAND_MENU_SELECTED, ACTION_EVENT );
	event.SetInt( TOKEN_INFO );
	event.SetString( str );

	// send in a thread-safe way
	wxPostEvent( frame, event );
}

ErrorTracer::ErrorTracer( wxWindow *__parent )
:	parent(__parent)
{
}

void ErrorTracer::Trace( const wxString& str )
{
	msgs += str + _T("\n");
}

void ErrorTracer::ShowErrors()
{
	Report_Dlg		rdlg( parent, _T("Error"), msgs, ERROR_REPORT );
	rdlg.ShowModal();
}

StringTracer::StringTracer( wxString& str )
:	msgs( str )
{
}

void StringTracer::Trace( const wxString& str )
{
	msgs += str + _T("\n");
}