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

#include "log_rev_list.hpp"

BEGIN_EVENT_TABLE(LogRevList, wxListView)
  EVT_SIZE(LogRevList::OnSize)
END_EVENT_TABLE()

LogRevList::LogRevList(wxWindow * parent, wxWindowID id, const wxPoint& pos,
           const wxSize& size, long style,
           const wxValidator& validator,
           const wxString& name)
  : wxListView(parent, id, pos, size, style, validator, name)
{
  InsertColumn(0, _("Revision"));
  InsertColumn(1, _("User"));
  InsertColumn(2, _("Date"));
  InsertColumn(3, _("Log Message"));

  SetColumnWidth(0, 65);
  SetColumnWidth(1, 100);
  SetColumnWidth(2, 150);
  SetColumnWidth(3, 200);
}

LogRevList::~LogRevList()
{
  DeleteAllItems();
}

void
LogRevList::SetEntries(const svn::LogEntries * entries)
{
  DeleteAllItems();

  if (entries == 0)
    return;

  AddEntriesToList(entries);
}

void
LogRevList::AddEntriesToList(const svn::LogEntries * entries)
{
  if (entries == 0)
    return;

  long index=0;
  if(GetItemCount() > 0)
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
    SetItem(index, 3, NewLinesToSpaces(Utf8ToLocal(entry.message.c_str())));
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
      if(revnum == revnumTemp)
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
  if(count == 0)
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

