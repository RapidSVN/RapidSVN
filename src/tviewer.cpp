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

// app
#include "tviewer.hpp"

struct TextViewer::Data
{
private:
  wxTextCtrl * m_text;
public:

  Data (wxWindow * window)
    : m_text (0)
  {
    // create controls
    // Text control:
    wxSize size (window->GetCharWidth () * 40, 
                 window->GetCharHeight () * 20);
    const long style = wxTE_MULTILINE | wxTE_READONLY;
    m_text = new wxTextCtrl (window, -1, "", wxDefaultPosition,
                             size, style);

    // Close Button
    wxButton * buttonClose = new wxButton (window, wxID_CANCEL, _("Close"));

    // position controls
    wxBoxSizer * sizer = new wxBoxSizer (wxVERTICAL);
    sizer->Add (m_text, 1, wxEXPAND);
    sizer->Add (buttonClose, 0, wxALIGN_RIGHT | wxALL, 5);

    window->SetAutoLayout(true);
    window->SetSizer(sizer);

    sizer->SetSizeHints(window);
    sizer->Fit(window);

  }

  void
  SetText (const char * text)
  {
    m_text->Show (false);
    m_text->SetValue (text);
    m_text->Show (true);
  }
};

BEGIN_EVENT_TABLE (TextViewer, wxDialog)
  EVT_BUTTON (wxID_CANCEL, TextViewer::OnCancel)
  EVT_CLOSE (TextViewer::OnClose)
END_EVENT_TABLE ()


TextViewer::TextViewer (const char * title)
  : wxDialog (NULL, -1, title,
              wxDefaultPosition, wxDefaultSize,
              wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER | 
              wxDIALOG_NO_PARENT)
{
  m = new Data (this);
  CentreOnParent();
}

TextViewer::~TextViewer ()
{
  delete m;
}

void
TextViewer::SetText (const char * text)
{
  m->SetText (text);
}

void 
TextViewer::OnCancel (wxCommandEvent &)
{
  Close (true);
}

void
TextViewer::OnClose (wxCloseEvent &)
{
  Destroy ();
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
