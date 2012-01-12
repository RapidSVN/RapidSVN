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

#ifndef _COLUMNS_HPP_
#define _COLUMNS_HPP_

#include "wx/string.h"
#include "ids.hpp"

struct Column
{
  const wxChar * name;
  const wxChar * caption;
  int id;

  Column()
      : name(wxT("")), caption(wxT("")), id(0)
  {
  }

  Column(const wxChar * name_, const wxChar * caption_, int id_)
      : name(name_), caption(caption_), id(id_)
  {
  }
};

struct Columns
{
  static const int SORT_COLUMN_OFFSET;

  Column COLUMNS[ID_ColumnSort_Min - ID_Column_Min];

  Columns();

  Column
  operator [](int index);
};

#endif

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
