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
#ifndef _COMMIT_DLG_H_INCLUDED_
#define _COMMIT_DLG_H_INCLUDED_

// svncpp
#include "svncpp/path.hpp"

// app
#include "rapidsvn_generated.h"

class CommitDlg : public CommitDlgBase
{
public:
  /**
   * constructor
   *
   * @param parent parent window
   */
  CommitDlg(wxWindow* parent, 
            const svn::PathVector & filenames=svn::EmptyPathVector);

  /**
   * destructor
   */
  virtual ~CommitDlg();

  /**
   * @return log message
   */
  const wxString &
  GetMessage() const;

  /**
   * @return filenames
   */
  const svn::PathVector &
  GetSelectedFilenames() const;

  /**
   * @return recursive setting
   */
  bool
  GetRecursive() const;

  void SetRecursive(bool recursive);

  /**
   * @return keepLocks setting
   */
  bool
  GetKeepLocks() const;

  virtual bool TransferDataFromWindow();

private:
  bool m_recursive;
  bool m_keepLocks;
  bool m_usesFilenames;
  wxString m_message;
  svn::PathVector m_selectedFilenames;

  virtual void OnComboHistory(wxCommandEvent &);
  virtual void OnCheckListFilesDClick(wxCommandEvent& event);

  virtual void OnButtonToggle(wxCommandEvent &);

  /** disallow default constructor */
  CommitDlg();

  /** disallow copy constructor */
  CommitDlg(const CommitDlg & src);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
