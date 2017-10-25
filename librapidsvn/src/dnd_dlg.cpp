/*
 * ====================================================================
 * Copyright (c) 2002-2012 The RapidSVN Group.  All rights reserved.
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

// app
#include "dnd_dlg.hpp"

DragAndDropDlg::DragAndDropDlg(wxWindow *parent, wxString src, wxString dest, int action)
  : DragAndDropDlgBase(parent)
{
  const wxString FMT_IMPORT(_("\
Are you sure that you want to import\n\
\n\
  %s\n\
\n\
into\n\
\n\
  %s?"));
  const wxString FMT_COPY_MOVE(_("\
Would you like to move or copy\n\
\n\
\n\
  %s\n\
\n\
into\n\
\n\
  %s?"));
  const wxString FMT_COPY(_("\
Would you like to copy\n\
\n\
\n\
  %s\n\
\n\
into\n\
\n\
  %s?"));

  wxString fmt;

  switch (action)
  {
  case IMPORT:
    SetTitle(_("Import"));
    fmt = FMT_IMPORT;

    m_buttonImport->Show(true);
    m_buttonCopy->Show(false);
    m_buttonMove->Show(false);

    break;
  case COPY:
    SetTitle(_("Copy"));
    fmt = FMT_COPY;

    m_buttonImport->Show(false);
    m_buttonCopy->Show(true);
    m_buttonMove->Show(false);

    break;

  default:
    SetTitle(_("Copy/Move"));
    fmt = FMT_COPY_MOVE;

    m_buttonImport->Show(false);
    m_buttonCopy->Show(true);
    m_buttonMove->Show(true);
  }

  m_staticQuestion->SetLabel(wxString::Format(fmt, src.c_str(), dest.c_str()));

  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  Layout();
  CentreOnParent();
}


void
DragAndDropDlg::OnButtonImport(wxCommandEvent & WXUNUSED(event))
{
  EndModal(RESULT_IMPORT);
}

void
DragAndDropDlg::OnButtonMove(wxCommandEvent & WXUNUSED(event))
{
  EndModal(RESULT_MOVE);
}

void
DragAndDropDlg::OnButtonCopy(wxCommandEvent & WXUNUSED(event))
{
  EndModal(RESULT_COPY);
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
