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

#ifndef _ENTRY_DLG_H_INCLUDED_
#define _ENTRY_DLG_H_INCLUDED_

#include "rapidsvn_generated.h"


/**
 * this dialog edits one single name/value pair
 */
class EntryDlg : public EntryDlgBase
{
public:
  typedef enum {
    NEW,
    EDIT
  } EditMode;

  /**
   * Constructor
   */
  EntryDlg(wxWindow * parent, const wxString & title);

  void 
  SetNameCaption(const wxString & caption);

  void 
  SetValueCaption(const wxString & caption);

  void 
  SetReadOnly(bool value);

  void 
  SetEditMode(EditMode mode);

  virtual bool 
  TransferDataFromWindow();
  
  virtual bool 
  TransferDataToWindow();

  void
  SetNameValue(const wxString & name, const wxString & value);

  void
  GetNameValue(wxString & name, wxString & value) const;

  void
  SetNameTemplates(const wxArrayString & names);

protected: // inherited event handlers
  virtual void
  OnText(wxCommandEvent &);

private:
  struct Data;
  Data * m;

  void
  CheckControls();
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
