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
#include "wx/valgen.h"

// app
#include "destination_dlg.hpp"

struct DestinationDlg::Data
{
public:
  wxString destination;

  Data (wxWindow * window, const char * descr, const char * dst)
    : destination (dst)
  {
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText * label = 
      new wxStaticText (window, -1, descr);
    topSizer->Add (label, 0, wxALL, 5);

    // The "force" check box:
    wxTextValidator val (wxFILTER_NONE, &destination);
    wxTextCtrl * textDst = 
      new wxTextCtrl (window, -1, "", wxDefaultPosition, 
                      wxSize (200, -1), 0, val);
    
    // The buttons:
    buttonSizer->Add(new wxButton( window, wxID_OK, _("OK" )), 0, 
                     wxALL, 10);
    buttonSizer->Add(new wxButton( window, wxID_CANCEL, _("Cancel")), 0, 
                     wxALL, 10);

    // Add all the sizers to the main sizer
    mainSizer->Add (topSizer, 1, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add (textDst, 0, wxALL, 5);
    mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

    window->SetAutoLayout(true);
    window->SetSizer(mainSizer);

    mainSizer->SetSizeHints(window);
    mainSizer->Fit(window);
  }
};


BEGIN_EVENT_TABLE (DestinationDlg, wxDialog)
END_EVENT_TABLE ()

DestinationDlg::DestinationDlg(wxWindow* parent, const char * title,
                               const char * descr, const char * dst)
 : wxDialog(parent, -1, title,
            wxDefaultPosition, wxDefaultSize, 
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (this, descr, dst);
}

DestinationDlg::~DestinationDlg ()
{
  delete m;
}

const char *
DestinationDlg::GetDestination () const
{
  return m->destination;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
