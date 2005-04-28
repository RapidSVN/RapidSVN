/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
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


IMPLEMENT_CLASS (HistoryValidator, wxValidator)

HistoryValidator::HistoryValidator (const wxString & settingName,
                                    wxString * value,
                                    bool dontUpdate)
 :  wxValidator (), m_settingName (settingName), m_value (value),
    m_dontUpdate (dontUpdate)
{
}


HistoryValidator::HistoryValidator (const HistoryValidator & src)
{
  Copy (src);
}


bool
HistoryValidator::Copy(const HistoryValidator & val)
{
  wxValidator::Copy (val);

  m_settingName = val.m_settingName;
  m_value = val.m_value;
  m_dontUpdate = val.m_dontUpdate;

  return true;
}


bool 
HistoryValidator::TransferToWindow()
{
  if (!m_validatorWindow)
    return false;

  // retrieve the list of recent entries
  const wxArrayString & list = TheHistoryManager.ReadList (m_settingName);

  // if we have a wxComboBox fill the list of values
  // with the recent entries and set the first value
  if (m_validatorWindow->IsKindOf (CLASSINFO (wxComboBox)))
  {
    wxComboBox * comboBox = (wxComboBox *)m_validatorWindow;

    // clear and fill the combobox
    comboBox->Clear ();

    size_t count = list.Count ();
    size_t index;

    for (index = 0; index < count; index++)
    {
      wxString value (list.Item (index));

      comboBox->Append (value);
    }

    if (count > 0)
    {
      wxString firstValue (list.Item (0));
      comboBox->SetValue (firstValue);
    }

    return true;
  }
  else if (m_validatorWindow->IsKindOf (CLASSINFO (wxTextCtrl)))
  {
    wxTextCtrl * textCtrl = (wxTextCtrl *)m_validatorWindow;

    if (list.Count () == 0)
      textCtrl->SetValue (wxT(""));
    else
    {
      textCtrl->SetValue (list.Item (0));

      // select all the text
      textCtrl->SetSelection (-1, -1);
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
  if (m_validatorWindow->IsKindOf (CLASSINFO (wxComboBox)))
  {
    wxComboBox * comboBox = (wxComboBox *)m_validatorWindow;

    wxString lastValue (comboBox->GetValue ());
    TrimString (lastValue);

    if (!m_dontUpdate && (lastValue.Length () > 0))
      TheHistoryManager.AddEntryToList (m_settingName, lastValue);

    if (m_value)
      *m_value = lastValue;

    return true;
  }
  else if (m_validatorWindow->IsKindOf (CLASSINFO (wxTextCtrl)))
  {
    wxTextCtrl * textCtrl = (wxTextCtrl *)m_validatorWindow;

    wxString lastValue (textCtrl->GetValue ());
    TrimString (lastValue);

    if (!m_dontUpdate && (lastValue.Length () > 0))
      TheHistoryManager.AddEntryToList (m_settingName, lastValue);

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
