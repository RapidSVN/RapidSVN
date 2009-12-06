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

#ifndef _ENTRY_DLG_H_INCLUDED_
#define _ENTRY_DLG_H_INCLUDED_

// wxWidgets
#include "wx/wx.h"
#include "wx/grid.h"
#include "wx/listctrl.h"
#include "wx/valgen.h"

// app
#include "utils.hpp"


enum {
  ID_Name=wxID_HIGHEST,
  ID_Value
};

enum
{
  EDIT_NEW,
  EDIT_EDIT
};


/**
 * this dialog edits one single name/value pair
 */
class EntryDlg : public wxDialog
{
public:
  /**
   * Constructor
   */
  EntryDlg(wxWindow * parent, const wxString & title)
      : wxDialog(parent, -1, title, wxDefaultPosition),
      m_readOnly(false)
  {
    wxStaticText * labelName = new wxStaticText(this, -1, _("Name"));
    wxStaticText * labelValue = new wxStaticText(this, -1, _("Value"));
    wxTextCtrl * textName = new wxTextCtrl(this, ID_Name);

    wxTextCtrl * textValue =
      new wxTextCtrl(this, ID_Value, wxEmptyString, wxDefaultPosition,
                     wxSize(300, 100), wxTE_MULTILINE);

    wxFlexGridSizer * textSizer = new wxFlexGridSizer(2, 5, 5);
    textSizer->Add(labelName);
    textSizer->Add(textName, 1, wxEXPAND);
    textSizer->Add(labelValue);
    textSizer->Add(textValue, 1, wxEXPAND);

    wxButton * okButton = new wxButton(this, wxID_OK, _("OK"));
    wxButton * cancelButton = new wxButton(this, wxID_CANCEL, _("Cancel"));
    wxBoxSizer * buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(okButton, 0, wxALL, 5);
    buttonSizer->Add(cancelButton, 0, wxALL, 5);

    wxBoxSizer * mainSizer = new wxBoxSizer(wxVERTICAL);

    mainSizer->Add(textSizer, 1, wxALL | wxEXPAND, 5);
    mainSizer->Add(buttonSizer, 0, wxALL | wxALIGN_RIGHT , 5);

    okButton->SetDefault();

    SetAutoLayout(true);
    SetSizer(mainSizer);

    mainSizer->SetSizeHints(this);
    mainSizer->Fit(this);

    m_buttonOk = okButton;
    m_textName = textName;
    m_textValue = textValue;
    m_labelName = labelName;
    m_labelValue = labelValue;

    m_mode = EDIT_NEW;
  }

  /**
   * Execute the dialog
   *
   * @param caption caption of the dialog
   * @param value name of the property
   * @retval true if OK button was clicked
   */
  bool
  Execute(const int mode,
          wxString & name,
          wxString & value)
  {
    switch (mode)
    {
    case EDIT_NEW:
    case EDIT_EDIT:
      break;
    default:
      // invalid mode, exit
      return false;
    }
    m_mode = mode;

    SetReturnCode(wxID_CANCEL);
    m_textName->Enable(mode == EDIT_NEW);
    TrimString(name);
    TrimString(value);
    m_textName->SetValue(name);
    m_textValue->SetValue(value);

    CheckButtons();

    bool ok = ShowModal() == wxID_OK;

    if (ok)
    {
      name = m_textName->GetValue();
      value = m_textValue->GetValue();
      TrimString(name);
      TrimString(value);
    }

    return ok;
  }

  void SetNameCaption(const wxString & caption)
  {
    m_labelName->SetLabel(caption);
  }

  void SetValueCaption(const wxString & caption)
  {
    m_labelValue->SetLabel(caption);
  }

  void SetReadOnly(bool value)
  {
    m_readOnly = value;
    m_textName->Enable((m_mode == EDIT_NEW) && !value);
    m_textValue->Enable(!value);
    CheckButtons();
  }

private:
  bool m_readOnly;
  wxTextCtrl * m_textName;
  wxTextCtrl * m_textValue;
  wxStaticText * m_labelName;
  wxStaticText * m_labelValue;
  wxButton * m_buttonOk;
  int m_mode;

  void
  OnName(wxCommandEvent &)
  {
    CheckButtons();
  }

  void
  CheckButtons()
  {
    wxString name = m_textName->GetValue();
    TrimString(name);
    m_buttonOk->Enable((name.Length() > 0) && !m_readOnly);
  }

private:
  DECLARE_EVENT_TABLE()
};

BEGIN_EVENT_TABLE(EntryDlg, wxDialog)
  EVT_TEXT(ID_Name, EntryDlg::OnName)
END_EVENT_TABLE()


#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
