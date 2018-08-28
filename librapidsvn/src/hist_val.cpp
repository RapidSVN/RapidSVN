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
#include "wx/debug.h"
#include "wx/string.h"
#include "wx/validate.h"


// app
#include "hist_val.hpp"
#include "hist_mgr.hpp"
#include "utils.hpp"


IMPLEMENT_CLASS(HistoryValidator, wxValidator)

HistoryValidator::HistoryValidator(const wxString & settingName,
                                   wxString * value,
                                   bool dontUpdate,
                                   bool useMostRecent)
  :  wxValidator(), m_settingName(settingName), m_value(value),
     m_dontUpdate(dontUpdate), m_useMostRecent(useMostRecent)
{
}


HistoryValidator::HistoryValidator(const HistoryValidator & src)
  :  wxValidator()
{
  Copy(src);
}


bool
HistoryValidator::Copy(const HistoryValidator & val)
{
  wxValidator::Copy(val);
  m_settingName = val.m_settingName;
  m_value = val.m_value;
  m_dontUpdate = val.m_dontUpdate;
  m_useMostRecent = val.m_useMostRecent;

  return true;
}


bool
HistoryValidator::TransferToWindow()
{
  if (!m_validatorWindow)
    return false;

  // retrieve the list of recent entries
  const wxArrayString & list = TheHistoryManager.ReadList(m_settingName);

  // if we have a wxComboBox fill the list of values
  // with the recent entries and set the first value
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxComboBox)))
  {
    wxComboBox * comboBox = (wxComboBox *)m_validatorWindow;

    // clear and fill the combobox
    comboBox->Clear();

    // insert an entry with file selected to checkout
    if (m_value != 0)
    {
      if (!m_value->IsEmpty())
        comboBox->Append(*m_value);
    }

    wxArrayString::const_iterator it;

    for (it = list.begin(); it != list.end(); it++)
    {
      const wxString & value = *it;

      comboBox->Append(value);
    }

    if (!m_useMostRecent)
      comboBox->SetValue(wxEmptyString);
    else
    {
      if (m_value != 0)
      {
        if (comboBox->FindString(*m_value) != wxNOT_FOUND)
          comboBox->SetValue(*m_value);
      }

      // if we have an entry, select it
      if (!comboBox->IsListEmpty())
        comboBox->SetSelection(0);
    }

    return true;
  }
  else if (m_validatorWindow->IsKindOf(CLASSINFO(wxChoice)))
  {
    wxControlWithItems * control = (wxControlWithItems *)m_validatorWindow;

    // clear and fill the control
    control->Clear();

    if (m_value != 0)
    {
      if (!m_value->IsEmpty())
        control->Append(*m_value);
    }

    wxArrayString::const_iterator it;
    for (it = list.begin(); it != list.end(); it++)
    {
      const wxString & value = *it;

      if (wxNOT_FOUND == control->FindString(value))
        control->Append(value);
    }

    if (!m_useMostRecent)
      control->SetSelection(-1);
    else if (m_value != 0)
    {
      int i = control->FindString(*m_value);
      if (i != wxNOT_FOUND)
        control->SetSelection(i);
    }

    return true;
  }
  else if (m_validatorWindow->IsKindOf(CLASSINFO(wxTextCtrl)))
  {
    wxTextCtrl * textCtrl = (wxTextCtrl *)m_validatorWindow;

    if ((list.Count() == 0) || !m_useMostRecent)
      textCtrl->SetValue(wxT(""));
    else
    {
      textCtrl->SetValue(list.Item(0));

      // select all the text
      textCtrl->SetSelection(-1, -1);
    }

    return true;
  }


  // unrecoginzed control
  return false;
}


bool
HistoryValidator::TransferFromWindow()
{
  if (!m_validatorWindow)
    return false;

  // if we have a wxComboBox we use the last value
  if (m_validatorWindow->IsKindOf(CLASSINFO(wxComboBox)))
  {
    wxComboBox * comboBox = (wxComboBox *)m_validatorWindow;

    wxString lastValue(comboBox->GetValue());
    lastValue.Trim();

    if (!m_dontUpdate && (lastValue.Length() > 0))
      TheHistoryManager.AddEntryToList(m_settingName, lastValue);

    if (m_value)
      *m_value = lastValue;

    return true;
  }
  else if (m_validatorWindow->IsKindOf(CLASSINFO(wxChoice)))
  {
    wxControlWithItems * control = (wxControlWithItems *)m_validatorWindow;

    wxString lastValue(control->GetStringSelection());
    lastValue.Trim();

    if (!m_dontUpdate && !lastValue.IsEmpty())
      TheHistoryManager.AddEntryToList(m_settingName, lastValue);

    if (m_value)
      *m_value = lastValue;

    return true;
  }
  else if (m_validatorWindow->IsKindOf(CLASSINFO(wxTextCtrl)))
  {
    wxTextCtrl * textCtrl = (wxTextCtrl *)m_validatorWindow;

    wxString lastValue(textCtrl->GetValue());
    lastValue.Trim();

    if (!m_dontUpdate && (lastValue.Length() > 0))
      TheHistoryManager.AddEntryToList(m_settingName, lastValue);

    if (m_value)
      *m_value = lastValue;

    return true;
  }


  // unrecognized control
  return false;
};


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
