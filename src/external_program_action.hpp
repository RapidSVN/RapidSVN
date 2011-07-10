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
#ifndef _EXTERNAL_PROGRAM_ACTION_HPP_INCLUDED_
#define _EXTERNAL_PROGRAM_ACTION_HPP_INCLUDED_

// app
#include "action.hpp"

// forward declarations
namespace svn
{
  class StatusSel;
}

class ExternalProgramAction:public Action
{
public:
  /**
   * Launch an external editor on the target
   *
   * @param verb_id         The index of the verb, found by using a Verblist,
   *                        or -1 to use whichever is the default.
   * @param treat_as_folder If true, the target will be treated like a folder
   *                        (typically: "explored"). If the target is a file,
   *                        then only it's path will be used.
   *                        If false, the target is assumed to be a file, and
   *                        the verb should be explicit.
   */
  ExternalProgramAction(wxWindow * parent, long verb_id,
                        bool treat_as_folder);

  virtual bool
  Perform();

  virtual bool
  Prepare();

  static bool
  CheckStatusSel(const svn::StatusSel & statusSel);

private:
  long m_verb_id;
  bool m_treat_as_folder;
  wxWindow * m_parent;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
