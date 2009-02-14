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
#ifndef _DND_DLG_H_INCLUDED_
#define _DND_DLG_H_INCLUDED_

// wxWidgets
#include "wx/wx.h"
#include "wx/dialog.h"

enum
{
  ID_DND_DLG=10018,
  ID_BUTTON_IMPORT,
  ID_BUTTON_COPY,
  ID_BUTTON_MOVE
};

class DragAndDropDialog : public wxDialog
{
public:
  DragAndDropDialog(wxWindow *parent, wxString src, wxString dest, bool bShowMove = true, bool bShowImport = false);
  int GetDecision();

  enum {
    RESULT_CANCEL = 1,
    RESULT_MOVE,
    RESULT_COPY,
    RESULT_IMPORT,
  };

  void
  OnImport(wxCommandEvent & event);

  void
  OnMove(wxCommandEvent & event);

  void
  OnCopy(wxCommandEvent & event);

  void
  OnCancel(wxCommandEvent & event);

private:
  void CreateControls();

  int m_decision;
  wxString m_src;
  wxString m_dest;
  bool m_showImport;
  bool m_showMove;

  DECLARE_EVENT_TABLE()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
