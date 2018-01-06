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
#ifndef _LOG_LIST_H_INCLUDED_
#define _LOG_LIST_H_INCLUDED_

// stl
#include <vector>

// wx
#include "wx/wx.h"
#include "wx/listctrl.h"

// app
#include "utils.hpp"

enum LogItemType
{
	LogItem_Normal,
	LogItem_Error,
	LogItem_Added,
	LogItem_Deleted,
	LogItem_Conflicted,
	LogItem_Updated,
	LogItemTypeCount
}; 

class LogList : public wxListView
{
public:

public:
  LogList(wxWindow * parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize, long style = wxLC_REPORT|wxLC_VIRTUAL,
                  const wxValidator& validator = wxDefaultValidator,
                  const wxString& name = wxT("LogList"));

  virtual ~LogList();

  void
  DeleteAllItems();

  void
  AppendItem(LogItemType type, const wxString& action, const wxString& message); 
  
  void 
  SetItemFilter(LogItemType type, bool enabled);
  
  bool 
  GetItemFilter(LogItemType type) const; 

protected:
  virtual wxListItemAttr*
  OnGetItemAttr(long item) const;
  
  virtual wxString 
  OnGetItemText(long item, long column) const;

  void
  OnSize(wxSizeEvent &event);

private:
  struct ItemInfo {
	LogItemType type;
	wxString action;
	wxString message; 
  };
  std::vector<ItemInfo> items;
  std::vector<size_t> displayedItems; 
  bool categoryEnabled[LogItemTypeCount]; 
  mutable wxListItemAttr errorItemAttr;

  void RebuildDisplayedItems();
  ItemInfo GetActualItem(long item) const;
  void AutoSizeLastColumn();
private:

  DECLARE_EVENT_TABLE()
};


#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
