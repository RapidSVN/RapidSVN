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

class FolderBrowser : public wxControl
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

  /**
   * selects @a path in the current folder.
   * If the tree leaf is closed it will be opened.
   *
   * @param path path in the current folder
   * @retval true on success
   * @retval false if entry was not found
   */
  bool
  SelectFolder(const char * path);

private:
  struct Data;
  Data * m;

  /**
   * disallow default constructor 
   */
  FolderBrowser ();

  /**
   * disallow copy constructor
   */
  FolderBrowser (const FolderBrowser &);

  /**
   * disallow assignment operator
   */
  FolderBrowser & operator = (const FolderBrowser &);

private:
  DECLARE_EVENT_TABLE ()

  void OnSize (wxSizeEvent & WXUNUSED (event));

  void OnExpandItem (wxTreeEvent & event);

  void OnCollapseItem (wxTreeEvent & event);

  void OnItemRightClk (wxTreeEvent & event);

};


#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
