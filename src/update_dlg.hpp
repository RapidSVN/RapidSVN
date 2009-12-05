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
#ifndef _UPDATE_DLG_H_INCLUDED_
#define _UPDATE_DLG_H_INCLUDED_

// app
#include "rapidsvn_generated.h"

// forward declarations
struct UpdateData;

class UpdateDlg:public UpdateDlgBase
{
public:
  enum
  {
    /** show dialog without "recursiv" checkbox */
    WITHOUT_RECURSIVE=0x1,
    
    /** show dialog with URL line */
    WITH_URL=0x2,

    /** show dialog without revision information */
    WITHOUT_REVISION=0x4
  };

  /**
   * constructor
   *
   * @param parent parent window
   * @param flags flags for the window (@a WITH_URL)
   * @param recursive default flag for the "recursive" check
   */
  UpdateDlg(wxWindow* parent, const wxString & title,
            int flags = 0, bool recursive = true);

  /**
   * destructor
   */
  virtual ~UpdateDlg();

  UpdateData &
  GetData();

protected: // events inherited from UpdateDlgBase
  void
  OnUseLatest(wxCommandEvent &event);

  void
  OnText(wxCommandEvent & event);

private:
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
