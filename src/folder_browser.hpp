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
#ifndef _FOLDER_BROWSER_H_INCLUDED_
#define _FOLDER_BROWSER_H_INCLUDED_

#include "unique_array_string.hpp"
#include "wx/treectrl.h"

class wxImageList;
class FolderItemData;

class FolderBrowser:public wxControl
{
public:
  FolderBrowser (wxWindow * parent,  const wxWindowID id = -1,
                 const wxPoint & pos = wxDefaultPosition,
                 const wxSize & size = wxDefaultSize,
                 const wxString & name = "FolderBrowser");

  virtual ~ FolderBrowser ();

  virtual void Refresh ();

  UniqueArrayString & GetWorkbenchItems ();

  const bool RemoveProject ();
  void AddProject (const wxString & projectPath);

  /**
   * returns the path of the current selection
   * if nothing is selected, an empty string
   * is returned
   */
  const wxString GetPath () const;

  /**
   * returns the "context" of the current selection.
   * returns NULL if nothing is selected
   *
   * @return context of selection
   */
  const FolderItemData * GetSelection () const;
  

private:
  wxTreeCtrl* m_treeCtrl;
  wxTreeItemId m_rootId;
  wxImageList* m_imageList;
  UniqueArrayString m_workbenchItems;
  
  static const unsigned int MAXLENGTH_PROJECT;

  void OnExpandItem (wxTreeEvent & event);
  void OnCollapseItem (wxTreeEvent & event);
  void OnSize (wxSizeEvent & event);
  void OnItemRightClk (wxTreeEvent & event);

  void ShowMenu (long index, wxPoint & pt);
  bool HasSubdirectories (const wxString & path);
  void Delete (const wxTreeItemId & id);
  void DeleteAllItems ();

  DECLARE_EVENT_TABLE ()

};


#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
