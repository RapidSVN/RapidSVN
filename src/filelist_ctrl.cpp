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

// wxwindows
#include "wx/wx.h"
#include "wx/confbase.h"
#include "wx/filename.h"
#include "wx/imaglist.h"

// subversion api
#include "svn_time.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/entry.hpp"
#include "svncpp/targets.hpp"

// app
#include "ids.hpp"
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
static const char ConfigSortColumn[] = "/FileListCtrl/SortColumn";
static const char ConfigSortOrder[] = "/FileListCtrl/SortOrder";
static const char ConfigColumnWidthFmt[] = "/FileListCtrl/Column%dWidth";
static const char ConfigColumnVisibleFmt[] = "/FileListCtrl/Column%dVisible";

/**
 * test if the given status entry is a file or
 * directory. if the status entry is unversioned we are
 * using wx to check what this might be.
 *
 * @todo when browsing a remote repos this wont be
 *       possible
 *
 * @param status status entry
 * @retval false is invalid status 
 */
static bool 
IsDir (const svn::Status * status)
{
  // invalid entry
  
  if (!status)
  {
    return false;
  }

  svn_wc_status_t * svn_status = *status;
  if (!svn_status)
  {
    return false;
  }

  // versioned resource?
  svn_wc_entry_t * entry = svn_status->entry;
  if (entry)
  {
    return entry->kind == svn_node_dir;
  }
  
  // unversioned.
  return wxDirExists (status->path ());
}

static const int
GetDefaultWidth (int col)
{
  int width;
  switch (col)
  {
  case FileListCtrl::COL_CHECKSUM:
    width = 220;
    break;
  case FileListCtrl::COL_NAME:
  case FileListCtrl::COL_CMT_DATE:
  case FileListCtrl::COL_TEXT_TIME:
  case FileListCtrl::COL_PROP_TIME:
    width = 150;
    break;
  case FileListCtrl::COL_AUTHOR:
  case FileListCtrl::COL_TEXT_STATUS:
  case FileListCtrl::COL_PROP_STATUS:
    width = 100;
    break;
  default:
    width = 75;
    break;
  }

  return width;
}

static const char * 
COLUMN_CAPTIONS[FileListCtrl::COL_COUNT] =
{
  _("Name"),
  _("Revision"),
  _("Rep. Rev."),
  _("Author"),
  _("Status"),
  _("Prop Status"),
  _("Last Changed"),
  _("Date"),
  _("Prop Date"),
  _("Checksum"),
  _("Url"),
  _("Repository"),
  _("UUID"),
  _("Schedule"),
  _("Copied"),
  _("Conflict Old"),
  _("Conflict New"),
  _("Conflict Work")
};



/**
 * private struct that hide implementation details
 * to users of @a FileListCtrl
 */
struct FileListCtrl::Data
{
public:
  bool SortIncreasing;
  int SortColumn;
  bool DirtyColumns;
  wxString Path;
  wxImageList *ImageListSmall;
  bool ColumnVisible[COL_COUNT];
  wxString ColumnCaption[COL_COUNT];
  int ColumnIndex[COL_COUNT];
  int ColumnWidth[COL_COUNT];

  /** default constructor */
  Data ()
    : SortIncreasing (true), SortColumn (COL_NAME), 
      DirtyColumns (true)
  {
    ImageListSmall = new wxImageList (16, 16, TRUE);

    // add images to image list
    ImageListSmall->Add (wxIcon (nonsvn_file_xpm));
    ImageListSmall->Add (wxIcon (normal_file_xpm));
    ImageListSmall->Add (wxIcon (added_file_xpm));
    ImageListSmall->Add (wxIcon (absent_file_xpm));
    ImageListSmall->Add (wxIcon (deleted_file_xpm));
    ImageListSmall->Add (wxIcon (replaced_file_xpm));
    ImageListSmall->Add (wxIcon (modified_file_xpm));
    ImageListSmall->Add (wxIcon (merged_file_xpm));
    ImageListSmall->Add (wxIcon (conflicted_file_xpm));

    ImageListSmall->Add (wxIcon (folder_xpm));
    ImageListSmall->Add (wxIcon (versioned_folder_xpm));

    ImageListSmall->Add (wxIcon (sort_down_xpm));
    ImageListSmall->Add (wxIcon (sort_up_xpm));
  }

  /** destructor */
  ~Data ()
  {
    delete ImageListSmall;
  }
    
  /**
   * check the given @a svn::Status entries and the 
   * sort settings.
   *
   * @todo check the sort algorithm, especially the
   *       switch statements...
   */
  static int
  CompareItems (svn::Status * ps1, svn::Status * ps2,
                int SortColumn, bool SortIncreasing)
  {
    int rc = 0;
    unsigned long r1 = 0, r2 = 0;
    bool ok1, ok2;
    svn::Entry e1 (ps1->entry ());
    svn::Entry e2 (ps2->entry ());
    wxString t1, t2;

    switch (SortColumn)
    {
    case COL_NAME:                     
      // Directories always precede files:
      if (IsDir (ps1) && !IsDir (ps2))
        rc = -1;
      else if (!IsDir (ps1) && IsDir (ps2))
        rc = 1;
      else
      {
        rc = wxString (ps1->path ()).CmpNoCase (ps2->path ());
        if (!SortIncreasing)
          rc = rc * -1;           // Reverse the sort order.
      }
      break;

    case COL_REV:     
    case COL_CMT_DATE:
    case COL_CMT_REV:
      ok1 = ok2 = true;

      switch (SortColumn)
      {
      case COL_REV:                  
        ok1 = ps1->isVersioned ();
        r1 = e1.revision ();
        ok2 = ps2->isVersioned ();
        r2 = e2.revision ();
        break;

      case COL_CMT_REV:
        ok1 = ps1->isVersioned ();
        r1 = e1.cmtRev ();
        ok2 = ps2->isVersioned ();
        r2 = e2.cmtRev ();
        break;

      case COL_CMT_DATE:
        ok1 = ps1->isVersioned ();
        r1 = e1.cmtDate ();
        ok2 = ps2->isVersioned ();
        r2 = e2.cmtDate ();
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

    case COL_TEXT_STATUS:
      t1 = _(svn::Status::statusDescription (ps1->textStatus ()));
      t2 = _(svn::Status::statusDescription (ps2->textStatus ()));
      rc = wxString (t1).CmpNoCase (t2);
      if (!SortIncreasing)
        rc = rc * -1;             // Reverse the sort order.
      break;

    case COL_PROP_STATUS:
      t1 = _(svn::Status::statusDescription (ps1->propStatus ()));
      t2 = _(svn::Status::statusDescription (ps2->propStatus ()));
      rc = wxString (t1).CmpNoCase (t2);
      if (!SortIncreasing)
        rc = rc * -1;             // Reverse the sort order.
      break;
    default:
      //TODO implement all the missing columns
      break;
    }

    // If the items are equal, we revert to a sort on the item name,
    // being careful to avoid recursion.
    if ((SortColumn != 0) && (rc == 0))
      rc = CompareItems (ps1, ps2, 0, SortIncreasing);

    return rc;
  }


  /**
   * callback function for @a wxListCtrl::SortColumns
   */
  static int wxCALLBACK
  CompareFunction (long item1, long item2, long sortData)
  {
    svn::Status * ps1 = (svn::Status *) item1;
    svn::Status * ps2 = (svn::Status *) item2;
    Data *data = (Data *) sortData;

    if (ps1 && ps2) 
      return CompareItems (ps1, ps2, data->SortColumn, 
                           data->SortIncreasing);
    else
      return 0;
  }

};  


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

/**
 * utility function to format a date
 */
static wxString
FormatDate (apr_time_t apr_date)
{
  svn::Pool pool;
  wxString str = svn_time_to_cstring (apr_date, pool);
  // use only first 19 chars
  // 5 year + sep
  // 3 month + sep
  // 3 day + sep
  // 3 h + sep
  // 3 m + sep
  // 2 s
  wxString date = str.Left (19);
  return date;
}

BEGIN_EVENT_TABLE (FileListCtrl, wxListCtrl)
  EVT_KEY_DOWN (FileListCtrl::OnKeyDown)
  EVT_LIST_ITEM_ACTIVATED (-1, FileListCtrl::OnItemActivated)
  EVT_LIST_ITEM_RIGHT_CLICK (FILELIST_CTRL, FileListCtrl::OnItemRightClk)
  EVT_LIST_COL_CLICK (FILELIST_CTRL, FileListCtrl::OnColumnLeftClick)
  EVT_LIST_COL_END_DRAG (FILELIST_CTRL, FileListCtrl::OnColumnEndDrag)
END_EVENT_TABLE ()

FileListCtrl::FileListCtrl (wxWindow * parent, const wxWindowID id, 
                            const wxPoint & pos, const wxSize & size)
  : wxListCtrl (parent, id, pos, size, wxLC_REPORT)
{
  m = new Data ();

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
  SetImageList (m->ImageListSmall, wxIMAGE_LIST_SMALL);

  // Get settings from config file:
  wxConfigBase *pConfig = wxConfigBase::Get ();
  m->SortColumn = pConfig->Read (ConfigSortColumn, (long) 0);
  m->SortIncreasing = pConfig->Read (ConfigSortOrder, (long) 1) ? true : false;

  wxListItem itemCol;
  itemCol.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_IMAGE;
  itemCol.m_image = -1;

  int col;
  for (col = 0; col < COL_COUNT; col++)
  {
    wxString key;
    key.Printf (ConfigColumnVisibleFmt, col);
    m->ColumnVisible[col] = pConfig->Read (key, (long) 1) != 0;
    
    key.Printf (ConfigColumnWidthFmt, col);
    long width = (long)GetDefaultWidth (col);
    m->ColumnWidth[col] = pConfig->Read (key, width);
  }
  m->DirtyColumns = true;
}

FileListCtrl::~FileListCtrl ()
{
  // Write settings to config file:
  wxConfigBase *pConfig = wxConfigBase::Get ();
  pConfig->Write (ConfigSortColumn, (long) m->SortColumn);
  pConfig->Write (ConfigSortOrder, (long) (m->SortIncreasing ? 1 : 0));
  for (int col=0; col < COL_COUNT; col++)
  {
    wxString key;
    key.Printf(ConfigColumnWidthFmt, col);
    pConfig->Write (key, (long) m->ColumnWidth[col]);

    key.Printf(ConfigColumnVisibleFmt, col);
    pConfig->Write (key, (long) m->ColumnVisible[col]);
  }

  DeleteAllItems ();
  delete m;
}

void
FileListCtrl::UpdateFileList (const wxString & path)
{
  m->Path = path;

  UpdateFileList ();
}

void
FileListCtrl::UpdateFileList ()
{
  const wxString & path = m->Path;
  // delete all the items in the list to display the new ones
  DeleteAllItems ();

  UpdateColumns ();

  // cycle through the files and folders in the current path
  wxFileName fullpath (path, "*");
  // wxString name;

  wxLogStatus (_("Listing entries in '%s'"), path.c_str ());

  // Hide the list to speed up inserting
  Hide ();

  std::string stdpath(path.c_str());

  svn::Context context;
  context.setLogin ("", "");

  svn::Client client (&context);
  const svn::StatusEntries statusVector =
    client.status (path.c_str (), FALSE);
  svn::StatusEntries::const_iterator it;

  for (it = statusVector.begin (); it != statusVector.end (); it++)
  {
    const svn::Status & status = *it;
    wxString values[COL_COUNT];

    int i = GetItemCount ();

    if( status.path() == stdpath )
    {
      values[COL_NAME] = ".";
    }
    else
    {
      values[COL_NAME] = wxFileNameFromPath (status.path ());
    }

    wxString text;

    int imageIndex = 0;

    if (status.isVersioned ())
    {
      if (IsDir (&status))
      {
        imageIndex = GetImageIndex (IMG_INDX_VERSIONED_FOLDER);
      }
      else
      {
        imageIndex = GetImageIndex (status.textStatus ());
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

    InsertItem (i, values[COL_NAME], imageIndex);

    // The item data will be used to sort the list:
    SetItemData (i, (long)new svn::Status (status));    // The control now owns this data
    // and must delete it in due course.

    text = "";

    if (status.isVersioned ())
    {
      const svn::Entry & entry = status.entry ();
      values[COL_REV].Printf ("%ld", entry.revision ());
      values[COL_CMT_REV].Printf ("%ld", entry.cmtRev ());

      values[COL_AUTHOR] = entry.cmtAuthor ();

      // date formatting
      values[COL_CMT_DATE] = FormatDate (entry.cmtDate ());
      values[COL_TEXT_TIME] = FormatDate (entry.textTime ());
      values[COL_PROP_TIME] = FormatDate (entry.propTime ());

      values[COL_URL] = entry.url ();
      values[COL_REPOS] = entry.repos ();
      values[COL_UUID] = entry.uuid ();
      
      wxString schedule;
      switch (entry.schedule ())
      {
      case svn_wc_schedule_add:
        schedule = _("add");
        break;
      case svn_wc_schedule_delete:
        schedule = _("delete");
        break;
      case svn_wc_schedule_replace:
        schedule = _("replace");
        break;
      }
      values[COL_SCHEDULE] = schedule;

      if (entry.isCopied ())
      {
        values[COL_COPIED].Printf("%s, %ld", 
                                  entry.copyfromUrl (),
                                  entry.copyfromRev ());
      }

      values[COL_CONFLICT_OLD] = entry.conflictOld ();
      values[COL_CONFLICT_NEW] = entry.conflictNew ();
      values[COL_CONFLICT_WRK] = entry.conflictWrk ();
      values[COL_CHECKSUM] = entry.checksum ();
    }
    switch(status.textStatus ())
    {
    case svn_wc_status_none:
    case svn_wc_status_normal:
      // empty text for them
      break;
    default:
      values[COL_TEXT_STATUS] = 
        svn::Status::statusDescription (status.textStatus ());
      break;
    }
    switch(status.propStatus ())
    {
    case svn_wc_status_none:
    case svn_wc_status_normal:
      // empty text
      break;
    default:
      values[COL_PROP_STATUS] = 
        svn::Status::statusDescription (status.propStatus ());
      break;
    }

    // set the text for all visible items
    // (ignore column 0 since this is already set with
    // InsertItem
    for (int col=1; col<COL_COUNT; col++)
    {
      int index = m->ColumnIndex[col];
      if (index != -1)
      {
        SetItem (i, index, values[col]);
      }
    }
  }

  SortItems (Data::CompareFunction, (long) this->m);

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
  //Unused now
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
  if (Column == m->SortColumn)
    m->SortIncreasing = !m->SortIncreasing;
  else
  {
    m->SortColumn = Column;
    m->SortIncreasing = true;
  }

  SetColumnImages ();
  SortItems (Data::CompareFunction, (long) this->m);
}

void
FileListCtrl::ShowMenu (long index, wxPoint & pt)
{
  wxFileName filepath (m->Path, GetItemText (index));
  wxMenu popup_menu;
  wxString path = filepath.GetFullPath ();

  buildMenu (popup_menu, UnixPath (path));

  PopupMenu (&popup_menu, pt);
}

void
FileListCtrl::buildMenu (wxMenu & menu, const wxString & path)
{
  wxMenuItem *pItem;

  pItem = new wxMenuItem (&menu, ID_Update, _("Update"));
  pItem->SetBitmap (wxBITMAP (update));
  menu.Append (pItem);
  pItem = new wxMenuItem (&menu, ID_Commit, _("Commit"));
  pItem->SetBitmap (wxBITMAP (commit));
  menu.Append (pItem);
  pItem = new wxMenuItem (&menu, ID_CopyTo, _("Copy"));
  menu.Append (pItem);
  pItem = new wxMenuItem (&menu, ID_MoveTo, _("Move"));
  menu.Append (pItem);

  menu.AppendSeparator ();

  pItem = new wxMenuItem (&menu, ID_Revert, _("Revert"));
  pItem->SetBitmap (wxBITMAP (revert));
  menu.Append (pItem);
  pItem = new wxMenuItem (&menu, ID_Resolve, _("Resolve"));
  pItem->SetBitmap (wxBITMAP (resolve));
  menu.Append (pItem);

  menu.AppendSeparator ();

  pItem = new wxMenuItem (&menu, ID_RenameHere, _("Rename"));
  menu.Append (pItem);
  pItem = new wxMenuItem (&menu, ID_Info, _("Info"));
  pItem->SetBitmap (wxBITMAP (info));
  menu.Append (pItem);
  pItem = new wxMenuItem (&menu, ID_Log, _("Log"));
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
    wxFileName fname (m->Path, GetItemText (arr[i]));
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

  wxFileName fname (m->Path, GetItemText (index));
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
      SetItemData (i, 0);
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

void 
FileListCtrl::ResetColumns ()
{
  for (int col=0; col < COL_COUNT; col++)
  {
    bool visible;
    switch (col)
    {
    case COL_URL:
    case COL_REPOS:
    case COL_UUID:
    case COL_SCHEDULE:
    case COL_COPIED:
    case COL_CONFLICT_OLD:
    case COL_CONFLICT_NEW:
    case COL_CONFLICT_WRK:
      visible = false;
      break;

    default:
      visible = true;
      break;
    }
    m->ColumnVisible[col] = visible;
    SetColumnWidth (col, GetDefaultWidth (col));
    m->DirtyColumns = true;
  }
}

void 
FileListCtrl::SetColumnImages()
{
  // Update the column titles to reflect the sort column.
  for (int col = 0; col < COL_COUNT; col++)
  {
    int index = m->ColumnIndex[col];
    if (index == -1)
      continue;

    wxListItem item;
    item.m_mask = wxLIST_MASK_IMAGE;
    if (col == m->SortColumn)
    {
      item.m_image = m->SortIncreasing ?
        IMAGE_INDEX[IMG_INDX_SORT_DOWN] : IMAGE_INDEX[IMG_INDX_SORT_UP];
    }
    else
    {
      item.m_image = -1;
    }
    SetColumn (index, item);
  }
}

void
FileListCtrl::SetColumnVisible (const int col, const bool visible)
{
  if (col == COL_NAME)
    return;

  if ((col >= 0) && (col < COL_COUNT))
  {
    m->ColumnVisible[col] = visible;
  }

  if ((col == m->SortColumn) && (visible == false))
  {
    m->SortColumn = COL_NAME;
    m->SortIncreasing = true;
  }
  m->DirtyColumns =true;
}

const bool
FileListCtrl::GetColumnVisible (const int col)
{
  if ((col >= 0) && (col < COL_COUNT))
  {
    return m->ColumnVisible[col];
  }
  else
  {
    return false;
  }
}

void
FileListCtrl::UpdateColumns ()
{
  if (!m->DirtyColumns)
    return;

  // rebuild the index of columns
  int index = 0;
  int count = 0;
  int col;
  for (col=0; col < COL_COUNT; col++)
  {
    if (m->ColumnVisible[col])
    {
      m->ColumnIndex[col] = index;
      index++;
      count++;
    }
    else
    {
      m->ColumnIndex[col] = -1;
    }
  }

  // delete all items
  DeleteAllItems ();

  // adapt the column count
  while (GetColumnCount () > count)
  {
    DeleteColumn (0);
  }
    
  while (GetColumnCount () < count)
  {
    InsertColumn (0, "");
  }

  // Now set the captions and widths
  for (col=0; col < COL_COUNT; col++)
  {
    int index = m->ColumnIndex[col];
    if (index != -1)
    {
      wxListItem item;
      item.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_WIDTH;
      item.m_text = COLUMN_CAPTIONS[col];
      item.m_width = m->ColumnWidth[col];
      SetColumn (index, item);
    }
  }

  SetColumnImages ();

  m->DirtyColumns = false;
}

void 
FileListCtrl::OnColumnEndDrag (wxListEvent & event)
{
  int index = event.GetColumn ();
  for (int col=0; col < COL_COUNT; col++)
  {
    if (m->ColumnIndex[col] == index)
    {
      m->ColumnWidth[col] = GetColumnWidth (col);
      break;
    }
  }
}

void
FileListCtrl::SetColumnWidth (const int col, const int width)
{
  m->ColumnWidth[col] = width;
  
  int index = m->ColumnIndex[col];
  if (index != -1)
  {
    wxListCtrl::SetColumnWidth(index, width);
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
