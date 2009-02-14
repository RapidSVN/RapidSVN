/*
 * ====================================================================
 * Copyright (c) 2002-2009 The RapidSvn Group.  All rights reserved.
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
#include "wx/listctrl.h"

// app
#include "annotate_dlg.hpp"
#include "utils.hpp"

BEGIN_EVENT_TABLE(AnnotateDlg, wxDialog)
  EVT_BUTTON(-1, AnnotateDlg::OnButton)
END_EVENT_TABLE()

AnnotateDlg::AnnotateDlg(wxWindow * parent,
                         const wxString & caption)
    : wxDialog(parent, -1, caption, wxDefaultPosition, wxDefaultSize,
               wxDEFAULT_DIALOG_STYLE | wxMAXIMIZE_BOX | wxRESIZE_BORDER)
{
  m_button = new wxButton(this, wxID_OK, _("OK"));

  m_list = new wxListView(this, -1, wxDefaultPosition, wxSize(565, 450),
                          wxLC_REPORT);

  m_list->InsertColumn(0, _("Revision"), wxLIST_FORMAT_RIGHT);
  m_list->InsertColumn(1, _("Author"), wxLIST_FORMAT_RIGHT);
  m_list->InsertColumn(2, _("Line"), wxLIST_FORMAT_RIGHT);
  m_list->InsertColumn(3, wxT(""));  // Empty spacer column
  m_list->InsertColumn(4, wxT(""));

  // On Windows, the first column isn't properly right aligned
  //  in the previous InsertColumn call.  Here we set it again
  //  to be sure that it is right aligned
  wxListItem item;
  m_list->GetColumn(0, item);
  item.SetAlign(wxLIST_FORMAT_RIGHT);
  m_list->SetColumn(0, item);

  m_list->SetColumnWidth(0, 75);
  m_list->SetColumnWidth(1, 100);
  m_list->SetColumnWidth(2, 50);
  m_list->SetColumnWidth(3, 10);
  m_list->SetColumnWidth(4, 150);

  wxBoxSizer * topsizer = new wxBoxSizer(wxVERTICAL);

  topsizer->Add(m_list, 1,        // make vertically stretchable
                wxEXPAND |     // make horizontally stretchable
                wxALL,         // and make border all around
                5);            // set border width to 10

  wxBoxSizer * button_sizer = new wxBoxSizer(wxHORIZONTAL);

  button_sizer->Add(m_button, 0,  // make horizontally unstretchable
                    wxALL,     // make border all around (implicit top alignment)
                    5);        // set border width to 10

  topsizer->Add(button_sizer, 0,        // make vertically unstretchable
                wxALIGN_CENTER);       // no border and centre horizontally


  SetAutoLayout(TRUE);          // tell dialog to use sizer
  SetSizer(topsizer);           // actually set the sizer

  topsizer->Fit(this);          // set size to minimum size as calculated by the sizer
  topsizer->SetSizeHints(this);         // set size hints to honour mininum size

  Maximize();
}

void
AnnotateDlg::OnButton(wxCommandEvent & event)
{
  event.Skip();
}

void
AnnotateDlg::AddAnnotateLine(int revision, const wxString & author,
                             const wxString & line)
{
  int index = m_list->GetItemCount();
  wxString rev = wxString::Format(wxT("%ld"), (long) revision);

  m_list->InsertItem(index, rev);
  m_list->SetItem(index, 1, author);
  m_list->SetItem(index, 2, wxString::Format(wxT("%ld"), (long) index + 1));
  m_list->SetItem(index, 3, wxT(""));
  // Windows displays the link breaks as squares.  Format the line
  //  to avoid that
  wxString formattedLine = line;
  formattedLine.Replace(wxT("\r"), wxT(""));
  formattedLine.Replace(wxT("\n"), wxT(""));
  formattedLine.Replace(wxT("\t"), wxT(" "));
  m_list->SetItem(index, 4, formattedLine);
}

void
AnnotateDlg::AutoSizeColumn()
{
  m_list->SetColumnWidth(4, wxLIST_AUTOSIZE);
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
