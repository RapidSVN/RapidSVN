/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
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

#include "unique_array_string.h"
#include "wx/treectrl.h"

static const wxString FolderBrowserNameStr;

class UniqueArrayString;
class wxImageList;

class FolderBrowser:public wxControl
{
public:
  FolderBrowser (wxWindow * parent,
                 apr_pool_t * pool,
                 const wxWindowID id = -1,
                 const wxPoint & pos = wxDefaultPosition,
                 const wxSize & size = wxDefaultSize,
                 const wxString & name = FolderBrowserNameStr);

  virtual ~ FolderBrowser ();

  virtual void Refresh ();

  UniqueArrayString & GetWorkbenchItems ();

  const bool RemoveProject ();
  void AddProject (const wxString & projectPath);
  wxString GetPath ();
  void SetPath(const wxString& path);

private:
  apr_pool_t * m_pool;
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
  void buildMenu (wxMenu & menu, const wxString & path);
  bool hasSubdirectories (const wxString & path);
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
