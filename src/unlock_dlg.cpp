/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.  
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxWidgets
#include "wx/wx.h"
#include "wx/valgen.h"

// app
#include "unlock_dlg.hpp"

struct UnlockDlg::Data
{
public:
  bool force;

  Data (wxWindow * window)
    : force (false)
  {
    wxBoxSizer * mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer * topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer * buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText * label =
      new wxStaticText (window, -1, _("Do you want to unlock the selected files/directories?"));
    topSizer->Add (label, 0, wxALL, 5);

    // The "force" check box:
    wxCheckBox * check =
      new wxCheckBox (window, -1,
                      _("Force unlocking even if you are not the lock owner"),
                      wxDefaultPosition, wxDefaultSize, 0,
                      wxGenericValidator(&force));
    // The buttons:
    wxButton * ok = new wxButton (window, wxID_OK, _("OK" ));
    buttonSizer->Add (ok, 0, wxALL, 10);

    wxButton * cancel = new wxButton (window, wxID_CANCEL, _("Cancel"));
    buttonSizer->Add (cancel, 0, wxALL, 10);

    // Add all the sizers to the main sizer
    mainSizer->Add (topSizer, 1, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add (check, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
    mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

    window->SetAutoLayout (true);
    window->SetSizer (mainSizer);

    mainSizer->SetSizeHints (window);
    mainSizer->Fit (window);

    ok->SetDefault ();
  }
};

BEGIN_EVENT_TABLE (UnlockDlg, wxDialog)
END_EVENT_TABLE ()

UnlockDlg::UnlockDlg (wxWindow * parent)
 : wxDialog (parent, -1, _("Unlock"),
             wxDefaultPosition, wxDefaultSize,
             wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (this);
}

UnlockDlg::~UnlockDlg ()
{
  delete m;
}

bool
UnlockDlg::GetForce () const
{
  return m->force;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
