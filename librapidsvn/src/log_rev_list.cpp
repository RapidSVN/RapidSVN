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

// wxWidgets
#include "wx/wx.h"
#include "wx/imaglist.h"

#include "log_rev_list.hpp"

// Bitmaps
#include "res/bitmaps/sort_down.png.h"
#include "res/bitmaps/sort_up.png.h"

int
wxCALLBACK LogRevListColumnCompareFunction(long item1, long item2, long sortData);

/* static */ long LogRevList::COL_COUNT = 4;


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


BEGIN_EVENT_TABLE(LogRevList, wxListView)
  EVT_SIZE(LogRevList::OnSize)
  EVT_LIST_COL_CLICK(-1, LogRevList::OnColClick)
END_EVENT_TABLE()

LogRevList::LogRevList(wxWindow * parent, wxWindowID id, const wxPoint& pos,
                       const wxSize& size, long style,
                       const wxValidator& validator,
                       const wxString& name)
  : wxListView(parent, id, pos, size, style, validator, name),
    m_ColSortInfo(this, 0, false)
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

  InsertColumn(0, _("Revision"));
  InsertColumn(1, _("User"));
  InsertColumn(2, _("Date"));
  InsertColumn(COL_COUNT - 1, _("Log Message"));

  SetColumnWidth(0, 65);
  SetColumnWidth(1, 100);
  SetColumnWidth(2, 150);
  SetColumnWidth(COL_COUNT - 1, 200);

  SetColumnImages();
}

LogRevList::~LogRevList()
{
  DeleteAllItems();

  delete m_ImageListSmall;
}

void
LogRevList::SetEntries(svn::LogEntries * entries)
{
  DeleteAllItems();

  if (entries == 0)
    return;

  AddEntriesToList(entries);
}

void
LogRevList::AddEntriesToList(svn::LogEntries * entries)
{
  if (entries == 0)
    return;

  long index=0;
  if (GetItemCount() > 0)
    index = GetItemCount();

  svn::LogEntries::const_iterator it;
  for (it=entries->begin(); it != entries->end(); it++)
  {
    const svn::LogEntry & entry = *it;
    wxString rev;
    wxString dateStr(FormatDateTime(entry.date));

    rev.Printf(wxT("%ld"), (long) entry.revision);

    InsertItem(index, rev);
    SetItem(index, 1, Utf8ToLocal(entry.author.c_str()));
    SetItem(index, 2, dateStr);
    SetItem(index, COL_COUNT - 1, NewLinesToSpaces(Utf8ToLocal(entry.message.c_str())));
    SetItemData(index, index);
    index++;
  }
}

/**
 * Returns the revision for the given @a item
 *
 * @param item
 * @return revnum
 * @retval -1 not found/error
 */
svn_revnum_t
LogRevList::GetRevisionForItem(long item) const
{
  wxListItem info;
  info.m_itemId = item;
  info.m_col = 0;
  info.m_mask = wxLIST_MASK_TEXT;

  if (!GetItem(info))
    return -1;

  svn_revnum_t revnum=-1;
  info.m_text.ToLong(&revnum);
  return revnum;
}

/**
 * returns the selected revision.
 *
 * @return selected revision
 * @retval -1 if nothing was selected or the cell
 *            contains an invalid string
 */
svn_revnum_t
LogRevList::GetSelectedRevision() const
{
  long item = GetNextItem(-1, wxLIST_NEXT_ALL,
                          wxLIST_STATE_SELECTED);
  if (item == -1)
    return -1;

  return GetRevisionForItem(item);
}

/**
 * returns the prior revision.
 * Like @a GetSelectedRevision, but returns revision prior to the one passed
 *
 * @return if none found then return -1
 */
svn_revnum_t
LogRevList::GetPriorRevision(const svn_revnum_t revnum) const
{
  long item = -1;

  do
  {
    item = GetNextItem(item, wxLIST_NEXT_BELOW,
                       wxLIST_STATE_DONTCARE);
    if (item != -1)
    {
      svn_revnum_t revnumTemp(GetRevisionForItem(item));
      if (revnum == revnumTemp)
      {
        item = GetNextItem(item, wxLIST_NEXT_BELOW,
                           wxLIST_STATE_DONTCARE);
        if (item != -1)
        {
          svn_revnum_t revnumPrior(GetRevisionForItem(item));
          return revnumPrior;
        }
      }
    }
  }
  while (item != -1);

  return -1;
}

/**
 * returns the selected revisions.
 * Like @a GetSelectedRevision, but can return
 * more revisions at once.
 *
 * @return if nothing is selected, an empty array
 *         will be returned
 */
RevnumArray
LogRevList::GetSelectedRevisions() const
{
  RevnumArray array;
  long item = -1;

  do
  {
    item = GetNextItem(item, wxLIST_NEXT_ALL,
                       wxLIST_STATE_SELECTED);
    if (item != -1)
    {
      svn_revnum_t revnum(GetRevisionForItem(item));

      array.Add(revnum);
    }
  }
  while (item != -1);

  return array;
}

/**
 * returns the last revision.
 *
 * @return last revision
 * @retval -1 if nothing was selected or the cell
 *            contains an invalid string
 */
svn_revnum_t
LogRevList::GetLastRevision() const
{
  int count = GetItemCount();
  if (count == 0)
    return -1;

  long item = GetNextItem(count - 2, wxLIST_NEXT_BELOW,
                          wxLIST_STATE_DONTCARE);
  if (item == -1)
    return -1;

  return GetRevisionForItem(item);
}

void
LogRevList::DeleteAllItems()
{
  // Delete the item data before deleting the items:
  while (GetItemCount() > 0)
    DeleteItem(0);

  wxListCtrl::DeleteAllItems();
}

wxString
LogRevList::NewLinesToSpaces(const wxString& str)
{
  /*
  wxString result = str;
  result.Replace (wxT("\n"), wxT(" "));
  return result;
  */

  wxString result;
  if (str.Length() == 0)
  {
    return result;
  }

  result.Alloc(str.Length());
  wxChar last = 0;

  for (size_t idx = 0; idx < str.Length(); idx++)
  {
    wxChar s = str[idx];
    switch (s)
    {
    case wxT('\r'):
    case wxT('\n'):
      if (last != wxChar(' '))
      {
        result += wxChar(' ');
      }
      break;

    default:
      result += str[idx];
      break;
    }
  }

  return result;
}

void
LogRevList::OnSize(wxSizeEvent& event)
{
  // Resize the last column "Description" to fill remaining available space
  AutoSizeLastColumn();

  // Layout();
  wxListView::OnSize(event);
}

/**
 * Automatically resize the last column to fill remaining space in ListView
 *
 * @return nothing
 */
void
LogRevList::AutoSizeLastColumn()
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

void
LogRevList::OnColClick(wxListEvent& event)
{
  int clickedColumn = event.GetColumn();

  if (m_ColSortInfo.Column == clickedColumn)
    m_ColSortInfo.Ascending = not m_ColSortInfo.Ascending;
  m_ColSortInfo.Column = clickedColumn;

  SetColumnImages();
  SortItems(LogRevListColumnCompareFunction, (long)&m_ColSortInfo);
}

void
LogRevList::SetColumnImages()
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
LogRevList::GetSortImageIndex(bool sortAscending)
{
  if (sortAscending)
    return m_ImageIndexArray[IMG_INDX_SORT_DOWN];

  return m_ImageIndexArray[IMG_INDX_SORT_UP];
}

int
wxCALLBACK LogRevListColumnCompareFunction(long item1, long item2, long sortData)
{
  if (item1 < 0 && item2 < 0)
    return 0;

  LogRevList::ColSortInfo* pColSortInfo = (LogRevList::ColSortInfo*)sortData;
  if (pColSortInfo->Parent == NULL)
    return 0;

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
    if (pColSortInfo->Column == 2)
    {
      wxDateTime dateTime1;
      if (dateTime1.ParseFormat(itemText1, wxT("%x %X")) != NULL)
      {
        wxDateTime dateTime2;
        if (dateTime2.ParseFormat(itemText2, wxT("%x %X")) != NULL)
        {
          if (dateTime1.IsEarlierThan(dateTime2))
            result = -1;
          else if (dateTime1.IsLaterThan(dateTime2))
            result = 1;
          else
            result = 0;
        }
      }
    }
    else
    {
      result = itemText1.CmpNoCase(itemText2);
    }
  }

  if (!pColSortInfo->Ascending)
    result = result * -1;

  return result;
}

