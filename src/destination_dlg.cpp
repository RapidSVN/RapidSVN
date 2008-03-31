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
#include "destination_dlg.hpp"
#include "hist_val.hpp"

struct DestinationDlg::Data
{
public:
  wxString destination;
  bool force;

  Data (wxWindow * window, const wxString & descr,
        int flags, const wxString & dest,
        const wxString history)
    : destination (dest), force (false)
  {
    bool withForce = (flags & WITH_FORCE) != 0;

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    // The description:
    wxStaticText * labelDescr =
      new wxStaticText (window, -1, descr);
    mainSizer->Add (labelDescr, 0, wxALL, 5);

    // The destination:
    if (wxEmptyString == history)
    {
      wxTextValidator val (wxFILTER_NONE, &destination);
      wxTextCtrl * textDest =
        new wxTextCtrl (window, -1, wxEmptyString, wxDefaultPosition,
                        wxSize (200, -1), 0, val);

      mainSizer->Add (textDest, 0, wxALL | wxEXPAND, 5);
    }
    else
    {
      HistoryValidator valDest (history, &destination, false, false);
      wxComboBox * comboDest =
        new wxComboBox (window, -1, wxEmptyString, wxDefaultPosition,
                        wxSize (200, -1), 0, 0, wxCB_DROPDOWN, valDest);

      mainSizer->Add (comboDest, 0, wxALL | wxEXPAND, 5);
    }
      

    // The force check
    if (withForce)
    {
      wxGenericValidator val (&force);
      wxCheckBox * check =
        new wxCheckBox (window, -1, _("Force"),
                        wxDefaultPosition, wxDefaultSize,
                        0, val);
      mainSizer->Add (check, 0, wxALL | wxALIGN_CENTER_HORIZONTAL);
    }

    // The buttons:
    wxButton * ok = new wxButton (window, wxID_OK, _("OK" ));
    buttonSizer->Add (ok, 0, wxALL, 10);

    wxButton * cancel = new wxButton (window, wxID_CANCEL, _("Cancel"));
    buttonSizer->Add (cancel, 0, wxALL, 10);

    // Add all the sizers to the main sizer
    mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

    window->SetAutoLayout(true);
    window->SetSizer(mainSizer);

    mainSizer->SetSizeHints(window);
    mainSizer->Fit(window);

    ok->SetDefault ();
  }
};

BEGIN_EVENT_TABLE (DestinationDlg, wxDialog)
END_EVENT_TABLE ()

const int DestinationDlg::WITH_FORCE=1;

const int DIALOG_FLAGS = wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER;

DestinationDlg::DestinationDlg (wxWindow* parent,
                                const wxString & title,
                                const wxString & descr,
                                const int flags,
                                const wxString & dst,
                                const wxString & history)
 : wxDialog(parent, -1, title,
            wxDefaultPosition, wxDefaultSize,
            DIALOG_FLAGS)
{
  m = new Data (this, descr, flags, dst, history);
}

DestinationDlg::DestinationDlg ()
  : wxDialog (), m (0)
{
}

DestinationDlg::~DestinationDlg ()
{
  if (m)
    delete m;
}

void
DestinationDlg::Create (wxWindow* parent, const wxString & title,
                        const wxString & descr, const int flags,
                        const wxString & dst,
                        const wxString & history)
{
  wxDialog::Create (parent, -1, title, wxDefaultPosition,
                    wxDefaultSize, DIALOG_FLAGS);

  m = new Data (this, descr, flags, dst, history);
}

const wxString &
DestinationDlg::GetDestination () const
{
  return m->destination;
}

bool
DestinationDlg::GetForce () const
{
  return m->force;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
