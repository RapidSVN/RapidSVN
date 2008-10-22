/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
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
#ifndef _ANNOTATE_ACTION_H_INCLUDED_
#define _ANNOTATE_ACTION_H_INCLUDED_

// app
#include "action.hpp"
#include "annotate_data.hpp"
#include "annotate_dlg.hpp"

// forward declarations
namespace svn
{
  class StatusSel;
}

/**
 * This action downloads a specific revision
 * of a file (specified in @a GetData) and
 * displays the file
 */
class AnnotateAction:public Action
{
public:
  /**
   * Constructor. This constructor creates an
   * action, that will not prompt for
   * input in Prepare. Instead, the settings
   * in @a data will be used
   *
   * @param parent parent window
   * @param data update data
   */
  AnnotateAction(wxWindow * parent,
                 const AnnotateData & data);

  /** Desctructor */
  virtual ~AnnotateAction();

  virtual bool
  Perform();

  virtual bool
  Prepare();

  static bool
  CheckStatusSel(const svn::StatusSel & statusSel);


private:
  struct Data;
  Data * m;
  AnnotateDlg dlg;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
