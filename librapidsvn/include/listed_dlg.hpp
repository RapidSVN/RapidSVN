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

#ifndef _LISTED_DLG_H_INCLUDED_
#define _LISTED_DLG_H_INCLUDED_

// app
#include "rapidsvn_generated.h"

// forward declarations
class wxArrayString;
class wxListEvent;

class ListEditorDlg : public ListEditorDlgBase
{
public:
  ListEditorDlg(wxWindow * parent, const wxString & title);

  /**
   * destructor
   */
  virtual ~ListEditorDlg();

  void
  SetAddTitle(const wxString & title);

  void
  SetEditTitle(const wxString & title);

  void
  SetValueCaption(const wxString & caption);

  void
  SetNameCaption(const wxString & caption);

  void
  SetCaption(const wxString & caption);

  /**
   * fills the entries in the combo box for
   * the names
   */
  void
  SetNameTemplates(const wxArrayString & values);

  void
  DeleteAllEntries();

  long
  SetEntry(const wxString & name, const wxString & value);

  void
  GetEntryAtIndex(long id, wxString & name, wxString & value) const;

  long
  GetEntryCount() const;

  long
  FindEntry(const wxString & name) const;

  /**
   * enables or disables the saving/modification
   * of properties.
   */
  void
  SetReadOnly(bool value);

protected: // inherited event handlers
  virtual void
  OnNew(wxCommandEvent & event);

  virtual void
  OnEdit(wxCommandEvent & event);

  virtual void
  OnDelete(wxCommandEvent & event);

  virtual void
  OnSelected(wxListEvent & event);

private:
  struct Data;
  Data * m;

  /**
   * disallow default constructor
   */
  ListEditorDlg();

  /**
   * disallow copy constructor
   */
  ListEditorDlg(const ListEditorDlg &);

  /**
   * disallow assignment operator
   */
  ListEditorDlg &
  operator = (const ListEditorDlg &);

  void
  CheckControls();
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
