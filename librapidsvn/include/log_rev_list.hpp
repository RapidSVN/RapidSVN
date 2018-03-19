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
#ifndef _LOG_REV_LIST_H_INCLUDED_
#define _LOG_REV_LIST_H_INCLUDED_

#include <map>

// wx
#include "wx/wx.h"
#include "wx/listctrl.h"

// svncpp
#include "svncpp/client.hpp"

// app
#include "utils.hpp"

class LogRevList : public wxListView
{
public:
  static long COL_COUNT;

  struct ColSortInfo {
    wxListView* Parent;
    long Column;
    bool Ascending;

    ColSortInfo(wxListView* parent, long column, bool ascending)
      : Parent(parent), Column(column), Ascending(ascending)
    {
    }
  };

public:
  LogRevList(wxWindow * parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition,
             const wxSize& size = wxDefaultSize, long style = wxLC_REPORT,
             const wxValidator& validator = wxDefaultValidator,
             const wxString& name = wxT("LogRevList"));

  virtual ~LogRevList();

  void SetEntries(svn::LogEntries * entries);
  void
  AddEntriesToList(svn::LogEntries * entries);

  /**
   * Returns the revision for the given @a item
   *
   * @param item
   * @return revnum
   * @retval -1 not found/error
   */
  svn_revnum_t
  GetRevisionForItem(long item) const;

  /**
   * returns the selected revision.
   *
   * @return selected revision
   * @retval -1 if nothing was selected or the cell
   *            contains an invalid string
   */
  svn_revnum_t
  GetSelectedRevision() const;

  /**
   * returns the prior revision.
   * Like @a GetSelectedRevision, but returns revision prior to the one passed
   *
   * @return if none found then return -1
   */
  svn_revnum_t
  GetPriorRevision(const svn_revnum_t revnum) const;

  /**
   * returns the selected revisions.
   * Like @a GetSelectedRevision, but can return
   * more revisions at once.
   *
   * @return if nothing is selected, an empty array
   *         will be returned
   */
  RevnumArray
  GetSelectedRevisions() const;

  /**
   * returns the last revision in list.
   *
   * @return last revision
   * @retval -1 if nothing was selected or the cell
   *            contains an invalid string
   */
  svn_revnum_t GetLastRevision() const;

  void
  DeleteAllItems();

  void
  AutoSizeLastColumn();

private:
  wxString
  NewLinesToSpaces(const wxString& str);
  void
  OnSize(wxSizeEvent& event);
  void
  OnColClick(wxListEvent& event);
  void
  SetColumnImages();
  inline int
  GetSortImageIndex(bool sortAscending);

private:
  ColSortInfo m_ColSortInfo;
  wxImageList * m_ImageListSmall;
  std::map<int,int> m_ImageIndexArray;

  DECLARE_EVENT_TABLE()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
