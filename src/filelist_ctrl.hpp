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
#ifndef _FILE_LIST_CONROL_H_INCLUDED_
#define _FILE_LIST_CONROL_H_INCLUDED_

#include "wx/listctrl.h"
#include "wx/dynarray.h"
#include "svncpp/status.hpp"
#include "svncpp/pool.hpp"

/**
 * Define an array of indexes of the selected items.
 */
#ifdef WX_DEFINE_ARRAY_INT
WX_DEFINE_ARRAY_INT (long, IndexArray);
#else
WX_DEFINE_ARRAY (long, IndexArray);
#endif

// forward declarations
namespace svn
{
  class Targets;
}

class FileListCtrl:public wxListCtrl
{
private:
  svn::Pool  m_pool;
  wxImageList *m_imageListSmall;
  wxString m_path;

  int SortColumn;
  bool SortIncreasing;

  static int wxCALLBACK FileListCtrl::wxListCompareFunction(
    long item1, long item2, long sortData);
  static int wxCALLBACK FileListCtrl::CompareItems(
    svn::Status* ps1, svn::Status* ps2, int SortColumn, bool SortIncreasing);
  void SetColumnImages();

  /**
   * Returns the overall file status. 
   */
  svn_wc_status_kind fileStatus (svn::Status * status);


public:
   FileListCtrl (wxWindow * parent, const wxWindowID id,
                 const wxPoint & pos, const wxSize & size);

  ~FileListCtrl ();

  // utility methods
  void UpdateFileList ();
  void UpdateFileList (const wxString & path);
  const IndexArray GetSelectedItems () const;
  const svn::Targets GetTargets () const;
  void GetFullUnixPath (long index, wxString & fullpath);

  void ShowMenu (long index, wxPoint & pt);
  void buildMenu (wxMenu & menu, const wxString & path);
  void DeleteAllItems();
  void DeleteItem( long item );

  // message handlers
  void OnItemActivated (wxListEvent & event);
  void OnKeyDown (wxKeyEvent & event);
  void OnItemRightClk (wxListEvent & event);
  void OnColumnLeftClick (wxListEvent & event);

private:
 DECLARE_EVENT_TABLE ()};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
