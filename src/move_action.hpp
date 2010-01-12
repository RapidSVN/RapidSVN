/*
 * ====================================================================
 * Copyright (c) 2002-2010 The RapidSvn Group.  All rights reserved.
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
#ifndef _MOVE_ACTION_H_INCLUDED_
#define _MOVE_ACTION_H_INCLUDED_

// svncpp
#include "svncpp/targets.hpp"

// app
#include "action.hpp"

// forward declarations
namespace svn
{
  class StatusSel;
}

enum
{
  MOVE_MOVE = 0,
  MOVE_COPY
};

/**
 * this action class can be used to copy, move and rename
 * files and folders. Right now it supports only a single target
 */
class MoveAction:public Action
{
public:
  /**
   * constructor
   *
   * @param parent parent window
   * @param type kind of action (MOVE_MOVE, MOVE_COPY)
   */
  MoveAction(wxWindow * parent, int kind);

  virtual bool
  Perform();

  virtual bool
  Prepare();

  static bool
  CheckStatusSel(const svn::StatusSel & statusSel);

private:
  int m_kind;
  wxString m_destination;
  bool m_force;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
