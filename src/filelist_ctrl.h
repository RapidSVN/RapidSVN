#ifndef _FILE_LIST_CONROL_H_INCLUDED_
#define _FILE_LIST_CONROL_H_INCLUDED_

#include "wx/listctrl.h"
#include "wx/dynarray.h"
#include "svncpp/status.h"

/**
 * Define an array of indexes of the selected items.
 */
#ifdef WX_DEFINE_ARRAY_INT
WX_DEFINE_ARRAY_INT (long, IndexArray);
#else
WX_DEFINE_ARRAY (long, IndexArray);
#endif

class FileListCtrl:public wxListCtrl
{
private:
  apr_pool_t * pool;

  wxImageList *m_imageListSmall;

  wxString m_path;

      /**
      * An array that holds indexes of the currently
      * selected items in the file list.
      */
  IndexArray indx_arr;
  
  int SortColumn;
  bool SortIncreasing;

  static int wxCALLBACK FileListCtrl::wxListCompareFunction(
    long item1, long item2, long sortData);
  static int wxCALLBACK FileListCtrl::CompareItems(
    svn::Status* ps1, svn::Status* ps2, int SortColumn, bool SortIncreasing);
  void SetColumnImages();

public:
   FileListCtrl (wxWindow * parent,
                 apr_pool_t * __pool,
                 const wxWindowID id,
                 const wxPoint & pos, const wxSize & size);

  ~FileListCtrl ();

  // utility methods
  void UpdateFileList (const wxString & path);
  const IndexArray & GetSelectedItems ();
  apr_array_header_t *GetTargets (apr_pool_t * pool);
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
