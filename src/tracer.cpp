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

// wxwindows
#include "wx/app.h"
#include "wx/textctrl.h"

// app
#include "ids.hpp"
#include "report_dlg.hpp"
#include "tracer.hpp"
#include "utils.hpp"

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


EventTracer::EventTracer (wxFrame * fr)
  : m_frame (fr)
{
}

void
EventTracer::Trace (const wxString & str)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, ACTION_EVENT);
  event.SetInt (TOKEN_INFO);
  event.SetString (str);

  // send in a thread-safe way
  wxPostEvent (m_frame, event);
}

ErrorTracer::ErrorTracer (wxWindow * parent)
  : m_parent (parent)
{
}

ErrorTracer::~ErrorTracer ()
{
}

void
ErrorTracer::Trace (const wxString & str)
{
  m_msgs += str + "\n";
}

void
ErrorTracer::ShowErrors ()
{
  ReportDlg rdlg (m_parent, _T ("Error"), m_msgs, ERROR_REPORT);
  rdlg.ShowModal ();
}

StringTracer::StringTracer (wxString & str)
  : m_msgs (str)
{
}

void
StringTracer::Trace (const wxString & str)
{
  m_msgs += str + "\n";
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
