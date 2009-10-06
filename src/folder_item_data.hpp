/*
 * ====================================================================
 * Copyright (c) 2002-2009 The RapidSvn Group.  All rights reserved.
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
#ifndef _FOLDER_ITEM_DATA_H_
#define _FOLDER_ITEM_DATA_H_

// wxWidgets
#include "wx/wx.h"
#include "wx/treectrl.h"

// svncpp
#include "svncpp/status.hpp"

enum
{
  FOLDER_TYPE_INVALID,
  FOLDER_TYPE_BOOKMARKS,
  FOLDER_TYPE_BOOKMARK,
  FOLDER_TYPE_NORMAL
};

/**
 * this information is stored with every entry in the
 * class FolderBrowser
 */
class FolderItemData : public wxTreeItemData
{
public:
  /**
   * constructor
   */
  FolderItemData(const int folderType,
                 const wxString & path = wxEmptyString,
                 const wxString & name = wxEmptyString,
                 bool hasChildren = false);

  /**
   * default constructor
   */
  FolderItemData();

  /**
   * copy constructor
   */
  FolderItemData(const FolderItemData& src);

  /**
   * destructor
   */
  virtual ~FolderItemData();

  /**
   * initialize the member variables
   */
  void
  Create(const int folderType,
         const wxString & path = wxEmptyString,
         const wxString & name = wxEmptyString,
         bool hasChildren = false);

  int
  getFolderType() const
  {
    return m_folderType;
  }

  const wxString&
  getPath() const
  {
    return m_path;
  }

  const wxString&
  getName() const
  {
    return m_name;
  }

  bool
  hasChildren() const
  {
    return m_hasChildren;
  }

  /**
   * returns true for a REAL directory.
   * FOLDER_TYPE_BOOKMARKS is NOT real.
   *
   * @retval true existing folder
   * @retval false unreal/virtual entry
   */
  bool isReal() const
  {
    switch (m_folderType)
    {
    case FOLDER_TYPE_BOOKMARK:
    case FOLDER_TYPE_NORMAL:
      return true;

    default:
      return false;
    }
  }

  const svn::Status &
  getStatus() const
  {
    return m_status;
  }

  void
  setStatus(const svn::Status & status)
  {
    m_status = status;
  }

private:
  int m_folderType;
  wxString m_path;
  wxString m_name;
  bool m_hasChildren;
  svn::Status m_status;

  /**
   * disallow assignment operator
   */
  const FolderItemData &
  operator = (const FolderItemData & src);
};


#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
