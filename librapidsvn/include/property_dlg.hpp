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

#ifndef _PROPERTY_DLG_H_INCLUDED_
#define _PROPERTY_DLG_H_INCLUDED_

// wxWidgets
#include "wx/dialog.h"
#include "wx/window.h"

// svncpp
#include "svncpp/property.hpp"

// app
#include "listed_dlg.hpp"

class PropertyDlg : public ListEditorDlg
{
public:
  PropertyDlg(wxWindow * parent, svn::Context * context,
              const svn::Path & target);

  /**
   * destructor
   */
  virtual ~PropertyDlg();

  virtual bool
  TransferDataFromWindow();

  virtual bool
  TransferDataToWindow();

private:
  struct Data;
  Data * m;

  /**
   * disallow default constructor
   */
  PropertyDlg();

  /**
   * disallow copy constructor
   */
  PropertyDlg(const PropertyDlg &);

  /**
   * disallow assignment operator
   */
  PropertyDlg operator & (const PropertyDlg &);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
