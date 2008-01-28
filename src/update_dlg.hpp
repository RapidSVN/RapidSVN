/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
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
#ifndef _UPDATE_DLG_H_INCLUDED_
#define _UPDATE_DLG_H_INCLUDED_

// wxWidgets
#include "wx/dialog.h"

// forward declarations
struct UpdateData;

class UpdateDlg:public wxDialog
{
public:
  /** show dialog without "recursiv" checkbox */
  static const int WITHOUT_RECURSIVE;

  /** show dialog with URL line */
  static const int WITH_URL;

  /** show dialog without revision information */
  static const int WITHOUT_REVISION;

  /**
   * constructor
   *
   * @param parent parent window
   * @param flags flags for the window (@a WITH_URL)
   * @param recursive default flag for the "recursive" check
   */
  UpdateDlg (wxWindow* parent,
             const wxString & title,
             int flags = 0,
             bool recursive = true);

  /**
   * destructor
   */
  virtual ~UpdateDlg ();

  UpdateData &
  GetData ();

  void InitDialog ();
private:
  struct Data;
  Data * m;

  void
  OnUseLatest(wxCommandEvent &event);

  void
  OnText (wxCommandEvent & event);

  DECLARE_EVENT_TABLE()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
