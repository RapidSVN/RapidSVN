/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifdef __GNUG__
#pragma implementation "folder_item_data.hpp"
#endif

#include "folder_item_data.hpp"

FolderItemData::FolderItemData (const int folderType, const wxString & path, 
                                const wxString & name, const bool hasChildren)
    : wxTreeItemData ()
{
    Create (folderType, path, name, hasChildren);
}

FolderItemData::FolderItemData ()
    : wxTreeItemData ()
{
}

FolderItemData::FolderItemData (const FolderItemData& src)
    : wxTreeItemData ()
{
    Create (src.m_folderType, src.m_path, src.m_name, src.m_hasChildren);
}

void
FolderItemData::Create (const int folderType, const wxString & path, 
                        const wxString & name, const bool hasChildren)
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
