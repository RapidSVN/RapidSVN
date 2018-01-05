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
#ifndef _CLEANUP_DLG_H_INCLUDED_
#define _CLEANUP_DLG_H_INCLUDED_

// app
#include "cleanup_data.hpp"
#include "rapidsvn_generated.h"

// svncpp
namespace svn
{
  class Path;
}

class CleanupDlg : public CleanupDlgBase
{
public:
  /**
   * Constructor
   *
   * @param parent parent window
   * @param selectedPath pre-fills the path field with
   *                    this path
   */
  CleanupDlg(wxWindow * parent, const svn::Path & selectedPath);

  /**
   * destructor
   */
  virtual ~CleanupDlg();

  /**
   * returns the cleanup data
   */
  const CleanupData &
  GetData() const;

protected: // Events inherited from CleanupDlgBase
  virtual void
  OnInitDialog(wxInitDialogEvent & event);
  virtual void
  OnCleanupChecked(wxCommandEvent & event);

  virtual void
  OnHelp(wxCommandEvent & event);

private:
  /** hide implementation details */
  struct Data;
  Data * m;

  void CheckControls();
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
