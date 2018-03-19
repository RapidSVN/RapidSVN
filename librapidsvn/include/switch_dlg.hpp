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
#ifndef _SWITCH_DLG_H_INCLUDED_
#define _SWITCH_DLG_H_INCLUDED_

// app
#include "rapidsvn_generated.h"

// forward declarations

namespace svn
{
  class Revision;
}

class SwitchDlg:public SwitchDlgBase
{
public:
  /**
   * constructor
   *
   * @param parent parent window
   * @param flags flags for the window (@a WITH_URL)
   * @param recursive default flag for the "recursive" check
   */
  SwitchDlg(wxWindow* parent, const wxString & url,
            bool recursive, bool relocate);

  /**
   * destructor
   */
  virtual ~SwitchDlg();

  const wxString &
  GetUrl() const;

  svn::Revision
  GetRevision() const;

  bool
  GetRecursive() const;

  bool
  GetRelocate() const;


protected: // event handlers form SwitchDlgBase
  virtual void
  OnUseLatest(wxCommandEvent &event);

  virtual void
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
