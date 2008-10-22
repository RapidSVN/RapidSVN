/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
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
#ifndef _IMPORT_DLG_H_INCLUDED_
#define _IMPORT_DLG_H_INCLUDED_

// wxWidgets
#include "wx/dialog.h"

// forward declarations
struct ImportData;
// svncpp
namespace svn
{
  class Path;
}


class ImportDlg:public wxDialog
{
public:
  /**
   * Constructor
   *
   * @param parent window
   * @param selectedUrl Use this value to pre-fill the URL
   */
  ImportDlg(wxWindow *parent, const svn::Path & selectedUrl);

  /**
   * Destructor
   */
  virtual ~ImportDlg();

  /**
   * return the reference to @a ImportData
   */
  ImportData &
  GetData();

private:
  /** hide implementation details */
  struct Data;
  Data *m;

  /**
   * Event handler for OK button
   */
  void OnOk(wxCommandEvent & event);

  /**
   * Event handler for Browser button
   */
  void OnBrowse(wxCommandEvent & event);

  /**
   * Event handler for changes in text fields
   */
  void OnText(wxCommandEvent & event);

  DECLARE_EVENT_TABLE()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
