/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
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
#include "delete_dlg.hpp"

struct DeleteDlg::Data
{
public:
  bool force;

  Data (wxWindow * window)
    : force (false)
  {
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText * label = 
      new wxStaticText (window, -1, _("Do you want to delete the selected files/directories?"));
    topSizer->Add (label, 0, wxALL, 5);

    // The "force" check box:
    wxCheckBox* check = new wxCheckBox(window, -1, _("Force removal"),
      wxDefaultPosition, wxDefaultSize, 0,
      wxGenericValidator(&force));
    
    // The buttons:
    buttonSizer->Add(new wxButton( window, wxID_OK, _("OK" )), 0, 
                     wxALL, 10);
    buttonSizer->Add(new wxButton( window, wxID_CANCEL, _("Cancel")), 0, 
                     wxALL, 10);

    // Add all the sizers to the main sizer
    mainSizer->Add (topSizer, 1, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add (check, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
    mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

    window->SetAutoLayout(true);
    window->SetSizer(mainSizer);

    mainSizer->SetSizeHints(window);
    mainSizer->Fit(window);
  }
};


BEGIN_EVENT_TABLE (DeleteDlg, wxDialog)
END_EVENT_TABLE ()

DeleteDlg::DeleteDlg(wxWindow* parent)
 : wxDialog(parent, -1, _("Delete"),
            wxDefaultPosition, wxDefaultSize, 
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (this);
}

DeleteDlg::~DeleteDlg ()
{
  delete m;
}

bool
DeleteDlg::GetForce () const
{
  return m->force;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
