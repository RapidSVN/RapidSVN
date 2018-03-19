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
 * along with this program (in the file GPL.txt.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _UPDATE_DATA_H_INCLUDED_
#define _UPDATE_DATA_H_INCLUDED_

// wxWidgets
#include "wx/string.h"

struct UpdateData
{
  UpdateData()
    : revision(wxEmptyString), url(wxEmptyString),
      useLatest(true), depth(0), stickyDepth(true), ignoreExternals(false)
  {
  }

  wxString revision;
  wxString url;
  bool useLatest;
  int depth;
  bool stickyDepth;
  bool ignoreExternals;
};

// Possible values for the Depth item of the UpdateData structure.
// Keep this synchronized with the GUI's choice definition.
enum UpdateDepth
{
  UPDATE_WORKING_COPY = 0,
  UPDATE_FULLY_RECURSIVE = 1,
  UPDATE_IMMEDIATES = 2,
  UPDATE_FILES = 3,
  UPDATE_EMPTY = 4
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
