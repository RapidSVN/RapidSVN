/*
 * ====================================================================
 * Copyright (c) 2002-2006 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#ifndef _LISTED_DLG_H_INCLUDED_
#define _LISTED_DLG_H_INCLUDED_

// wxWidgets
#include "wx/dialog.h"
#include "wx/window.h"

// forward declarations
class wxListEvent;

class ListEditorDlg : public wxDialog
{
public:
  ListEditorDlg (wxWindow * parent, const wxString & title);

  /**
   * destructor
   */
  virtual ~ListEditorDlg ();

  void
  SetAddTitle (const wxString & title);

  void
  SetEditTitle (const wxString & title);

  void
  SetValueCaption (const wxString & caption);

  void
  SetNameCaption (const wxString & caption);

  void
  SetCaption (const wxString & caption);

  void
  DeleteAllEntries ();

  long
  SetEntry (const wxString & name, const wxString & value);

  void
  GetEntryAtIndex (long id, wxString & name, wxString & value) const;

  long
  GetEntryCount () const;

  long
  FindEntry (const wxString & name) const;

  virtual int
  ShowModal ();

  /**
   * enables or disables the saving/modification
   * of properties.
   */
  void
  SetReadOnly(bool value);

protected:
  virtual void
  ReadFromGrid () = 0;

  virtual void
  WriteToGrid () = 0;

private:
  struct Data;
  Data * m;

  /**
   * disallow default constructor
   */
  ListEditorDlg ();

  /**
   * disallow copy constructor
   */
  ListEditorDlg (const ListEditorDlg &);

  /**
   * disallow assignment operator
   */
  ListEditorDlg &
  operator = (const ListEditorDlg &);

  void
  CheckButtons ();

  void
  OnClose (wxCommandEvent & event);

  void
  OnOK (wxCommandEvent & event);

  void
  OnNew (wxCommandEvent & event);

  void
  OnEdit (wxCommandEvent & event);

  void
  OnDelete (wxCommandEvent & event);

  void
  OnSelected (wxListEvent & event);

private:
  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
