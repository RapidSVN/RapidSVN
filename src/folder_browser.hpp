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

// wxwindows
#include "wx/treectrl.h"

// forward declarations
class wxImageList;
class FolderItemData;
namespace svn
{
  class Context;
};

class FolderBrowser : public wxControl
{
public:
  FolderBrowser (wxWindow * parent,  const wxWindowID id = -1,
                 const wxPoint & pos = wxDefaultPosition,
                 const wxSize & size = wxDefaultSize,
                 const wxString & name = "FolderBrowser");

  virtual ~ FolderBrowser ();

  virtual void Refresh ();

  //UniqueArrayString & GetWorkbenchItems ();

  /**
   * remove the selected project from the project
   */
  const bool RemoveProject ();

  /**
   * add project 
   */
  void AddProject (const char * projectPath);

  /**
   * @return project count on workbench
   */
  const size_t 
  GetProjectCount () const;

  /**
   * get the path of the project at @a index
   *
   * @param index zero-based index
   * @return path of the project
   */
  const char *
  GetProject (const size_t index) const;

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
   * return the authentication context of the current selection
   *
   * @return authentication context
   * @retval NULL if Workbench is selected
   */
  svn::Context * GetContext ();

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

  /**
   * setting whether there will be one login for all the
   * projects in the workbench 
   *   or
   * each project in the workbench remembers its own login
   */
  void
  SetAuthPerProject (const bool value);

  /**
   * @return auth per project setting
   */
  const bool 
  GetAuthPerProject () const;

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
