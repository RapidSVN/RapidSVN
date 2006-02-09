/*
 * ====================================================================
 * Copyright (c) 2002-2006 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxWidgets
#include "wx/wx.h"

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
  m_txtCtrl->AppendText (str + wxT("\n"));
  wxMutexGuiLeave ();
}


EventTracer::EventTracer (wxFrame * fr)
  : m_frame (fr)
{
}

void
EventTracer::Trace (const wxString & str)
{
  wxCommandEvent event = CreateActionEvent (TOKEN_INFO);
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
  m_msgs += str + wxT("\n");
}

void
ErrorTracer::ShowErrors ()
{
  ReportDlg rdlg (m_parent, _("Error"), m_msgs, ERROR_REPORT);
  rdlg.ShowModal ();
}

StringTracer::StringTracer (wxString & str)
  : m_msgs (str)
{
}

void
StringTracer::Trace (const wxString & str)
{
  m_msgs += str + wxT("\n");
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
