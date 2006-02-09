/*
 * ====================================================================
 * Copyright (c) 2002-2006 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _FILE_LIST_CONROL_H_INCLUDED_
#define _FILE_LIST_CONROL_H_INCLUDED_

// wxWidgets
#include "wx/listctrl.h"
#include "wx/dynarray.h"

// svncpp
#include "svncpp/status.hpp"
#include "svncpp/pool.hpp"

/**
 * Define an array of indexes of the selected items.
 */
#ifdef WX_DEFINE_ARRAY_LONG
WX_DEFINE_ARRAY_LONG (long, IndexArray);
#else
WX_DEFINE_ARRAY (long, IndexArray);
#endif

// forward declarations
namespace svn
{
  class Targets;
  class Context;
}

class FileListCtrl:public wxListView
{
public:
  /**
   * Columns in the filelist
   *
   * Note: whenever you make changes in the order
   *       of the columns, add new columns or
   *       delete columns make sure to apply
   *       these changes to the constants
   *       filelist_ctrl.cpp: COLUMN_CAPTIONS, COLUMN_NAMES
   *       rapidsvn_frame.cpp: COLUMN_ID_MAP, COLUMN_CAPTIONS
   */
  enum
  {
    COL_NAME = 0,
    COL_PATH,
    COL_REV,
    COL_CMT_REV,
    COL_AUTHOR,
    COL_TEXT_STATUS,
    COL_PROP_STATUS,
    COL_CMT_DATE,
    COL_EXTENSION,
    COL_TEXT_TIME,
    COL_PROP_TIME,
    COL_LOCK_OWNER,
    COL_LOCK_COMMENT,
    COL_CHECKSUM,
    COL_URL,
    COL_REPOS,
    COL_UUID,
    COL_SCHEDULE,
    COL_COPIED,
    COL_CONFLICT_OLD,
    COL_CONFLICT_NEW,
    COL_CONFLICT_WRK,
    COL_COUNT
  } FileListColumns;


  FileListCtrl (wxWindow * parent, const wxWindowID id,
                const wxPoint & pos, const wxSize & size);

  virtual ~FileListCtrl ();

  void ResetColumns ();
  void DeleteAllItems();

  /**
   * set visibility of column
   *
   * @param col column number
   * @param visible
   */
  void SetColumnVisible (const int col, const bool visible);

  /**
   * @param col column number
   * @retval true column visible
   * @retval false invisible
   */
  const bool GetColumnVisible (const int col);

  /**
   * @param col column number
   * @return returns name of the column
   */
  static wxString GetColumnCaption (const int col);

  /**
   * set column width
   *
   * @param col column number
   * @param width
   */
  void SetColumnWidth (const int col, const int width);

  /**
   * change to either flat or single directory view
   * in flat view all the subdirectories are listed
   * as well.
   *
   * @param flat
   */
  void SetFlat (const bool flat);

  /**
   * shall we contact the repository when updating,
   * so we get information about out-dated-ness?
   */
  void
  SetWithUpdate (bool value);

  bool
  GetWithUpdate () const;

  /**
   * shall we display unversioned file entries?
   */
  void
  SetShowUnversioned (bool value);

  bool
  GetShowUnversioned () const;

  void
  SetIgnoreExternals (bool value);

  bool
  GetIgnoreExternals () const;

  /**
   * get the "flat" setting
   *
   * @return true=flat
   */
  bool IsFlat ();

  void UpdateFileList ();
  void UpdateFileList (const wxString & path);
  const IndexArray GetSelectedItems () const;
  const svn::Targets GetTargets () const;

  /**
   * determine the properties of what has been selected
   * in terms of the relevant Action Flags.
   */
  unsigned int
  GetSelectionActionFlags () const;

  void
  SetContext (svn::Context * Context);


  svn::Context *
  GetContext () const;

private:
  struct Data;
  /** private data for this class */
  Data * m;

  // utility methods
  void ShowMenu (wxPoint & pt);
  void DeleteItem (long item);

  void SetColumnImages ();
  void UpdateColumns ();

  // message handlers
  void OnKeyDown (wxKeyEvent & event);
  void OnItemRightClk (wxListEvent & event);
  void OnDoubleClick (wxMouseEvent & event);
  void OnColumnLeftClick (wxListEvent & event);
  void OnColumnEndDrag (wxListEvent & event);
  void OnContextMenu (wxContextMenuEvent & event);

private:
  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
