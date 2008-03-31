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
#ifndef _COMMIT_DLG_H_INCLUDED_
#define _COMMIT_DLG_H_INCLUDED_

// wxWidgets
#include "wx/dialog.h"

// forward declarations
class CommitDlg : public wxDialog
{
public:
  /**
   * constructor
   *
   * @param parent parent window
   */
  CommitDlg(wxWindow* parent, bool unexpectedCommit = false);

  /**
   * destructor
   */
  virtual ~CommitDlg ();

  /**
   * @return log message
   */
  const wxString &
  GetMessage () const;

  /**
   * @return recursive setting
   */
  bool
  GetRecursive () const;

  /**
   * @return keepLocks setting
   */
  bool
  GetKeepLocks () const;

private:
  /** hide implementation details */
  struct Data;
  Data * m;

  void OnHistoryComboBox (wxCommandEvent &);
private:
  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
