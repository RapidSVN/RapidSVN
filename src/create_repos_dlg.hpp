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
#ifndef _CREATE_REPOS_DLG_H_INCLUDED_
#define _CREATE_REPOS_DLG_H_INCLUDED_

// app
#include "rapidsvn_generated.h"


class CreateReposDlg : public CreateReposDlgBase
{
public:
  /**
   * Constructor
   *
   * @param parent window
   */
  CreateReposDlg(wxWindow *parent);

  /**
   * Destructor
   */
  virtual ~CreateReposDlg();

  enum
  {
    TYPE_FSFS=0,
    TYPE_BDB
  };

  enum
  {
    COMPAT_DEFAULT=0,
    COMPAT_PRE_1_6,
    COMPAT_PRE_1_5,
    COMPAT_PRE_1_4
  };

  virtual bool TransferDataFromWindow();
  virtual bool TransferDataToWindow();

protected: // CreateReposDlgBase event handlers
	virtual void OnComboType(wxCommandEvent& event);
	virtual void OnComboDirText(wxCommandEvent& event);
	virtual void OnButtonBrowseDirClick(wxCommandEvent& event);
	virtual void OnComboNameText(wxCommandEvent& event);
	virtual void OnComboCompatibility(wxCommandEvent& event);
	virtual void OnComboConfigDirText(wxCommandEvent& event);
	virtual void OnButtonBrowseConfigDirClick(wxCommandEvent& event);

private:
  /** hide implementation details */
  struct Data;
  Data *m;

  void CheckValues();
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
