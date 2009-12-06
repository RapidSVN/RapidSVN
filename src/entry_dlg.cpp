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
#include "wx/valgen.h"

// app
#include "entry_dlg.hpp"
#include "utils.hpp"

struct EntryDlg::Data
{
public:
  bool readOnly;
  EditMode mode;
  wxString name;
  wxString value;

  Data()
    : readOnly(false), mode(NEW)
  {
  }
};


EntryDlg::EntryDlg(wxWindow * parent, const wxString & title)
  : EntryDlgBase(parent, -1, title)
{
  m = new Data();

  m_textName->SetValidator(wxGenericValidator(&m->name));
  m_textValue->SetValidator(wxGenericValidator(&m->value));
  
  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  Layout();
  CentreOnParent();

  CheckControls();
}


bool
EntryDlg::TransferDataToWindow()
{
  TrimString(m->name);
  TrimString(m->value);

  return EntryDlgBase::TransferDataToWindow();
}

bool
EntryDlg::TransferDataFromWindow()
{
  if (!EntryDlgBase::TransferDataFromWindow())
    return false;

  TrimString(m->name);
  TrimString(m->value);

  return true;
}


void 
EntryDlg::SetNameValue(const wxString & name, 
		       const wxString & value)
{
  m->name = name;
  m->value = value;
}

void
EntryDlg::GetNameValue(wxString & name,
		       wxString & value) const
{
  name = m->name;
  value = m->value;
}
  

void 
EntryDlg::SetNameCaption(const wxString & caption)
{
  m_staticName->SetLabel(caption);
}

void 
EntryDlg::SetValueCaption(const wxString & caption)
{
  m_staticValue->SetLabel(caption);
}

void 
EntryDlg::SetReadOnly(bool value)
{
  m->readOnly = value;

  CheckControls();
}

void 
EntryDlg::SetEditMode(EditMode mode)
{
  m->mode = mode;

  CheckControls();
}

void
EntryDlg::CheckControls()
{
  wxString name = m_textName->GetValue();
  TrimString(name);

  bool ok = true;

  EnableCtrl(m_textName, !m->readOnly && (m->mode != EDIT));
  EnableCtrl(m_textValue, !m->readOnly);

  if (name.IsEmpty())
    ok = false;

  EnableCtrl(m_buttonOK, !m->readOnly && ok);

}


void
EntryDlg::OnText(wxCommandEvent &)
{
  CheckControls();
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
