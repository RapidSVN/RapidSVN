/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#include "include.h"
#include "rapidsvn_app.h"
#include "report_dlg.h"
#include "tracer.h"

TextCtrlTracer::TextCtrlTracer (wxTextCtrl * ctrl):m_txtCtrl (ctrl)
{
}

void
TextCtrlTracer::Trace (const wxString & str)
{
  wxMutexGuiEnter ();
  m_txtCtrl->AppendText (str + _T ("\n"));
  wxMutexGuiLeave ();
}


EventTracer::EventTracer (wxFrame * fr):frame (fr)
{
}

void
EventTracer::Trace (const wxString & str)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, ACTION_EVENT);
  event.SetInt (TOKEN_INFO);
  event.SetString (str);

  // send in a thread-safe way
  wxPostEvent (frame, event);
}

ErrorTracer::ErrorTracer (wxWindow * __parent):parent (__parent)
{
}

ErrorTracer::~ErrorTracer ()
{
}

void
ErrorTracer::Trace (const wxString & str)
{
  msgs += str + _T ("\n");
}

void
ErrorTracer::ShowErrors ()
{
  Report_Dlg rdlg (parent, _T ("Error"), msgs, ERROR_REPORT);
  rdlg.ShowModal ();
}

StringTracer::StringTracer (wxString & str):msgs (str)
{
}

void
StringTracer::Trace (const wxString & str)
{
  msgs += str + _T ("\n");
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
