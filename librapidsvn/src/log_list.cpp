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
 * along with this program (in the file GPL.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#include "log_list.hpp"

#include "ids.hpp"

// wxWidgets
#include "wx/clipbrd.h"
#include "wx/accel.h"
#include "wx/filename.h"

BEGIN_EVENT_TABLE(LogList, wxListView)
  EVT_SIZE(LogList::OnSize)
  EVT_CONTEXT_MENU(LogList::OnContextMenu)
  EVT_MENU_RANGE(ID_LogList_SelectAll, ID_LogList_Copy, LogList::OnPopupClick)
END_EVENT_TABLE()


LogList::LogList(wxWindow * parent, wxWindowID id, const wxPoint& pos,
                 const wxSize& size, long style,
                 const wxValidator& validator,
                 const wxString& name)
  : wxListView(parent, id, pos, size, style, validator, name)
{
  for (int b = 0; b < LogItemTypeCount; b++)
    categoryEnabled[b] = true;

  // Initialize the log widget
  InsertColumn(0, _("Action"), wxLIST_FORMAT_LEFT);
  InsertColumn(1, _("Message"), wxLIST_FORMAT_LEFT);
  // Set initial column widths (will be optimized in OnSize())
  SetColumnWidth(0, GetTextExtent(wxString('n', 16)).GetWidth());
  SetColumnWidth(1, GetTextExtent(wxString('n', 32)).GetWidth());

  // Initialize the list attrs
  errorItemAttr.SetTextColour(*wxRED);

  // Initialize accelerators
  wxAcceleratorEntry entries[3];
  entries[0].Set(wxACCEL_CTRL, (int) 'A', ID_LogList_SelectAll);
  entries[0].Set(wxACCEL_CTRL, (int) 'B', ID_LogList_BrowseTo);
  entries[1].Set(wxACCEL_CTRL, (int) 'C', ID_LogList_Copy);
  wxAcceleratorTable accel(3, entries);
  SetAcceleratorTable(accel);
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

static void
CleanupLogString(wxString& s)
{
  wxString repl = "\t";
#ifdef __WXMSW__
  repl = "  "; // the Windows version cannot handle tabs
#endif

  s.Replace("\r", repl); // newlines, in general, make no sense here
  s.Replace("\n", repl);
#ifdef __WXMSW__
  s.Replace("\t", repl);
#endif
}

void
LogList::AppendItem(LogItemType type, const wxString &action, const wxString &message)
{
  // check if the last item was visible before (if so, we will autoscroll later)
  bool autoscroll = false;
  if (GetTopItem() >= GetItemCount() - GetCountPerPage())
    autoscroll = true;

  ItemInfo info;
  info.type = type;
  info.action = action;
  CleanupLogString(info.action);
  info.message = message;
  CleanupLogString(info.message);

  items.push_back(info);
  size_t newIndex = items.size()-1;
  if (categoryEnabled[type]) {
    // add to displayedItems, but only if it's visible
    displayedItems.push_back(newIndex);
    SetItemCount(displayedItems.size());

    if (autoscroll)
      EnsureVisible(displayedItems.size()-1);
  }
}

void LogList::SetItemFilter(LogItemType type, bool enabled)
{
  if (enabled != categoryEnabled[type])
  {
    categoryEnabled[type] = enabled;
    // check if need to scroll down later
    bool autoscroll = false;
    if (GetTopItem() >= GetItemCount() - GetCountPerPage())
      autoscroll = true;
    // rebuild displayed items
    displayedItems.clear();
    for (size_t i = 0; i < items.size(); i++)
    {
      if (categoryEnabled[items[i].type])
        displayedItems.push_back(i);
    }
    // set list control item count and autoscroll, if desired
    SetItemCount(displayedItems.size());
    RefreshItems(0, displayedItems.size()-1);
    if (autoscroll)
      EnsureVisible(displayedItems.size()-1);
  }
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
  if (actItem.type == LogItem_Error)
    return &errorItemAttr;
  // all others use the default style
  return NULL;
}

wxString
LogList::OnGetItemText(long item, long column) const
{
  const ItemInfo& actItem = GetActualItem(item);
  if (column == 0)
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


void LogList::OnPopupClick(wxCommandEvent &evt)
{
  //void *data=static_cast<wxMenu *>(evt.GetEventObject())->GetClientData();
  switch (evt.GetId()) {
  case ID_LogList_Copy:
  {
    wxString r;
    for (long l = GetFirstSelected(); l != -1; l = GetNextSelected(l))
      r << items[l].action << '\t' << items[l].message << '\n';
    // Write some text to the clipboard
    if (wxTheClipboard->Open())
    {
      // This data objects are held by the clipboard,
      // so do not delete them in the app.
      wxTheClipboard->SetData(new wxTextDataObject(r));
      wxTheClipboard->Close();
    }
    break;
  }
  case ID_LogList_SelectAll:
  {
    for (long i = 0; i < GetItemCount(); i++)
      Select(i, true);

  }
  break;

  default:
    evt.Skip();
    break;
  }
}

wxString LogList::GetSelectedFileOrDir()
{
  if (GetSelectedItemCount() == 1) {
    long selItem = GetFirstSelected();
    wxString text = GetItemText(selItem, 1);
    if (wxFileName::Exists(text)) {
      // convert the text to a native file or dir name using wxFileName
      wxFileName temp(text);
      return temp.GetFullPath();
    }
  }
  return wxEmptyString;
}

/*void LogList::OnListRightClick(wxListEvent &evt)
{
   void *data = reinterpret_cast<void *>(evt.GetItem().GetData());*/
void LogList::OnContextMenu(wxContextMenuEvent &evt)
{
  //void *data = reinterpret_cast<void *>(evt.GetItem().GetData());
  wxMenu mnu;
  //mnu.SetClientData( data );
  mnu.Append(ID_LogList_Copy, _("Copy selected\tCTRL-C"));
  mnu.Append(ID_LogList_SelectAll, _("Select all\tCTRL-A"));
  if (!GetSelectedFileOrDir().IsEmpty())
    mnu.Append(ID_LogList_BrowseTo, _("Browse to\tCTRL-B"));

  mnu.Connect(wxEVT_COMMAND_MENU_SELECTED, wxCommandEventHandler(LogList::OnPopupClick), NULL, this);
  PopupMenu(&mnu);
}
