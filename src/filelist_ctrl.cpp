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

// wxwindows
#include "wx/wx.h"
#include "wx/confbase.h"
#include "wx/filename.h"
#include "wx/imaglist.h"
/*
#include "wx/gdicmn.h"
#include "wx/window.h"
*/

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/targets.hpp"

// app
//REMOVE #include "include.hpp"
#include "ids.hpp"
//REMOVE #include "rapidsvn_app.hpp"
#include "filelist_ctrl.hpp"
#include "utils.hpp"

// Bitmaps
#include "res/bitmaps/update.xpm"
#include "res/bitmaps/commit.xpm"
#include "res/bitmaps/revert.xpm"
#include "res/bitmaps/resolve.xpm"
#include "res/bitmaps/info.xpm"
#include "res/bitmaps/log.xpm"
#include "res/bitmaps/nonsvn_file.xpm"
#include "res/bitmaps/normal_file.xpm"
#include "res/bitmaps/added_file.xpm"
#include "res/bitmaps/absent_file.xpm"
#include "res/bitmaps/deleted_file.xpm"
#include "res/bitmaps/replaced_file.xpm"
#include "res/bitmaps/modified_file.xpm"
#include "res/bitmaps/merged_file.xpm"
#include "res/bitmaps/conflicted_file.xpm"
#include "res/bitmaps/folder.xpm"
#include "res/bitmaps/versioned_folder.xpm"
#include "res/bitmaps/sort_down.xpm"
#include "res/bitmaps/sort_up.xpm"

/**
 * Number of items in the IMAGE_INDEX table.
 * This should be large enough to include the range of status codes.
 */
#define N_STATUS_KIND  21

/**
 * The index from where there will be only images not related 
 * to the status.
 */
#define N_START_EXTRA_IMGS 15

#define IMG_INDX_FOLDER   N_START_EXTRA_IMGS
#define IMG_INDX_VERSIONED_FOLDER (N_START_EXTRA_IMGS + 1)
#define IMG_INDX_SORT_DOWN (N_START_EXTRA_IMGS + 2)
#define IMG_INDX_SORT_UP (N_START_EXTRA_IMGS + 3)

/**
 * This table holds information about image index in a image list.
 * It will be accessed using a status code as an index.
 * For every index that equals a valid status code, there should be
 * a valid value which represents an index in the image list.
 */
int IMAGE_INDEX[N_STATUS_KIND];

/**
 * Tags for wxConfig file settings, defined here to avoid duplicate
 * hard coded strings.
 */
static const char *pSortColumnTag = "/FileListCtrl/SortColumn";
static const char *pSortOrderTag = "/FileListCtrl/SortOrder";
static const char *pSortColumn0Width = "/FileListCtrl/Column0Width";
static const char *pSortColumn1Width = "/FileListCtrl/Column1Width";
static const char *pSortColumn2Width = "/FileListCtrl/Column2Width";
static const char *pSortColumn3Width = "/FileListCtrl/Column3Width";
static const char *pSortColumn4Width = "/FileListCtrl/Column4Width";

/**
 * A safe wrapper for getting images - avoids array bounds
 * exceptions.
 */
static int
GetImageIndex (int Index)
{
  if ((Index >= 0) && (Index <= N_STATUS_KIND))
    return IMAGE_INDEX[Index];
  else
    return 0;
}

BEGIN_EVENT_TABLE (FileListCtrl, wxListCtrl)
  EVT_KEY_DOWN (FileListCtrl::OnKeyDown)
  EVT_LIST_ITEM_ACTIVATED (-1, FileListCtrl::OnItemActivated)
  EVT_LIST_ITEM_RIGHT_CLICK (LIST_CTRL, FileListCtrl::OnItemRightClk)
  EVT_LIST_COL_CLICK (LIST_CTRL, FileListCtrl::OnColumnLeftClick)
END_EVENT_TABLE ()

FileListCtrl::FileListCtrl (wxWindow * parent, const wxWindowID id, 
                            const wxPoint & pos, const wxSize & size)
  : wxListCtrl (parent, id, pos, size, wxLC_REPORT),
    m_pool(NULL)
{
  m_imageListSmall = new wxImageList (16, 16, TRUE);

  // add images to image list
  m_imageListSmall->Add (wxIcon (nonsvn_file_xpm));
  m_imageListSmall->Add (wxIcon (normal_file_xpm));
  m_imageListSmall->Add (wxIcon (added_file_xpm));
  m_imageListSmall->Add (wxIcon (absent_file_xpm));
  m_imageListSmall->Add (wxIcon (deleted_file_xpm));
  m_imageListSmall->Add (wxIcon (replaced_file_xpm));
  m_imageListSmall->Add (wxIcon (modified_file_xpm));
  m_imageListSmall->Add (wxIcon (merged_file_xpm));
  m_imageListSmall->Add (wxIcon (conflicted_file_xpm));

  m_imageListSmall->Add (wxIcon (folder_xpm));
  m_imageListSmall->Add (wxIcon (versioned_folder_xpm));

  m_imageListSmall->Add (wxIcon (sort_down_xpm));
  m_imageListSmall->Add (wxIcon (sort_up_xpm));

  // set the indexes
  IMAGE_INDEX[svn_wc_status_none] = 0;
  IMAGE_INDEX[svn_wc_status_unversioned] = 0;
  IMAGE_INDEX[svn_wc_status_normal] = 1;
  IMAGE_INDEX[svn_wc_status_added] = 2;
  IMAGE_INDEX[svn_wc_status_absent] = 3;
  IMAGE_INDEX[svn_wc_status_deleted] = 4;
  IMAGE_INDEX[svn_wc_status_replaced] = 5;
  IMAGE_INDEX[svn_wc_status_modified] = 6;
  IMAGE_INDEX[svn_wc_status_merged] = 7;
  IMAGE_INDEX[svn_wc_status_conflicted] = 8;

  IMAGE_INDEX[IMG_INDX_FOLDER] = 9;
  IMAGE_INDEX[IMG_INDX_VERSIONED_FOLDER] = 10;

  IMAGE_INDEX[IMG_INDX_SORT_DOWN] = 11;
  IMAGE_INDEX[IMG_INDX_SORT_UP] = 12;

  // set this file list control to use the image list
  SetImageList (m_imageListSmall, wxIMAGE_LIST_SMALL);

  m_path.Empty ();

  // Get settings from config file:
  wxConfigBase *pConfig = wxConfigBase::Get ();
  SortColumn = pConfig->Read (pSortColumnTag, (long) 0);
  SortIncreasing = pConfig->Read (pSortOrderTag, (long) 1) ? true : false;

  wxListItem itemCol;
  itemCol.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_IMAGE;
  itemCol.m_text = wxT ("Name");
  itemCol.m_image = -1;
  InsertColumn (0, itemCol);

  itemCol.m_text = wxT ("Revision");
  InsertColumn (1, itemCol);

  itemCol.m_text = wxT ("Last Changed");
  InsertColumn (2, itemCol);

  itemCol.m_text = wxT ("Status");
  InsertColumn (3, itemCol);

  itemCol.m_text = wxT ("Prop Status");
  InsertColumn (4, itemCol);

  SetColumnImages ();

  // Set the column widths stored in the config:
  SetColumnWidth (0, pConfig->Read (pSortColumn0Width, (long) 150));
  SetColumnWidth (1, pConfig->Read (pSortColumn1Width, (long) 75));
  SetColumnWidth (2, pConfig->Read (pSortColumn2Width, (long) 100));
  SetColumnWidth (3, pConfig->Read (pSortColumn3Width, (long) 75));
  SetColumnWidth (4, pConfig->Read (pSortColumn4Width, (long) 75));
}

FileListCtrl::~FileListCtrl ()
{
  // Write settings to config file:
  wxConfigBase *pConfig = wxConfigBase::Get ();
  pConfig->Write (pSortColumnTag, (long) SortColumn);
  pConfig->Write (pSortOrderTag, (long) (SortIncreasing ? 1 : 0));
  pConfig->Write (pSortColumn0Width, (long) GetColumnWidth (0));
  pConfig->Write (pSortColumn1Width, (long) GetColumnWidth (1));
  pConfig->Write (pSortColumn2Width, (long) GetColumnWidth (2));
  pConfig->Write (pSortColumn3Width, (long) GetColumnWidth (3));
  pConfig->Write (pSortColumn4Width, (long) GetColumnWidth (4));

  delete m_imageListSmall;
}

int wxCALLBACK
FileListCtrl::wxListCompareFunction (long item1, long item2, long sortData)
{
  svn::Status * ps1 = (svn::Status *) item1;
  svn::Status * ps2 = (svn::Status *) item2;
  FileListCtrl *pList = (FileListCtrl *) sortData;

  if (ps1 && ps2)               // Defensive programming.
    return CompareItems (ps1, ps2, pList->SortColumn, pList->SortIncreasing);
  else
    return 0;
}

int wxCALLBACK
FileListCtrl::CompareItems (svn::Status * ps1, svn::Status * ps2,
                            int SortColumn, bool SortIncreasing)
{
  int rc = 0;
  unsigned long r1 = 0, r2 = 0;
  bool ok1, ok2;

  switch (SortColumn)
  {
  case 0:                      // Name
    // Directories always precede files:
    if (ps1->isDir () && !ps2->isDir ())
      rc = -1;
    else if (!ps1->isDir () && ps2->isDir ())
      rc = 1;
    else
    {
      rc = wxString (ps1->path ()).CmpNoCase (ps2->path ());
      if (!SortIncreasing)
        rc = rc * -1;           // Reverse the sort order.
    }
    break;

  case 1:                      // Revision
  case 2:                      // Last change
    ok1 = ok2 = true;

    switch (SortColumn)
    {
    case 1:                    // Revision
      ok1 = ps1->isVersioned ();
      r1 = ps1->revision ();
      ok2 = ps2->isVersioned ();
      r2 = ps2->revision ();
      break;

    case 2:                    // Last change
      ok1 = ps1->isVersioned ();
      r1 = ps1->lastChanged ();
      ok2 = ps2->isVersioned ();
      r2 = ps2->lastChanged ();
      break;
    }

    // Unversioned always come last.
    if (ok1 && !ok2)
      rc = -1;
    else if (ok2 && !ok1)
      rc = 1;
    else if (r1 == r2)
      rc = 0;
    else
    {
      rc = r1 > r2 ? 1 : -1;
      if (!SortIncreasing)
        rc = rc * -1;           // Reverse the sort order.
    }
    break;

  case 3:                      // Status
    rc =
      wxString (ps1->textDescription ()).CmpNoCase (ps2->textDescription ());
    if (!SortIncreasing)
      rc = rc * -1;             // Reverse the sort order.
    break;

  case 4:                      // Prop status
    rc =
      wxString (ps1->propDescription ()).CmpNoCase (ps2->propDescription ());
    if (!SortIncreasing)
      rc = rc * -1;             // Reverse the sort order.
    break;
  }

  // If the items are equal, we revert to a sort on the item name,
  // being careful to avoid recursion.
  if ((SortColumn != 0) && (rc == 0))
    rc = CompareItems (ps1, ps2, 0, SortIncreasing);

  return rc;
}

void
FileListCtrl::UpdateFileList (const wxString & path)
{
  m_path = path;

  UpdateFileList ();
}

void
FileListCtrl::UpdateFileList ()
{
  const wxString & path = m_path;
  // delete all the items in the list to display the new ones
  DeleteAllItems ();

  // cycle through the files and folders in the current path
  wxFileName fullpath (path, wxT ("*"));
  wxString name;

  wxLogStatus (_T ("Listing entries in '%s'"), path.c_str ());

  // Hide the list to speed up inserting
  Hide ();

  std::string stdpath(path.c_str());

  svn::Context context;
  context.setLogin ("", "");

  svn::Client client (&context);
  const std::vector<svn::Status> & statusVector =
    client.status (path.c_str (), FALSE);
  std::vector<svn::Status>::const_iterator it;

  for (it = statusVector.begin (); it != statusVector.end (); it++)
  {
    const svn::Status & status = *it;

    int i = GetItemCount ();

    if( status.path() == stdpath )
    {
      name = ".";
    }
    else
    {
      name = wxFileNameFromPath (status.path ());
    }

    wxString text;
    const char *ctext;

    int imageIndex = 0;

    if (status.isVersioned ())
    {
      if (status.isDir ())
      {
        imageIndex = GetImageIndex (IMG_INDX_VERSIONED_FOLDER);
      }
      else
      {
        imageIndex = GetImageIndex (status.textType ());
      }
    }
    else
    {
      // unversioned entries dont carry dir info
      // with them. must find this out by ourself
      if (wxDirExists (status.path ()))
      {
        imageIndex = GetImageIndex (IMG_INDX_FOLDER);
      }
      else if (wxFileExists (status.path ()))
      {
        imageIndex = GetImageIndex (svn_wc_status_unversioned);
      }
      else
      {
        imageIndex = svn_wc_status_none;
      }
    }

    InsertItem (i, name, imageIndex);

    // The item data will be used to sort the list:
    SetItemData (i, (long)new svn::Status (status));    // The control now owns this data
    // and must delete it in due course.

    text = "";
    if (status.isVersioned ())
    {
      text.Printf ("%ld", status.revision ());
    }
    SetItem (i, 1, text);

    text = "";
    if (status.isVersioned ())
    {
      text.Printf ("%ld", status.lastChanged ());
    }
    SetItem (i, 2, text);

    ctext = status.textDescription ();
    if (!ctext)
    {
      ctext = "";
    }
    SetItem (i, 3, _T (ctext));

    ctext = status.propDescription ();
    if (!ctext)
    {
      ctext = "";
    }
    SetItem (i, 4, _T (ctext));
  }

  SortItems (wxListCompareFunction, (long) this);

  Show ();
}

void
FileListCtrl::OnKeyDown (wxKeyEvent & event)
{
  if (event.GetKeyCode () == WXK_F5)
  {
    UpdateFileList ();
  }
  else
  {
    event.Skip ();
  }
}

void
FileListCtrl::OnItemActivated (wxListEvent & event)
{
  //TODO do we really need this
  //TODO if the answer is yes, then find a way to
  //TODO eliminate the way calls are passed DEEP
  /*
  wxString name = GetItemText (event.GetIndex ());
  wxFileName fullpath (m_path, name);

  if (fullpath.DirExists ())
  {
    // If the path is a directory, change the path in the folder browser.
    // There is no need to call UpdateFileList() as SetPath() will 
    // trigger a EVT_TREE_SEL_CHANGED message in the fiolder browser.

    wxGetApp ().GetAppFrame ()->GetFolderBrowser ()->SetPath (fullpath.
                                                              GetFullPath ());
  }
  */
}

void
FileListCtrl::OnItemRightClk (wxListEvent & event)
{
  int flag;
  wxPoint screenPt = wxGetMousePosition ();
  wxPoint clientPt = ScreenToClient (screenPt);

  long index = HitTest (clientPt, flag);
  if (index >= 0)
  {
    ShowMenu (index, clientPt);
  }
}

void
FileListCtrl::OnColumnLeftClick (wxListEvent & event)
{
  int Column = event.GetColumn ();

  // A second click on the current sort column reverses the order of sorting.
  if (Column == SortColumn)
    SortIncreasing = !SortIncreasing;
  else
  {
    SortColumn = Column;
    SortIncreasing = true;
  }

  SetColumnImages ();
  SortItems (wxListCompareFunction, (long) this);
}

void
FileListCtrl::SetColumnImages ()
{
  // Update the column titles to reflect the sort column.
  for (int i = 0; i < GetColumnCount (); i++)
  {
    wxListItem LI;
    LI.m_mask = wxLIST_MASK_IMAGE;
    if (i == SortColumn)
      LI.m_image = SortIncreasing ?
        IMAGE_INDEX[IMG_INDX_SORT_DOWN] : IMAGE_INDEX[IMG_INDX_SORT_UP];
    else
      LI.m_image = -1;
    SetColumn (i, LI);
  }
}

svn_wc_status_kind FileListCtrl::fileStatus (svn::Status * status)
{
  if (status->textType () != svn_wc_status_none &&
      status->textType () != svn_wc_status_normal)
    return status->textType ();

  return status->propType ();
}

void
FileListCtrl::ShowMenu (long index, wxPoint & pt)
{
  wxFileName filepath (m_path, GetItemText (index));
  wxMenu popup_menu;
  wxString path = filepath.GetFullPath ();

  buildMenu (popup_menu, UnixPath (path));

  PopupMenu (&popup_menu, pt);
}

void
FileListCtrl::buildMenu (wxMenu & menu, const wxString & path)
{
  wxMenuItem *pItem;

  pItem = new wxMenuItem (&menu, ID_Update, _T ("Update"));
  pItem->SetBitmap (wxBITMAP (update));
  menu.Append (pItem);
  pItem = new wxMenuItem (&menu, ID_Commit, _T ("Commit"));
  pItem->SetBitmap (wxBITMAP (commit));
  menu.Append (pItem);
  pItem = new wxMenuItem (&menu, ID_CopyTo, _T ("Copy"));
  menu.Append (pItem);
  pItem = new wxMenuItem (&menu, ID_MoveTo, _T ("Move"));
  menu.Append (pItem);

  menu.AppendSeparator ();

  pItem = new wxMenuItem (&menu, ID_Revert, _T ("Revert"));
  pItem->SetBitmap (wxBITMAP (revert));
  menu.Append (pItem);
  pItem = new wxMenuItem (&menu, ID_Resolve, _T ("Resolve"));
  pItem->SetBitmap (wxBITMAP (resolve));
  menu.Append (pItem);

  menu.AppendSeparator ();

  pItem = new wxMenuItem (&menu, ID_RenameHere, _T ("Rename"));
  menu.Append (pItem);
  pItem = new wxMenuItem (&menu, ID_Info, _T ("Info"));
  pItem->SetBitmap (wxBITMAP (info));
  menu.Append (pItem);
  pItem = new wxMenuItem (&menu, ID_Log, _T ("Log"));
  pItem->SetBitmap (wxBITMAP (log));
  menu.Append (pItem);
}

const IndexArray 
FileListCtrl::GetSelectedItems () const
{
  int sel = GetSelectedItemCount ();
  IndexArray indx_arr;

  if (sel <= 0)
    indx_arr.Clear ();
  else
  {
    indx_arr.Alloc (sel);

    long item = -1;
    for (;;)
    {
      item = GetNextItem (item, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
      if (item == -1)
        break;

      indx_arr.Add (item);
    }
  }

  sel = indx_arr.GetCount ();

  return indx_arr;
}

const svn::Targets 
FileListCtrl::GetTargets () const
{
  IndexArray arr = GetSelectedItems ();
  size_t i;


  std::vector<svn::Path> v;
  v.reserve (arr.GetCount ());

  for (i = 0; i < arr.GetCount (); i++)
  {
    wxFileName fname (m_path, GetItemText (arr[i]));
    wxString path = fname.GetFullPath ();

    v.push_back (path.c_str ());
  }

  return svn::Targets (v);
}

void
FileListCtrl::GetFullUnixPath (long index, wxString & fullpath)
{
  fullpath.Empty ();

  if (index < 0)
    return;

  wxFileName fname (m_path, GetItemText (index));
  fullpath = fname.GetFullPath ();
  UnixPath (fullpath);
}

void
FileListCtrl::DeleteAllItems ()
{
  // Delete the item data before deleting the items:
  for (int i = 0; i < GetItemCount (); i++)
  {
    svn::Status * p = (svn::Status *) GetItemData (i);
    if (p)
    {
      delete p;
    }
  }
  wxListCtrl::DeleteAllItems ();
}

void
FileListCtrl::DeleteItem (long item)
{
  svn::Status * p = (svn::Status *) GetItemData (item);
  if (p)
  {
    delete p;
  }
  wxListCtrl::DeleteItem (item);
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
