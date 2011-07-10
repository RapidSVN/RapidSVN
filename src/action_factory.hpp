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
#ifndef _ACTION_FACTORY_H_
#define _ACTION_FACTORY_H_

// forward declarations
class Action;
class wxWindow;

namespace svn
{
  class StatusSel;
}

/**
 * this is a factory class that checks, whether
 * an @ref Action for a @a id is valid and next,
 * creates a new @ref Action instance (an inherited
 * @ref Action of course, like @ref CheckoutAction
 */
class ActionFactory
{
public:
  /**
   * check whether the selection of @a statusSel can
   * be used for the action identified by @a id
   *
   * @param id id of the action
   * @param targets
   * @retval true valid
   */
  static bool
  CheckIdForStatusSel(int id, const svn::StatusSel & statusSel);


  /**
   * creates a new action instance. The correct action is
   * derived from the id. @a ID_Checkout belongs to @ref CheckoutAction
   * and so on
   *
   * @param parent parent window for the new action
   * @param id the id of the action
   */
  static Action *
  CreateAction(wxWindow * parent, int id);
};


#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
