/*
 * ====================================================================
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
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
#include "report_dlg.h"

BEGIN_EVENT_TABLE (Report_Dlg, wxDialog)
EVT_BUTTON (-1, Report_Dlg::OnButton)
END_EVENT_TABLE ()Report_Dlg::Report_Dlg (wxWindow * parent,
                                          const wxString & caption,
                                          const wxString & data, int type):
wxDialog (parent, -1, caption, wxDefaultPosition, wxDefaultSize,
          wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
str (data)
{
  text = new wxTextCtrl (this, -1, _T (""),
                         wxDefaultPosition,
                         wxSize (300, 100),
                         wxTE_MULTILINE | wxTE_READONLY | wxTE_RICH);
  text->SetMaxLength (0);

  if (type == ERROR_REPORT)
  {
    text->SetDefaultStyle (wxTextAttr (*wxRED));
  }
  text->WriteText (str);

  button = new wxButton (this, wxID_OK, "OK");

  wxBoxSizer *
    topsizer =
    new
    wxBoxSizer (wxVERTICAL);

  topsizer->Add (text, 1,       // make vertically stretchable
                 wxEXPAND |     // make horizontally stretchable
                 wxALL,         // and make border all around
                 5);            // set border width to 10

  wxBoxSizer *
    button_sizer =
    new
    wxBoxSizer (wxHORIZONTAL);

  button_sizer->Add (button, 0, // make horizontally unstretchable
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
Report_Dlg::OnButton (wxCommandEvent & event)
{
  event.Skip ();
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
