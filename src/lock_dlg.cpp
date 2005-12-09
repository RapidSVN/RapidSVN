/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
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

//svncpp
#include "svncpp/check.hpp"

// wxWidgets
#include "wx/wx.h"
#include "wx/valgen.h"

// app
#include "lock_dlg.hpp"

#if CHECK_SVN_SUPPORTS_LOCK

struct LockDlg::Data
{
public:
  bool stealLock;
  wxTextCtrl * msg;

  wxString message;

  Data (wxWindow * window)
    : stealLock (false), msg (0)
  {
    // create controls
    wxStaticBox* msgBox =
      new wxStaticBox(window, 1, _("Enter a comment for this lock"));

    wxSize msgSize (window->GetCharWidth () * 80,
                    window->GetCharHeight () * 4);

    msg = new wxTextCtrl (window, -1, wxEmptyString, wxDefaultPosition,
                          msgSize, wxTE_MULTILINE);

    wxCheckBox * checkStealLock = NULL;
    wxGenericValidator val (&stealLock);
    checkStealLock =
      new wxCheckBox (window, -1, _("Steal lock if it belongs to another user"),
                      wxDefaultPosition, wxDefaultSize, 0,
                      val);
    wxButton* ok =
      new wxButton (window, wxID_OK, _("OK" ));
    wxButton* cancel =
      new wxButton (window, wxID_CANCEL, _("Cancel"));

    // position controls
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // The message field:
    wxStaticBoxSizer *msgSizer =
      new wxStaticBoxSizer (msgBox, wxHORIZONTAL);
    msgSizer->Add (msg, 1, wxALL | wxEXPAND, 5);

    // The buttons:
    wxBoxSizer *buttonSizer = new wxBoxSizer (wxHORIZONTAL);
    buttonSizer->Add (checkStealLock, 1,
                      wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT,
                      10);
    buttonSizer->Add (ok, 0, wxALL, 10);
    buttonSizer->Add (cancel, 0, wxALL, 10);

    wxBoxSizer *topSizer = new wxBoxSizer (wxHORIZONTAL);
    topSizer->Add (msgSizer, 1, wxALL | wxEXPAND, 5);

    // Add all the sizers to the main sizer
    mainSizer->Add (topSizer, 1, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);

    window->SetAutoLayout (true);
    window->SetSizer (mainSizer);

    mainSizer->SetSizeHints (window);
    mainSizer->Fit (window);
  }
};

BEGIN_EVENT_TABLE (LockDlg, wxDialog)
END_EVENT_TABLE ()

LockDlg::LockDlg (wxWindow* parent)
  : wxDialog(parent, -1, _("Lock"),
             wxDefaultPosition, wxDefaultSize,
             wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (this);
  CentreOnParent ();
}

LockDlg::~LockDlg ()
{
  delete m;
}

const wxString &
LockDlg::GetMessage () const
{
  return m->message;
}

bool
LockDlg::GetStealLock () const
{
  return m->stealLock;
}

#endif

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
