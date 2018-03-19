/*
 * ====================================================================
 * Copyright (c) 2002-2018 The RapidSVN Group.  All rights reserved.
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
 * along with this program (in the file GPL.txt).
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
#include "lock_dlg.hpp"

struct LockDlg::Data
{
public:
  bool stealLock;
  wxString message;

  Data()
    : stealLock(false)
  {
  }
};

LockDlg::LockDlg(wxWindow * parent)
  : LockDlgBase(parent)
{
  m = new Data();

  m_textMessage->SetSize(GetCharWidth() * 80,
                         GetCharHeight() * 4);
  m_textMessage->SetValidator(wxTextValidator(wxFILTER_NONE, &m->message));
  m_checkStealLock->SetValidator(wxGenericValidator(&m->stealLock));

  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  Layout();
  CentreOnParent();
}

LockDlg::~LockDlg()
{
  delete m;
}

const wxString &
LockDlg::GetMessage() const
{
  return m->message;
}

bool
LockDlg::GetStealLock() const
{
  return m->stealLock;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
