/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
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
#include "wx/wx.h"
//#include "wx/textctrl.h"
//#include "wx/button.h"
//#include "wx/sizer.h"

// app
#include "report_dlg.hpp"

BEGIN_EVENT_TABLE (ReportDlg, wxDialog)
EVT_BUTTON (-1, ReportDlg::OnButton)
END_EVENT_TABLE ()

ReportDlg::ReportDlg (wxWindow * parent,
                      const wxString & caption,
                      const wxString & data, int type)
  : wxDialog (parent, -1, caption, wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_str (data)
{
  m_text = new wxTextCtrl (this, -1, "",
                         wxDefaultPosition,
                         wxSize (500, 200),
                         wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);
  m_text->SetMaxLength (0);

  if (type == ERROR_REPORT)
  {
    m_text->SetDefaultStyle (wxTextAttr (*wxRED));
  }
  m_text->WriteText (m_str);

  m_button = new wxButton (this, wxID_OK, "OK");

  wxBoxSizer * topsizer = new wxBoxSizer (wxVERTICAL);

  topsizer->Add (m_text, 1,       // make vertically stretchable
                 wxEXPAND |     // make horizontally stretchable
                 wxALL,         // and make border all around
                 5);            // set border width to 10

  wxBoxSizer * button_sizer = new wxBoxSizer (wxHORIZONTAL);

  button_sizer->Add (m_button, 0, // make horizontally unstretchable
                     wxALL,     // make border all around (implicit top alignment)
                     5);        // set border width to 10

  topsizer->Add (button_sizer, 0,       // make vertically unstretchable
                 wxALIGN_CENTER);       // no border and centre horizontally


  SetAutoLayout (TRUE);         // tell dialog to use sizer
  SetSizer (topsizer);          // actually set the sizer

  topsizer->Fit (this);         // set size to minimum size as calculated by the sizer
  topsizer->SetSizeHints (this);        // set size hints to honour mininum size
}

void
ReportDlg::OnButton (wxCommandEvent & event)
{
  event.Skip ();
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
