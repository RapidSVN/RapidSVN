/*
 * ====================================================================
 * Copyright (c) 2002-2010 The RapidSVN Group.  All rights reserved.
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
#ifndef _PREFERENCES_DLG_H_INCLUDED_
#define _PREFERENCES_DLG_H_INCLUDED_

// wxWidgets
#include "rapidsvn_generated.h"

// forward declarations
struct Preferences;

/**
 * Preferences dialog that displays the preference pages.
 */
class PreferencesDlg : public PreferencesDlgBase
{
public:
  /**
   * constructor
   */
  PreferencesDlg(wxWindow* parent, Preferences * prefs);

  /**
   * destructor
   */
  virtual ~PreferencesDlg();

  /**
   * A specialised version of TransferDataFromWindow that calls
   * TransferDataFromWindow for each panel in the dialog. Sadly this
   * is not the default base class behaviour.
   * @return true if transfer succeeded.
   */
  virtual bool TransferDataFromWindow();

  /**
   * A specialised version of TransferDataToWindow that calls
   * TransferDataToWindow for each panel in the dialog. Sadly this
   * is not the default base class behaviour.
   * @return true if transfer succeeded.
   */
  virtual bool TransferDataToWindow();

protected:
  virtual void OnButtonEditorClick(wxCommandEvent& event);
  virtual void OnButtonExplorerClick(wxCommandEvent& event);
  virtual void OnButtonDiffToolClick(wxCommandEvent& event);
  virtual void OnButtonMergeToolClick(wxCommandEvent& event);

private:
  /**
   * disallow default constructor
   */
  PreferencesDlg();

  /**
   * disallow copy constructor
   */
  PreferencesDlg(const PreferencesDlg &);

  /**
   * disallow assignment operator
   */
  PreferencesDlg &
  operator = (const PreferencesDlg & src);

  Preferences * m_prefs;

  bool SelectExecutable(const wxString & title, wxTextCtrl * textCtrl);
};

#endif // _PREFERENCES_DLG_H_INCLUDED_

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
