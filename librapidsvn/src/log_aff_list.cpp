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

#include "log_aff_list.hpp"

// wxWidgets
#include "wx/imaglist.h"

// Bitmaps
#include "res/bitmaps/sort_down.png.h"
#include "res/bitmaps/sort_up.png.h"

int
wxCALLBACK LogAffectedListColumnCompareFunction(long item1, long item2, long sortData);

/* static */ long LogAffectedList::COL_COUNT = 4;

/**
 * The index from where there will be only images
 * not related to the status.
 * See @a svn_wc_status_kind in @a svn_wc.h for more details.
 */
enum
{
  IMG_INDX_SORT_DOWN = 17,
  IMG_INDX_SORT_UP,
  IMG_INDX_COUNT
};

/**
 * structure that maps a status entry to an
 * according icon.
 */
struct MapItem
{
  int status;
  const unsigned char * data;
  size_t len;

  MapItem(int status_, const unsigned char *data_, size_t len_)
    : status(status_), data(data_), len(len_)
  {
  }
};

#define MAP_ITEM(status,data) MapItem(status, data, sizeof(data))

/** array of icons and corresponding status */
static const MapItem MAP_ICON_ARRAY [] =
{
  MAP_ITEM(IMG_INDX_SORT_DOWN,                  sort_down_png),
  MAP_ITEM(IMG_INDX_SORT_UP,                    sort_up_png),
};

static const size_t MAP_ICON_COUNT =
  sizeof(MAP_ICON_ARRAY) / sizeof(MAP_ICON_ARRAY [0]);

BEGIN_EVENT_TABLE(LogAffectedList, wxListView)
  EVT_LIST_COL_CLICK(-1, LogAffectedList::OnColClick)
END_EVENT_TABLE()

LogAffectedList::LogAffectedList(wxWindow * parent, wxWindowID id, const wxPoint& pos,
                                 const wxSize& size, long style,
                                 const wxValidator& validator,
                                 const wxString& name)
  : wxListView(parent, id, pos, size, style, validator, name),
    m_ColSortInfo(this, 1, true)
{
  m_ImageListSmall = new wxImageList(16, 16, TRUE);

  size_t i;
  for (i = 0; i < MAP_ICON_COUNT; i++)
  {
    const MapItem & item = MAP_ICON_ARRAY [i];

    m_ImageIndexArray [item.status] = i;
    m_ImageListSmall->Add(EmbeddedBitmap(item.data, item.len));
  }

  SetImageList(m_ImageListSmall, wxIMAGE_LIST_SMALL);

  InsertColumn(0, _("Action"));
  InsertColumn(1, _("Path"));
  InsertColumn(2, _("Copied from Path"));
  InsertColumn(COL_COUNT - 1, _("Copied from Rev"));

  SetColumnImages();
}

/* virtual */ LogAffectedList::~LogAffectedList()
{
  DeleteAllItems();

  delete m_ImageListSmall;
}

void
LogAffectedList::DeleteAllItems()
{
  // Delete the item data before deleting the items:
  while (GetItemCount() > 0)
    DeleteItem(0);

  wxListCtrl::DeleteAllItems();
}

void LogAffectedList::SetValue(const std::list<svn::LogChangePathEntry> & changedPaths)
{
  Freeze();

  try
  {
    DeleteAllItems();
    int i=0;
    int itemCount=0;
    char actionBuffer [2];
    actionBuffer [1] = 0;

    std::list<svn::LogChangePathEntry>::const_iterator it;

    for (it=changedPaths.begin(); it!=changedPaths.end(); it++)
    {
      const svn::LogChangePathEntry & changedPath = *it;
      actionBuffer [0] = changedPath.action;

      wxString label(Utf8ToLocal(actionBuffer));
      wxString copyFromRev(wxEmptyString);

      if (changedPath.copyFromRevision != -1)
        copyFromRev.Printf(wxT("%ld"), changedPath.copyFromRevision);

      InsertItem(i, label);
      SetItem(i, 1, Utf8ToLocal(changedPath.path.c_str()));
      SetItem(i, 2, Utf8ToLocal(changedPath.copyFromPath.c_str()));
      SetItem(i, COL_COUNT - 1, copyFromRev);
      SetItemData(i, i);

      i++;
    }
    itemCount = i;

    // now refresh the column width
    i=GetColumnCount();
    while (i-- > 0)
      SetColumnWidth(i, (i == 0) ? wxLIST_AUTOSIZE_USEHEADER : wxLIST_AUTOSIZE);

    // Sort the data according to current column sorting
    // Limit this automatic sorting to 1000 items, since it is quite slow. User can still sort by clicking on column headers.
    const int AUTOMATICALLY_SORT_ITEMS_LIMIT = 1000;
    if (itemCount <= AUTOMATICALLY_SORT_ITEMS_LIMIT)
    {
      m_ColSortInfo.Ascending = not m_ColSortInfo.Ascending;
      wxListEvent colClickEvent;
      if (m_ColSortInfo.Column == -1)
        m_ColSortInfo.Column = 1;
      colClickEvent.m_col = m_ColSortInfo.Column;
      OnColClick(colClickEvent);
    }
    else
    {
      m_ColSortInfo.Column = -1;
      SetColumnImages();
    }

    Thaw();
  }
  catch (...)
  {
    Thaw();

    throw;
  }
}

void
LogAffectedList::OnColClick(wxListEvent& event)
{
  int clickedColumn = event.GetColumn();

  if (m_ColSortInfo.Column == clickedColumn)
    m_ColSortInfo.Ascending = not m_ColSortInfo.Ascending;
  m_ColSortInfo.Column = clickedColumn;
  m_ColSortInfo.SortIterations = 0;

  SetColumnImages();

  { // Busy cursor scope
    wxBusyCursor busyCursor;

    SortItems(LogAffectedListColumnCompareFunction, (long)&m_ColSortInfo);
  }
}

void
LogAffectedList::SetColumnImages()
{
  // Update the column titles to reflect the sort column.
  for (int col = 0; col < COL_COUNT; col++)
  {
    wxListItem item;
    item.m_mask = wxLIST_MASK_IMAGE;
    if (col == m_ColSortInfo.Column)
    {
      item.m_image = GetSortImageIndex(m_ColSortInfo.Ascending);
    }
    else
    {
      item.m_image = -1;
    }
    SetColumn(col, item);
  }
}

/**
 * A safe wrapper for getting images for sorting.
 */
inline int
LogAffectedList::GetSortImageIndex(bool sortAscending)
{
  if (sortAscending)
    return m_ImageIndexArray[IMG_INDX_SORT_DOWN];

  return m_ImageIndexArray[IMG_INDX_SORT_UP];
}

int
wxCALLBACK LogAffectedListColumnCompareFunction(long item1, long item2, long sortData)
{
  if (item1 < 0 && item2 < 0)
    return 0;

  LogAffectedList::ColSortInfo* pColSortInfo = (LogAffectedList::ColSortInfo*)sortData;
  if (pColSortInfo->Parent == NULL)
    return 0;

  pColSortInfo->SortIterations++;

  long positionOfItem1 = pColSortInfo->Parent->FindItem(0L, item1);
  long positionOfItem2 = pColSortInfo->Parent->FindItem(0L, item2);

  wxListItem info1;
  info1.SetColumn(pColSortInfo->Column);
  info1.SetMask(wxLIST_MASK_TEXT);
  info1.SetId(positionOfItem1);
  if (!pColSortInfo->Parent->GetItem(info1))
    return 0;
  wxString itemText1 = info1.GetText();

  wxListItem info2;
  info2.SetColumn(pColSortInfo->Column);
  info2.SetMask(wxLIST_MASK_TEXT);
  info2.SetId(positionOfItem2);
  if (!pColSortInfo->Parent->GetItem(info2))
    return 0;
  wxString itemText2 = info2.GetText();

  long result = 0;

  if (itemText1.IsNumber() && itemText2.IsNumber())
  {
    long number1 = 0;
    itemText1.ToLong(&number1);
    long number2 = 0;
    itemText2.ToLong(&number2);
    if (number1 < number2)
      result = -1;
    else if (number1 > number2)
      result = 1;
    else
      result = 0;
  }
  else
  {
    result = itemText1.CmpNoCase(itemText2);
  }

  if (!pColSortInfo->Ascending)
    result = result * -1;

  return result;
}
