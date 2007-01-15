/*
 * ====================================================================
 * Copyright (c) 2002-2007 The RapidSvn Group.  All rights reserved.
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

// app
#include "dnd_dlg.hpp"


BEGIN_EVENT_TABLE (DragAndDropDialog, wxDialog)
  EVT_BUTTON (ID_BUTTON_IMPORT, DragAndDropDialog::OnImport)
  EVT_BUTTON (ID_BUTTON_MOVE, DragAndDropDialog::OnMove)
  EVT_BUTTON (ID_BUTTON_COPY, DragAndDropDialog::OnCopy)
  EVT_BUTTON (ID_BUTTON_CANCEL, DragAndDropDialog::OnCancel)
END_EVENT_TABLE ()

DragAndDropDialog::DragAndDropDialog (wxWindow *parent, wxString src, wxString dest, bool showMove, bool showImport)
: wxDialog(parent, -1, ((showImport) ? _("Import") : _("Copy/Move")),
             wxDefaultPosition, wxDefaultSize,
             wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m_decision = DragAndDropDialog::RESULT_CANCEL;
  m_src = src;
  m_dest = dest;
  m_showImport = showImport;
  m_showMove = showMove;

  CreateControls ();
}

void
DragAndDropDialog::CreateControls ()
{
  wxString msg = wxEmptyString;
  if (m_showImport)
  {
    msg = _("Are you sure that you want to import\n\n  \"") + m_src + _("\"\n\ninto\n\n  \"") + m_dest + _("\"?");
  }
  else
  {
    msg = _("Would you like to move or copy\n\n  \"") + m_src + _("\"\n\ninto\n\n  \"") + m_dest + _("\"?");
  }

  DragAndDropDialog* itemDialog1 = this;

  wxBoxSizer* itemBoxSizer2 = new wxBoxSizer(wxVERTICAL);
  itemDialog1->SetSizer(itemBoxSizer2);

  wxStaticText* itemStaticText3 = new wxStaticText( itemDialog1, wxID_STATIC, msg, wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer2->Add(itemStaticText3, 1, wxALIGN_CENTER_HORIZONTAL|wxALL|wxADJUST_MINSIZE, 5);

  itemBoxSizer2->AddSpacer (10);

  wxBoxSizer* itemBoxSizer4 = new wxBoxSizer(wxHORIZONTAL);
  itemBoxSizer2->Add(itemBoxSizer4, 0, wxALIGN_CENTER_HORIZONTAL|wxALL, 5);

  if (m_showImport)
  {
    wxButton* itemButton5 = new wxButton( itemDialog1, ID_BUTTON_IMPORT, _("Import"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemButton5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  }
  else
  {
    if (m_showMove)
    {
      wxButton* itemButton5 = new wxButton( itemDialog1, ID_BUTTON_MOVE, _("Move"), wxDefaultPosition, wxDefaultSize, 0 );
      itemBoxSizer4->Add(itemButton5, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
    }

    wxButton* itemButton6 = new wxButton( itemDialog1, ID_BUTTON_COPY, _("Copy"), wxDefaultPosition, wxDefaultSize, 0 );
    itemBoxSizer4->Add(itemButton6, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);
  }

  wxButton* itemButton7 = new wxButton( itemDialog1, ID_BUTTON_CANCEL, _("Cancel"), wxDefaultPosition, wxDefaultSize, 0 );
  itemBoxSizer4->Add(itemButton7, 0, wxALIGN_CENTER_VERTICAL|wxALL, 5);

  GetSizer()->Fit(this);
  GetSizer()->SetSizeHints(this);
}

void
DragAndDropDialog::OnImport (wxCommandEvent & event)
{
  m_decision = DragAndDropDialog::RESULT_IMPORT;
  EndModal (m_decision);
}

void
DragAndDropDialog::OnMove (wxCommandEvent & event)
{
  m_decision = DragAndDropDialog::RESULT_MOVE;
  EndModal (m_decision);
}

void
DragAndDropDialog::OnCopy (wxCommandEvent & event)
{
  m_decision = DragAndDropDialog::RESULT_COPY;
  EndModal (m_decision);
}

void
DragAndDropDialog::OnCancel (wxCommandEvent & event)
{
  m_decision = DragAndDropDialog::RESULT_CANCEL;
  EndModal (m_decision);
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
