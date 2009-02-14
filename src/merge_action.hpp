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
#ifndef _MERGE_ACTION_H_INCLUDED_
#define _MERGE_ACTION_H_INCLUDED_

// svncpp
#include "svncpp/path.hpp"

// app
#include "action.hpp"
#include "merge_data.hpp"

// forward declarations
namespace svn
{
  class StatusSel;
}

class MergeAction:public Action
{
public:
  MergeAction(wxWindow * frame);

  /**
   * Constructor, that takes read configured @a MergeData
   * so dialog will be displayed with given data
   *
   * @param parent parent window
   * @param data merge data
   */
  MergeAction(wxWindow * frame, MergeData & data);

  virtual bool
  Prepare();

  virtual bool
  Perform();

  static bool
  CheckStatusSel(const svn::StatusSel & statusSel);

private:
  MergeData m_data;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
