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

#include "log_list.hpp"

// wxWidgets



BEGIN_EVENT_TABLE(LogList, wxListView)
  EVT_SIZE(LogList::OnSize)
  //EVT_LIST_COL_CLICK(-1, LogList::OnColClick)
END_EVENT_TABLE()

LogList::LogList(wxWindow * parent, wxWindowID id, const wxPoint& pos,
                                 const wxSize& size, long style,
                                 const wxValidator& validator,
                                 const wxString& name)
  : wxListView(parent, id, pos, size, style, validator, name)
{
  for(int b = 0; b < LogItemTypeCount; b++)
    categoryEnabled[b] = true; 
    
  // Initialize the log widget
  InsertColumn(0, _("Action"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE);
  InsertColumn(1, _("Message"), wxLIST_FORMAT_LEFT, wxLIST_AUTOSIZE_USEHEADER);

  // Initialize the list attrs
  errorItemAttr.SetTextColour(*wxRED);
}

/* virtual */ LogList::~LogList()
{
  DeleteAllItems();
}

void
LogList::DeleteAllItems()
{
  items.clear();
  displayedItems.clear();

  wxListView::DeleteAllItems();
  SetItemCount(0);
}

void
LogList::AppendItem(LogItemType type, const wxString &action, const wxString &message)
{
    // check if the last item was visible before (if so, we will autoscroll later)
    bool autoscroll = false;
    if(GetTopItem() >= GetItemCount() - GetCountPerPage())
        autoscroll = true;

    ItemInfo info;
    info.type = type;
    info.action = action;
    info.message = message;

    items.push_back(info);
    size_t newIndex = items.size()-1;
    if(categoryEnabled[type]) {
      // add to displayedItems, but only if it's visible
      displayedItems.push_back(newIndex);
      SetItemCount(displayedItems.size());

      if(autoscroll)
        EnsureVisible(displayedItems.size()-1);
    }
}

void LogList::SetItemFilter(LogItemType type, bool enabled)
{
    categoryEnabled[type] = enabled;
    // TODO rebuild displayed items and SetItemCount
}

bool LogList::GetItemFilter(LogItemType type) const
{
    return categoryEnabled[type];
}

wxListItemAttr *
LogList::OnGetItemAttr(long item) const
{
    const ItemInfo& actItem = GetActualItem(item);
    // error items use their own special style
    if(actItem.type == LogItem_Error)
        return &errorItemAttr;
    // all others use the default style
    return NULL;
}

wxString
LogList::OnGetItemText(long item, long column) const
{
    const ItemInfo& actItem = GetActualItem(item);
    if(column == 0)
        return actItem.action;
    else
        return actItem.message;
}

LogList::ItemInfo
LogList::GetActualItem(long item) const
{
    return items[displayedItems[item]];
}


void
LogList::OnSize(wxSizeEvent& event)
{
  // Resize the last column "Description" to fill remaining available space
  AutoSizeLastColumn();

  // Layout();
  //wxListView::OnSize(event);
  event.Skip();
}

/**
 * Automatically resize the last column to fill remaining space in ListView
 *
 * @return nothing
 */
void
LogList::AutoSizeLastColumn()
{
  if (GetColumnCount() > 0)
  {
    int width = GetSize().GetWidth();
    for (int column = 0; column < GetColumnCount() - 1; ++column)
    {
      width -= GetColumnWidth(column);
    }
    width -= 15;  // hacky guess of scrollbar width (may not be always present)
    if (width > 0)
    {
      SetColumnWidth(GetColumnCount() - 1, width);
    }
  }
}
