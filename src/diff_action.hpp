/*
 * ====================================================================
 * Copyright (c) 2002-2011 The RapidSVN Group.  All rights reserved.
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
#ifndef _DIFF_ACTION_H_INCLUDED
#define _DIFF_ACTION_H_INCLUDED

// app
#include "action.hpp"

// forward declarations
namespace svn
{
  class StatusSel;
}
struct DiffData;

class DiffAction:public Action
{
public:
  /**
   * Constructor
   *
   * @param parent parent window
   */
  DiffAction(wxWindow * parent);

  /**
   * Constructor, that takes read configured @a DiffData,
   * so no dialog will be displayed
   *
   * @param parent parent window
   * @param data diff data
   */
  DiffAction(wxWindow * parent, DiffData & data);

  /** Destructor */
  virtual ~DiffAction();

  virtual bool
  Perform();

  virtual bool
  Prepare();

  static bool
  CheckStatusSel(const svn::StatusSel & statusSel);

private:
  struct Data;
  Data * m;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
