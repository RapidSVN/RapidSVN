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
#ifndef _FOLDER_ITEM_DATA_H_
#define _FOLDER_ITEN_DATA_H_

#if defined(__GNUG__) && !defined(__APPLE__)
#pragma interface "folder_item_data.hpp"
#endif

#include "wx/wx.h"
#include "wx/treectrl.h"

enum
{
  FOLDER_TYPE_INVALID,
  FOLDER_TYPE_WORKBENCH,
  FOLDER_TYPE_PROJECT,
  FOLDER_TYPE_NORMAL
};

class FolderItemData : public wxTreeItemData
{
public: 
  FolderItemData(const int folderType,  
                 const wxString & path = wxEmptyString, 
                 const wxString & name = wxEmptyString,
                 const bool hasChildren = FALSE);

  FolderItemData ();

  FolderItemData (const FolderItemData& src);   

  void Create (const int folderType,  
               const wxString & path = wxEmptyString, 
               const wxString & name = wxEmptyString,
               const bool hasChildren = FALSE);
    
  const int getFolderType () const
  {
    return m_folderType;
  }

  const wxString& getPath () const 
  {
    return m_path;
  }

  const wxString& getName () const
  {
    return m_name;
  }

  const bool hasChildren () const
  {
    return m_hasChildren;
  }

  /**
   * returns true for a REAL directory.
   * FOLDER_TYPE_WORKBENCH is NOT real.
   *
   * @retval true existing folder
   * @retval false unreal/virtual entry
   */
  const bool isReal () const
  {
    switch (m_folderType)
    {
    case FOLDER_TYPE_PROJECT:
    case FOLDER_TYPE_NORMAL:
      return true;

    default:
      return false;
    }
  }

private:
  int m_folderType;
  wxString m_path;
  wxString m_name;
  bool m_hasChildren;
};


#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
