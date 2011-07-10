/*
 * ====================================================================
 * Copyright (c) 2002-2011 The RapidSVN Group.  All rights reserved.
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

// svncpp
#include "svncpp/context.hpp"

// app
#include "folder_item_data.hpp"

FolderItemData::FolderItemData(const int folderType,
                               const wxString & path,
                               const wxString & name,
                               bool hasChildren)
    : wxTreeItemData()
{
  Create(folderType, path, name, hasChildren);
}

FolderItemData::FolderItemData()
    : wxTreeItemData()
{
}

FolderItemData::FolderItemData(const FolderItemData& src)
    : wxTreeItemData()
{
  Create(src.m_folderType, src.m_path, src.m_name, src.m_hasChildren);
}

FolderItemData::~FolderItemData()
{
}

void
FolderItemData::Create(const int folderType,
                       const wxString & path,
                       const wxString & name,
                       bool hasChildren)
{
  m_folderType = folderType;
  m_path = path;
  m_name = name;
  m_hasChildren = hasChildren;
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
