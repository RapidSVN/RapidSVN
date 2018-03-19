/*
 * ====================================================================
 * Copyright (c) 2002-2018 The RapidSVN Group.  All rights reserved.
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
#ifndef _FOLDER_BROWSER_H_INCLUDED_
#define _FOLDER_BROWSER_H_INCLUDED_

// wxWidgets
#include "wx/treectrl.h"

// forward declarations
class wxImageList;
class FolderItemData;
class wxConfigBase;

namespace svn
{
  class Context;
  class ContextListener;
  class StatusSel;
};

class FolderBrowser : public wxTreeCtrl
{
public:
  FolderBrowser(wxWindow* parent, wxWindowID id,
                const wxPoint& pos = wxDefaultPosition,
                const wxSize& size = wxDefaultSize,
                long style = wxTR_HAS_BUTTONS | wxTR_SINGLE,
                const wxValidator& validator = wxDefaultValidator, const
                wxString& name = wxT("FolderBrowser"));

  virtual ~FolderBrowser();

  void
  RefreshFolderBrowser();

  /**
   * remove the selected bookmark from bookmarks
   */
  bool
  RemoveBookmark();

  /**
   * add bookmark.
   *
   * @param path path/url of the bookmark
   */
  void
  AddBookmark(const wxString & path);

  /**
   * returns the path of the current selection
   * if nothing is selected, an empty string
   * is returned
   */
  const wxString
  GetPath() const;

  /**
   * returns the "context" of the current selection.
   * returns NULL if nothing is selected
   *
   * @return context of selection
   */
  const FolderItemData *
  GetSelectedItemData() const;

  /**
   * the selected @ref Status instances
   */
  const svn::StatusSel &
  GetStatusSel() const;

  /**
   * returns the "context" of the item at point.
   * returns NULL if no item is at that location
   *
   * @return context of selection
   */
  const FolderItemData *
  HitTest(const wxPoint & point) const;

  /**
   * return the authentication context of the current selection
   *
   * @return authentication context
   * @retval NULL if Bookmarks is selected
   */
  svn::Context *
  GetContext();

  /**
   * Tries to select @a path in the current selected bookmark.
   * If it cannot be found in there (because it is deeper in
   * the folder hierarchy) then we try to open folder nodes
   * until we find the path. If the path as whole is not found
   * (folder removed or deleted) we open up the directory structure
   * as close as possible.
   *
   * If @a path is an empty string, select the root
   *
   * @param path
   * @retval true selection successful
   */
  bool
  SelectFolder(const wxString & path);

  /**
   * selects the @a bookmark
   *
   * @param bookmarkPath bookmark path
   * @return true on success
   */
  bool
  SelectBookmark(const wxString & bookmarkPath);

  /**
   * Find a bookmark which contains the given filesystem file or path.
   * @param path directory or file path
   * @return the bookmark path, if found. Empty string if none found.
   */
  wxString
  FindContainingBookmark(const wxString &path);

  /**
   * setting whether there will be one login for all the
   * bookmakrs
   *   or
   * each bookmark in the bookmarks list remembers its own login
   */
  void
  SetAuthPerBookmark(const bool value);

  /**
   * check whether flat-mode is turned on for the
   * selected bookmark
   *
   * @retval true flat-mode is turned on
   */
  bool
  IsFlat() const;

  /**
   * turns flat-mode for the selected bookmark
   * on or off
   *
   * @param flatMode true=on
   * @retval true flat-mode successfully set
   * @retval false not set, e.g. root selected or nothing
   */
  bool
  SetFlat(bool flatMode);

  /**
   * checks whether indication of modified content is
   * switched on (for the selected bookmark)
   *
   * @retval true modified content is indicarted
   */
  bool
  GetIndicateModifiedChildren() const;

  /**
   * switch on/off indication of modified children for
   * the selected bookmark
   *
   * @param show
   * @retval true success
   * @retval false not set, no bookmark selected
   */
  bool
  SetIndicateModifiedChildren(bool show);

  /**
   * @return auth per bookmark setting
   */
  bool
  GetAuthPerBookmark() const;

  /**
   * check if authentication credentials should be cached or not
   */
  bool
  GetAuthCache() const;

  /**
   * select if authentication credentials should be cached or not
   */
  void
  SetAuthCache(const bool value);

  void
  SetListener(svn::ContextListener * listener);

  svn::ContextListener *
  GetListener() const;

  /**
   * Writes configuration including bookmarks
   * to @a cfg
   *
   * @param cfg Config instance to write to
   */
  void
  WriteConfig(wxConfigBase * cfg) const;

  /**
   * Read configuration
   *
   * @param cfg Config instance
   */
  void
  ReadConfig(wxConfigBase * cfg);

  /**
   * Expand the selected leaf of the tree.
   * Has no effect if the selection has no
   * leafs
   */
  void
  ExpandSelection();

private:
  struct Data;
  Data * m;

  /**
   * disallow default constructor
   */
  FolderBrowser();

  /**
   * disallow copy constructor
   */
  FolderBrowser(const FolderBrowser &);

  /**
   * disallow assignment operator
   */
  FolderBrowser & operator = (const FolderBrowser &);

private:
  DECLARE_EVENT_TABLE()

  void OnTreeKeyDown(wxTreeEvent & event);
  void OnExpandItem(wxTreeEvent & event);
  void OnCollapseItem(wxTreeEvent & event);
  void OnContextMenu(wxContextMenuEvent & event);
  void OnBeginDrag(wxTreeEvent & event);
  void OnSetFocus(wxFocusEvent & event);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
