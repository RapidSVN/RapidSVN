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
// stl
#include <exception>

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
#include "verblist.hpp"

// Bitmaps
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
static const char ConfigColumnWidthFmt[] = "/FileListCtrl/Column%sWidth";
static const char ConfigColumnVisibleFmt[] = "/FileListCtrl/Column%sVisible";

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
  case FileListCtrl::COL_PATH:
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

/**
 * compare two paths
 *
 * @param p1
 * @param p2
 * @return < -> -1 / = -> 0 / > -> 1
 */
static int
ComparePaths (const char * p1, const char * p2)
{
  wxFileName fn1 (p1);
  wxFileName fn2 (p2);

  wxString path1 = fn1.GetPath ();
  wxString path2 = fn2.GetPath ();

  int res = 0;

  // Is p2 a subdir or entry of p1?
  if (path1.CmpNoCase (p2) == 0)
  {
    return -1;
  }

  // Is p1 a subdir or entry of p2?
  if (path2.CmpNoCase (p1) == 0)
  {
    return 1;
  }

  // first compare path component
  res = path1.CmpNoCase (path2);

  if (res == 0)
  {
    wxString file1 = fn1.GetName ();
    wxString file2 = fn2.GetName ();

    res = file1.CmpNoCase (file2);
  }

  return res;
}

/**
 * compare two unsigned long numbers
 *
 * @param val1 value 1
 * @param val2 value 2
 * @return result of comparison
 * @retval 0 both are equal
 * @retval 1 rev1 > rev2
 * @retval -1 rev1 < rev2
 */
static int
Compare (unsigned long rev1, unsigned long rev2)
{
  if (rev1 == rev2)
  {
    return 0;
  }
  return rev1 > rev2 ? 1 : -1;
}

/**
 * Compare two strings
 *
 * @param s1 string 2
 * @param s2 string 2
 * @return < -> -1 / = -> 0 / > -> 1
 */
static int
Compare (const char * s1, const char * s2)
{
  const char * sv1 = s1 == 0 ? "" : s1;
  const char * sv2 = s2 == 0 ? "" : s2;
  
  return wxString (sv1).CmpNoCase (sv2);
}

/**
 * compare the given @a svn::Status entries
 *
 * @param ps1 item to compare
 * @param ps2 item to compare
 * @param column to compare
 * @return -1
 */
static int 
CompareColumn (svn::Status * ps1,
               svn::Status * ps2,
               int column)
{
  int res = 0;
  unsigned long r1 = 0, r2 = 0;
  bool ok1 = ps1->isVersioned ();
  bool ok2 = ps2->isVersioned ();
  svn::Entry e1 (ps1->entry ());
  svn::Entry e2 (ps2->entry ());

  switch (column)
  {
  case FileListCtrl::COL_NAME:                     
    res = Compare (ps1->path (), ps2->path ());
    break;

  case FileListCtrl::COL_PATH: 
    res = ComparePaths (ps1->path (), ps2->path ());
    break;

  case FileListCtrl::COL_REV:     
    res = Compare (e1.revision (), e2.revision ());
    break;

  case FileListCtrl::COL_CMT_REV:     
    res = Compare (e1.cmtRev (), e2.cmtRev ());
    break;

  case FileListCtrl::COL_AUTHOR:
    res = Compare (e1.cmtAuthor (), e2.cmtAuthor ());
    break;

  case FileListCtrl::COL_TEXT_STATUS:
    res = Compare (ps1->textStatus (), ps2->textStatus ());
    break;

  case FileListCtrl::COL_PROP_STATUS:
    res = Compare (ps2->propStatus (), ps2->propStatus ());
    break;

  case FileListCtrl::COL_CMT_DATE:
    res = Compare (e1.cmtDate (), e2.cmtDate ());
    break;

  case FileListCtrl::COL_TEXT_TIME:
    res = Compare (e1.textTime (), e2.textTime ());
    break;

  case FileListCtrl::COL_PROP_TIME:
    res = Compare (e1.propTime (), e2.propTime ());
    break;

  case FileListCtrl::COL_CHECKSUM:
    res = Compare (e1.checksum (), e2.checksum ());
    break;

  case FileListCtrl::COL_URL:
    res = Compare (e1.url (), e2.url ());
    break;

  case FileListCtrl::COL_REPOS:
    res = Compare (e1.url (), e2.url ());
    break;

  case FileListCtrl::COL_UUID:
    res = Compare (e1.uuid (), e2.uuid ());
    break;

  case FileListCtrl::COL_SCHEDULE:
    res = Compare (e1.schedule (), e2.schedule ());
    break;

  case FileListCtrl::COL_COPIED:
    res = Compare (e1.isCopied (), e2.isCopied ());
    break;

  case FileListCtrl::COL_CONFLICT_OLD:
    res = Compare (e1.conflictOld (), e2.conflictOld ());
    break;

  case FileListCtrl::COL_CONFLICT_NEW:
    res = Compare (e1.conflictNew (), e2.conflictNew ());
    break;

  case FileListCtrl::COL_CONFLICT_WRK:
    res = Compare (e1.conflictWrk (), e2.conflictWrk ());
    break;

  default:
    res = 0;
  }

  return res;
}

/**
 * check the given @a svn::Status entries and the 
 * sort settings. Not only the @a SortColumn is
 * used for comparison but secondary columns as
 * well so the entries are always sorted in
 * unique way. Lets say we want to sort for COL_REV and
 * all the entries have the same revision. We will
 * use COL_PATH and COL_NAME so the listing will still
 * be unique.
 *
 * @todo check the sort algorithm, especially the
 *       switch statements...
 */
static int
CompareItems (svn::Status * ps1, svn::Status * ps2,
              int SortColumn, bool SortIncreasing)
{
  int res = 0;

  // Directories always precede files:
  if (IsDir (ps1) && !IsDir (ps2))
    res = -1;
  else if (!IsDir (ps1) && IsDir (ps2))
    res = 1;

  if (res != 0)
  {
    return res;
  }

  switch (SortColumn)
  {
  case FileListCtrl::COL_NAME:                     
    res = CompareColumn (ps1, ps2, FileListCtrl::COL_NAME);
    if (res == 0)
    {
      res = CompareColumn (ps1, ps2, FileListCtrl::COL_PATH);
    }
    break;

  case FileListCtrl::COL_PATH:
    res = CompareColumn (ps1, ps2, FileListCtrl::COL_PATH);
    if (res == 0)
    {
      res = CompareColumn (ps1, ps2, FileListCtrl::COL_NAME);
    }
    break;

  default:
    res = CompareColumn (ps1, ps2, SortColumn);
    if (res == 0)
    {
      res = CompareColumn (ps1, ps2, FileListCtrl::COL_PATH);
    }
    if (res == 0)
    {
      res = CompareColumn (ps1, ps2, FileListCtrl::COL_NAME);
    }
    break;
  }

  if (!SortIncreasing)
  {
    res = res * -1;
  }

  return res;
}


/**
 * array with column captions
 */
static const char * 
COLUMN_CAPTIONS[FileListCtrl::COL_COUNT] =
{
  _("Name"),
  _("Path"),
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
 * array with column names. These names are
 * used to save and retrieve column specific
 * preferences.
 * 
 * Note: these string must not be translatable
 *       so dont enclose them in _( )
 */
static const char *
COLUMN_NAMES[FileListCtrl::COL_COUNT] =
{
  "Name",
  "Path",
  "Revision",
  "RepRev",
  "Author",
  "Status",
  "PropStatus",
  "LastChanged",
  "Date",
  "PropDate",
  "Checksum",
  "Url",
  "Repository",
  "UUID",
  "Schedule",
  "Copied",
  "ConflictOld",
  "ConflictNew",
  "ConflictWork"
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
  bool FlatMode;
  wxString Path;
  wxImageList *ImageListSmall;
  bool ColumnVisible[COL_COUNT];
  wxString ColumnCaption[COL_COUNT];
  int ColumnIndex[COL_COUNT];
  int ColumnWidth[COL_COUNT];

  /** default constructor */
  Data ()
    : SortIncreasing (true), SortColumn (COL_NAME), 
      DirtyColumns (true), FlatMode (false)
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
   * get the real column. @a clickedColumn is only the index
   * of the visible column that got clicked. If there are
   * invisible columns this translates to something completely different
   */
  int 
  GetRealColumn (int clickedColumn)
  {
    int colIndex;
    int column = clickedColumn;

    for (colIndex = 0; colIndex <= clickedColumn; colIndex++)
    {
      if (!ColumnVisible[colIndex])
      {
        column++;
      }
    }

    return column;
  }

  /**
   * callback function for @a wxListCtrl::SortColumns
   */
  static int wxCALLBACK
  CompareFunction (long item1, long item2, long sortData)
  {
    svn::Status * ps1 = (svn::Status *) item1;
    svn::Status * ps2 = (svn::Status *) item2;
    Data *data = (Data *) (sortData);

    if (ps1 && ps2) 
      return CompareItems (ps1, ps2, data->SortColumn, 
                           data->SortIncreasing);
    else
      return 0;
  }

  /**
   * read the preferences for the filelist
   * in this case the visibilty and width of the columns
   */
  void
  ReadConfig ()
  {
      // Get settings from config file:
    wxConfigBase *config = wxConfigBase::Get ();
    SortColumn = config->Read (ConfigSortColumn, (long) 0);
    SortIncreasing = config->Read (ConfigSortOrder, (long) 1) ? true : false;

    int col;
    for (col = 0; col < COL_COUNT; col++)
    {
      wxString key;
      key.Printf (ConfigColumnVisibleFmt, COLUMN_NAMES[col]);
      ColumnVisible[col] = config->Read (key, (long) 1) != 0;
  
      key.Printf (ConfigColumnWidthFmt, COLUMN_NAMES[col]);
      long width = (long)GetDefaultWidth (col);
      ColumnWidth[col] = config->Read (key, width);
    }
    ColumnVisible[COL_PATH] = FlatMode;
  }

  /**
   * save the preferences for the filelist
   */
  void
  WriteConfig ()
  {
    // Write settings to config file:
    wxConfigBase *config = wxConfigBase::Get ();
    config->Write (ConfigSortColumn, (long) SortColumn);
    config->Write (ConfigSortOrder, (long) (SortIncreasing ? 1 : 0));
    // loop through all the columns
    for (int col=0; col < COL_COUNT; col++)
    {
      wxString key;
      key.Printf (ConfigColumnWidthFmt, COLUMN_NAMES[col]);
      config->Write (key, (long) ColumnWidth[col]);

      key.Printf (ConfigColumnVisibleFmt, COLUMN_NAMES[col]);
      config->Write (key, (long) ColumnVisible[col]);
    }
  }
};  


/**
 * A safe wrapper for getting images - avoids array bounds
 * exceptions.
 */
static int
GetImageIndex (int textIndex, int propIndex)
{
  int image = 0;
  if ((textIndex >= 0) && (textIndex <= N_STATUS_KIND))
  {
    if ( (textIndex == svn_wc_status_normal) && 
         (propIndex > svn_wc_status_normal) )
    {
      if ( (propIndex >= 0) && (propIndex <= N_STATUS_KIND) )
      {
        image = IMAGE_INDEX[propIndex];
      }
    }
    else
    {
      image = IMAGE_INDEX[textIndex];
    }
  }

  return image;
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
  EVT_LEFT_DCLICK (FileListCtrl::OnDoubleClick)
  EVT_LIST_ITEM_ACTIVATED (-1, FileListCtrl::OnItemActivated)
  EVT_LIST_ITEM_RIGHT_CLICK (FILELIST_CTRL, FileListCtrl::OnItemRightClk)
  EVT_LIST_COL_CLICK (FILELIST_CTRL, FileListCtrl::OnColumnLeftClick)
  EVT_LIST_COL_END_DRAG (FILELIST_CTRL, FileListCtrl::OnColumnEndDrag)
END_EVENT_TABLE ()

FileListCtrl::FileListCtrl (wxWindow * parent, const wxWindowID id, 
                            const wxPoint & pos, const wxSize & size)
  : wxListView (parent, id, pos, size, wxLC_REPORT)
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

  m->ReadConfig ();

  m->DirtyColumns = true;
}

FileListCtrl::~FileListCtrl ()
{
  m->WriteConfig ();

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

  wxLogStatus (_("Listing entries in '%s'"), path.c_str ());

  // Hide the list to speed up inserting
  Hide ();

  std::string stdpath (path.c_str ());

  svn::Context context;
  context.setLogin ("", "");

  svn::Client client (&context);
  const svn::StatusEntries statusVector =
    client.status (path.c_str (), m->FlatMode);
  svn::StatusEntries::const_iterator it;
  const size_t pathLength = path.Length () + 1;

  for (it = statusVector.begin (); it != statusVector.end (); it++)
  {
    const svn::Status & status = *it;
    wxString values[COL_COUNT];

    int i = GetItemCount ();

    // truncate the first part of the path
    wxString fullPath (status.path ());
    wxString filename (fullPath.Mid (pathLength));
    if (filename.Length () == 0)
    {
      values[COL_NAME] = ".";
    }
    else
    {
      values[COL_NAME] = wxFileNameFromPath (filename);
    }
    if (m->ColumnVisible[COL_PATH])
    {
      wxFileName path (fullPath);
      values[COL_PATH] = path.GetFullPath ();
    }

    wxString text;

    int imageIndex = 0;

    if (status.isVersioned ())
    {
      if (IsDir (&status))
      {
        imageIndex = GetImageIndex (IMG_INDX_VERSIONED_FOLDER, -1);
      }
      else
      {
        imageIndex = GetImageIndex (status.textStatus (), status.propStatus ());
      }
    }
    else
    {
      // unversioned entries dont carry dir info
      // with them. must find this out by ourself
      if (wxDirExists (status.path ()))
      {
        imageIndex = GetImageIndex (IMG_INDX_FOLDER, -1);
      }
      else if (wxFileExists (status.path ()))
      {
        imageIndex = GetImageIndex (svn_wc_status_unversioned, -1);
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
      case svn_wc_schedule_normal:
        schedule = "";
        break;
      }
      values[COL_SCHEDULE] = schedule;

      if (entry.isCopied ())
      {
        values[COL_COPIED].Printf ("%s, %ld", 
                                  entry.copyfromUrl (),
                                  entry.copyfromRev ());
      }

      values[COL_CONFLICT_OLD] = entry.conflictOld ();
      values[COL_CONFLICT_NEW] = entry.conflictNew ();
      values[COL_CONFLICT_WRK] = entry.conflictWrk ();
      values[COL_CHECKSUM] = entry.checksum ();
    }
    switch (status.textStatus ())
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
    switch (status.propStatus ())
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

  SortItems (Data::CompareFunction, (long)this->m);

  Show ();

  wxLogStatus (_("Ready"),"");
}

void 
FileListCtrl::OnDoubleClick (wxMouseEvent & event)
{
  int flag;
  
  // Don't post if the click didn't hit anything
  if (HitTest (ScreenToClient (wxGetMousePosition ()), flag) >= 0)
    if (!PostMenuEvent (this, ID_Default_Action))
      event.Skip ();
}


void 
FileListCtrl::OnKeyDown (wxKeyEvent & event)
{
  switch (event.GetKeyCode ())
  {
    case WXK_RETURN:
      if (!PostMenuEvent (this, ID_Default_Action))
        event.Skip ();
      break;
    
    default:
      event.Skip ();
      break;
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
  int clickedColumn = event.GetColumn ();

  // First we have to get the real column
  int column = m->GetRealColumn (clickedColumn);

  // A second click on the current sort column reverses the order of sorting.
  if (column == m->SortColumn)
    m->SortIncreasing = !m->SortIncreasing;
  else
  {
    m->SortColumn = column;
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
  VerbList verb_list;

  // modify menu
  AppendModifyMenu (&menu);

  menu.AppendSeparator ();

  // query menu
  AppendQueryMenu (&menu);
  

  // Append file verbs
  try
  {
    verb_list.InitFromDocument (path);
  }
  catch (std::exception)
  {
    // Failed assembling verbs. 
    // TODO: Report this error in the status bar?
  }

  if (verb_list.GetCount ())
  {
    menu.AppendSeparator ();
    for (size_t i = 0; (i < verb_list.GetCount ()) && (i < (ID_Verb_Max - ID_Verb_Min + 1)); i++)
    {
      wxMenuItem *pItem;
      // TODO: Convert verb names to unicode on the fly if needed (or make
      // verblist follow wxWindows' unicode setting)
      pItem = new wxMenuItem (&menu, ID_Verb_Min + i, verb_list.GetName (i));
      //pItem->SetBitmap (wxBITMAP (?))
      menu.Append (pItem);
    }
  }
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

    case COL_PATH:
      visible = m->FlatMode;
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
FileListCtrl::SetColumnImages ()
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

  // TODO clean this mess up. We dont want #ifdef's in the source code

#ifdef __WXMSW__
  // this works only with wxMSW
  const wxListItem item = event.GetItem ();
  const int width = item.GetWidth ();
#else
  // and this is for all the other platforms
  const int width = GetColumnWidth (index);
#endif
  for (int col=0; col < COL_COUNT; col++)
  {
    if (m->ColumnIndex[col] == index)
    {
      m->ColumnWidth[col] = width;
      break;
    }
  }
  event.Skip ();
}

void
FileListCtrl::SetColumnWidth (const int col, const int width)
{
  m->ColumnWidth[col] = width;
  
  int index = m->ColumnIndex[col];
  if (index != -1)
  {
    wxListCtrl::SetColumnWidth (index, width);
  }
}

void
FileListCtrl::SetFlat (const bool flat)
{
  m->FlatMode = flat;
  SetColumnVisible (COL_PATH, flat);
}

bool
FileListCtrl::IsFlat ()
{
  return m->FlatMode;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
