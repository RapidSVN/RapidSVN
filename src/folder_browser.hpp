/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
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

// wxWidgets
#include "wx/treectrl.h"

// forward declarations
class wxImageList;
class FolderItemData;
namespace svn
{
  class Context;
  class ContextListener;
};

class FolderBrowser : public wxControl
{
public:
  FolderBrowser (wxWindow * parent,  const wxWindowID id = -1,
                 const wxPoint & pos = wxDefaultPosition,
                 const wxSize & size = wxDefaultSize,
                 const wxString & name = wxT("FolderBrowser"));

  virtual ~ FolderBrowser ();

  virtual void Refresh ();

  /**
   * remove the selected bookmark from bookmarks
   */
  const bool RemoveBookmark ();

  /**
   * add bookmark
   */
  void AddBookmark (const wxString & path);

  /**
   * @return bookmark count on bookmarks
   */
  const size_t
  GetBookmarkCount () const;

  /**
   * get the path of the bookmark at @a index
   *
   * @param index zero-based index
   * @return path of the bookmark
   */
  const wxString &
  GetBookmark (const size_t index) const;

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
   * @retval NULL if Bookmarks is selected
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
  SelectFolder (const wxString & path);

  /**
   * selects the @a bookmark
   *
   * @param bookmarkPath bookmark path
   * @return true on success
   */
  bool
  SelectBookmark (const wxString & bookmarkPath);

  /**
   * setting whether there will be one login for all the
   * bookmakrs
   *   or
   * each bookmark in the bookmarks list remembers its own login
   */
  void
  SetAuthPerBookmark (const bool value);

  /**
   * @return auth per bookmark setting
   */
  const bool
  GetAuthPerBookmark () const;


  void
  SetListener (svn::ContextListener * listener);


  svn::ContextListener *
  GetListener () const;

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

  void OnContextMenu (wxContextMenuEvent & event);

};


#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
