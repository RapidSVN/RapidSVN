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
#include "revert_dlg.hpp"

struct RevertDlg::Data
{
public:
  bool recursive;

  Data(wxWindow * window)
      : recursive(false)
  {
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    wxStaticText * label =
      new wxStaticText(window, -1, _("Do you want to revert local changes?"));
    topSizer->Add(label, 0, wxALL, 5);

    // The "recursive" check box:
    wxCheckBox* check = new wxCheckBox(window, -1, _("Recursive"),
                                       wxDefaultPosition, wxDefaultSize, 0,
                                       wxGenericValidator(&recursive));

    // The buttons:
    wxButton * yes = new wxButton(window, wxID_OK, _("Yes"));
    buttonSizer->Add(yes, 0, wxALL, 10);

    wxButton * no = new wxButton(window, wxID_CANCEL, _("No"));
    buttonSizer->Add(no, 0, wxALL, 10);

    // Add all the sizers to the main sizer
    mainSizer->Add(topSizer, 1, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add(check, 0, wxALL | wxALIGN_CENTER_HORIZONTAL, 5);
    mainSizer->Add(buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

    window->SetAutoLayout(true);
    window->SetSizer(mainSizer);

    mainSizer->SetSizeHints(window);
    mainSizer->Fit(window);

    yes->SetDefault();
  }
};

BEGIN_EVENT_TABLE(RevertDlg, wxDialog)
END_EVENT_TABLE()

RevertDlg::RevertDlg(wxWindow* parent)
    : wxDialog(parent, -1, _("Revert"),
               wxDefaultPosition, wxDefaultSize,
               wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data(this);
  CentreOnParent();
}

RevertDlg::~RevertDlg()
{
  delete m;
}

bool
RevertDlg::GetRecursive() const
{
  return m->recursive;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
