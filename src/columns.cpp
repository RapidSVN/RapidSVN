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

#include "wx/wx.h"
#include "wx/string.h"

// app
#include "columns.hpp"
#include "ids.hpp"

const int
Columns::SORT_COLUMN_OFFSET = ID_ColumnSort_Max - ID_Column_Max;

Columns::Columns ()
{
  /**
   * ATTENTION!
   *
   * Don't forget to update ID_Column_* and ID_ColumnSort_*
   * whenever changing one of them or making changes to
   * the order of column in src/columns.cpp.
   * This order should be the same in all 3 cases.
   */
  COLUMNS[0]  = Column (wxT("Name"),         _("Name"),          ID_Column_Name); // cannot show/hide name
  COLUMNS[1]  = Column (wxT("Path"),         _("Path"),          ID_Column_Path); // cannot show/hide path
  COLUMNS[2]  = Column (wxT("Revision"),     _("Revision"),      ID_Column_Rev);
  COLUMNS[3]  = Column (wxT("RepRev"),       _("Rep. Rev."),     ID_Column_Cmt_Rev);
  COLUMNS[4]  = Column (wxT("Author"),       _("Author"),        ID_Column_Author);
  COLUMNS[5]  = Column (wxT("Status"),       _("Status"),        ID_Column_Text_Status);
  COLUMNS[6]  = Column (wxT("PropStatus"),   _("Prop Status"),   ID_Column_Prop_Status);
  COLUMNS[7]  = Column (wxT("LastChanged"),  _("Last Changed"),  ID_Column_Cmt_Date);
  COLUMNS[8]  = Column (wxT("Extension"),    _("Extension"),     ID_Column_Extension);
  COLUMNS[9]  = Column (wxT("Date"),         _("Date"),          ID_Column_Text_Time);
  COLUMNS[10] = Column (wxT("PropDate"),     _("Prop Date"),     ID_Column_Prop_Time);
  COLUMNS[11] = Column (wxT("LockOwner"),    _("Lock Owner"),    ID_Column_LockOwner);
  COLUMNS[12] = Column (wxT("LockComment"),  _("Lock Comment"),  ID_Column_LockComment);
  COLUMNS[13] = Column (wxT("Checksum"),     _("Checksum"),      ID_Column_Checksum);
  COLUMNS[14] = Column (wxT("URL"),          _("URL"),           ID_Column_Url);
  COLUMNS[15] = Column (wxT("Repository"),   _("Repository"),    ID_Column_Repos);
  COLUMNS[16] = Column (wxT("UUID"),         _("UUID"),          ID_Column_Uuid);
  COLUMNS[17] = Column (wxT("Schedule"),     _("Schedule"),      ID_Column_Schedule);
  COLUMNS[18] = Column (wxT("Copied"),       _("Copied"),        ID_Column_Copied);
  COLUMNS[19] = Column (wxT("ConflictOld"),  _("Conflict BASE"), ID_Column_ConflictOld);
  COLUMNS[20] = Column (wxT("ConflictNew"),  _("Conflict HEAD"), ID_Column_ConflictNew);
  COLUMNS[21] = Column (wxT("ConflictWork"), _("Conflict Work"), ID_Column_ConflictWrk);
}

Column
Columns::operator [](int index)
{
  return COLUMNS[index];
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
