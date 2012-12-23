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
#ifndef _LOG_AFF_LIST_H_INCLUDED_
#define _LOG_AFF_LIST_H_INCLUDED_

// stl
#include <list>

// wx
#include "wx/wx.h"
#include "wx/listctrl.h"

// svncpp
#include "svncpp/log_entry.hpp"

// app
#include "utils.hpp"


class LogAffectedList : public wxListView
{
public:
  LogAffectedList(wxWindow * parent, wxWindowID id, const wxPoint& pos = wxDefaultPosition, 
             const wxSize& size = wxDefaultSize, long style = wxLC_REPORT, 
             const wxValidator& validator = wxDefaultValidator, 
             const wxString& name = wxT("LogAffectedList"))
    : wxListView(parent, id, pos, size, style, validator, name)
  {
    InsertColumn(0, _("Action"));
    InsertColumn(1, _("Path"));
    InsertColumn(2, _("Copied from Path"));
    InsertColumn(3, _("Copied from Rev"));
  }

  virtual ~LogAffectedList()
  {
    DeleteAllItems();
  }

  void
  DeleteAllItems()
  {
    // Delete the item data before deleting the items:
    while (GetItemCount() > 0)
      DeleteItem(0);

    wxListCtrl::DeleteAllItems();
  }

  void SetValue(const std::list<svn::LogChangePathEntry> & changedPaths)
  {
    Freeze();

    try
    {
      DeleteAllItems();
      int i=0;
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
        SetItem(i, 3, copyFromRev);

        i++;
      }

      // now refresh the column width
      i=GetColumnCount();
      while (i-- > 0)
        SetColumnWidth(i, (i == 0) ? wxLIST_AUTOSIZE_USEHEADER : wxLIST_AUTOSIZE);

      Thaw();
    }
    catch (...)
    {
      Thaw();

      throw;
    }
  }
};


#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
