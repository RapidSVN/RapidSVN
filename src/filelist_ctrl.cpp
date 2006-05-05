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
#if defined( _MSC_VER) && _MSC_VER <= 1200
#pragma warning( disable: 4786 )// debug symbol truncated
#endif
// stl
#include <exception>
#include <map>

// wxWidgets
#include "wx/wx.h"
#include "wx/confbase.h"
#include "wx/filename.h"
#include "wx/imaglist.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/entry.hpp"
#include "svncpp/targets.hpp"
#include "svncpp/url.hpp"

// app
#include "action.hpp"
#include "action_event.hpp"
#include "columns.hpp"
#include "filelist_ctrl.hpp"
#include "ids.hpp"
#include "preferences.hpp"
#include "rapidsvn_app.hpp"
#include "rapidsvn_frame.hpp"
#include "utils.hpp"

// Bitmaps
#include "res/bitmaps/nonsvn_file.xpm"
#include "res/bitmaps/normal_file.xpm"
#include "res/bitmaps/added_file.xpm"
#include "res/bitmaps/missing_file.xpm"
#include "res/bitmaps/deleted_file.xpm"
#include "res/bitmaps/replaced_file.xpm"
#include "res/bitmaps/modified_file.xpm"
#include "res/bitmaps/merged_file.xpm"
#include "res/bitmaps/conflicted_file.xpm"
#include "res/bitmaps/folder.xpm"
#include "res/bitmaps/versioned_folder.xpm"
#include "res/bitmaps/sort_down.xpm"
#include "res/bitmaps/sort_up.xpm"
#include "res/bitmaps/modified_versioned_folder.xpm"
#include "res/bitmaps/newer_file.xpm"
#include "res/bitmaps/newer_folder.xpm"
#include "res/bitmaps/modified_newer_file.xpm"
#include "res/bitmaps/externals_folder.xpm"

#include "res/bitmaps/locked_normal_file.xpm"
#include "res/bitmaps/locked_missing_file.xpm"
#include "res/bitmaps/locked_deleted_file.xpm"
#include "res/bitmaps/locked_replaced_file.xpm"
#include "res/bitmaps/locked_modified_file.xpm"
#include "res/bitmaps/locked_merged_file.xpm"
#include "res/bitmaps/locked_conflicted_file.xpm"
#include "res/bitmaps/locked_newer_file.xpm"
#include "res/bitmaps/locked_modified_newer_file.xpm"

#include "res/bitmaps/replock_normal_file.xpm"
#include "res/bitmaps/replock_missing_file.xpm"
#include "res/bitmaps/replock_deleted_file.xpm"
#include "res/bitmaps/replock_replaced_file.xpm"
#include "res/bitmaps/replock_modified_file.xpm"
#include "res/bitmaps/replock_merged_file.xpm"
#include "res/bitmaps/replock_conflicted_file.xpm"
#include "res/bitmaps/replock_newer_file.xpm"
#include "res/bitmaps/replock_modified_newer_file.xpm"


/**
 * Tags for wxConfig file settings, defined here to avoid duplicate
 * hard coded strings.
 */
static const wxChar ConfigSortColumn[]       = wxT("/FileListCtrl/SortColumn");
static const wxChar ConfigSortOrder[]        = wxT("/FileListCtrl/SortOrder");
static const wxChar ConfigColumnWidthFmt[]   = wxT("/FileListCtrl/Column%sWidth");
static const wxChar ConfigColumnVisibleFmt[] = wxT("/FileListCtrl/Column%sVisible");
static const wxChar ConfigWithUpdate[]       = wxT("/FileListCtrl/WithUpdate");
static const wxChar ConfigFlatView[]         = wxT("/FileListCtrl/FlatView");
static const wxChar ConfigShowUnversioned[]  = wxT("/FileListCtrl/ShowUnversioned");
static const wxChar ConfigIgnoreExternals[]  = wxT("/FileListCtrl/IgnoreExternals");
static const wxChar ConfigIncludePath[]      = wxT("/FileListCtrl/IncludePath");
static const wxChar ConfigSortAscending[]    = wxT("/FileListCtrl/SortAscending");


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

  // versioned resource?
  const svn::Entry & entry = status->entry ();

  if (entry.isValid ())
  {
    return entry.kind () == svn_node_dir;
  }

  // unversioned.
  return wxDirExists (Utf8ToLocal (status->path ()));
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

#define FAKE_SVN_WC_OUT_OF_DATE  ((svn_wc_status_kind)999)

/**
 * compare two svn_wc_status_kind enums
 *
 * @param status1 status value 1
 * @param status2 status value 2
 * @param newer1 if item1 is also newer
 * @param newer2 if item2 is also newer
 * @return result of comparison
 * @retval 0 both are equal
 * @retval 1 status1 > status2
 * @retval -1 status1 < status2
 */
static int
Compare (svn_wc_status_kind status1, svn_wc_status_kind status2, bool newer1, bool newer2)
{
  if (newer1 && status1 == svn_wc_status_normal)
  {
    status1 = FAKE_SVN_WC_OUT_OF_DATE;
  }
  if (newer2 && status2 == svn_wc_status_normal)
  {
    status2 = FAKE_SVN_WC_OUT_OF_DATE;
  }

  if (status1 == status2)
  {
    return 0;
  }

  static svn_wc_status_kind lookup[] =
  {
    svn_wc_status_external,
    svn_wc_status_ignored,
    svn_wc_status_incomplete,
    svn_wc_status_obstructed,
    svn_wc_status_unversioned,
    svn_wc_status_deleted,
    svn_wc_status_conflicted,
    svn_wc_status_added,
    svn_wc_status_missing,
    svn_wc_status_replaced,
    svn_wc_status_merged,
    svn_wc_status_modified,
    FAKE_SVN_WC_OUT_OF_DATE,
    svn_wc_status_normal,
    svn_wc_status_none,
    (svn_wc_status_kind)0
  };

  for (svn_wc_status_kind* pkind = lookup; *pkind; ++pkind)
  {
    if (*pkind == status1)
    {
      return -1;
    }
    if (*pkind == status2)
    {
      return 1;
    }
  }

  // Unrecognised either
  return 0;
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
  wxString wxp1 (Utf8ToLocal (p1));
  wxString wxp2 (Utf8ToLocal (p2));

  wxFileName fn1 (wxp1);
  wxFileName fn2 (wxp2);

  wxString path1 = fn1.GetPath ();
  wxString path2 = fn2.GetPath ();

  int res = 0;

  // Is p2 a subdir or entry of p1?
  if (path1.CmpNoCase (wxp2) == 0)
  {
    return -1;
  }

  // Is p1 a subdir or entry of p2?
  if (path2.CmpNoCase (wxp1) == 0)
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
 * compare two timestamps (compatible with unsigned long numbers,
 * but contain more digits)
 *
 * @param val1 value 1
 * @param val2 value 2
 * @return result of comparison
 * @retval 0 both are equal
 * @retval 1 rev1 > rev2
 * @retval -1 rev1 < rev2
 */
static int
Compare (const apr_time_t rev1, const apr_time_t rev2)
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

  wxString wx1 (Utf8ToLocal (sv1));
  wxString wx2 (Utf8ToLocal (sv2));

  return wx1.CmpNoCase (wx2);
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
  svn::Entry e1 (ps1->entry ());
  svn::Entry e2 (ps2->entry ());

  bool newer1 =
    (ps1->reposTextStatus () == svn_wc_status_modified) ||
    (ps1->reposPropStatus () == svn_wc_status_modified);
  bool newer2 =
    (ps2->reposTextStatus () == svn_wc_status_modified) ||
    (ps2->reposPropStatus () == svn_wc_status_modified);

  svn::Path path1 (ps1->path ());
  svn::Path path2 (ps2->path ());

  switch (column)
  {
  case FileListCtrl::COL_NAME:
    res = Compare (path1.basename ().c_str (), path2.basename ().c_str ());
    break;

  case FileListCtrl::COL_PATH:
    res = ComparePaths (path1.c_str (), path2.c_str ());
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
    res = Compare (ps1->textStatus (), ps2->textStatus (), newer1, newer2);
    break;

  case FileListCtrl::COL_PROP_STATUS:
    res = Compare (ps1->propStatus (), ps2->propStatus (), newer1, newer2);
    break;

  case FileListCtrl::COL_EXTENSION:
    res = wxFileName(Utf8ToLocal (ps1->path())).GetExt().CmpNoCase (wxFileName(Utf8ToLocal (ps2->path())).GetExt());
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
              int SortColumn, bool SortAscending,
              bool IncludePath, size_t RootPathLength)
{
  int res = 0;

  // Directories always precede files AND
  // Current working directory '.' always first
  if (IsDir (ps1) &&
      (!IsDir (ps2) || ((std::string) ps1->path ()).length () <= RootPathLength))
  {
    res = -1;
  }
  else if (IsDir (ps2) &&
           (!IsDir (ps1) || ((std::string) ps2->path ()).length () <= RootPathLength))
  {
    res = 1;
  }

  if (res != 0)
    return res;

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

    // if IncludePath is true (Use full path in sorting), sort by full name
    // (path first, then name).
    // Otherwise, sort by name.
    if (res == 0)
    {
      res = CompareColumn (ps1, ps2, (IncludePath ? FileListCtrl::COL_PATH :
                                      FileListCtrl::COL_NAME));
    }
    if (res == 0)
    {
      res = CompareColumn (ps1, ps2, (IncludePath ? FileListCtrl::COL_NAME :
                                      FileListCtrl::COL_PATH));
    }
    break;
  }

  if (!SortAscending)
  {
    res = res * -1;
  }

  return res;
}


/**
 * The index from where there will be only images
 * not related to the status.
 * See @a svn_wc_status_kind in @a svn_wc.h for more details.
 */
enum
{
  IMG_INDX_FOLDER = 15,
  IMG_INDX_VERSIONED_FOLDER,
  IMG_INDX_SORT_DOWN,
  IMG_INDX_SORT_UP,
  IMG_INDX_MODIFIED_VERSIONED_FOLDER,
  IMG_INDX_NEWER_FILE,
  IMG_INDX_NEWER_FOLDER,
  IMG_INDX_MODIFIED_NEWER,
  IMG_INDX_EXTERNALS_FOLDER,
  IMG_INDX_COUNT
};


/**
 * structure that maps a status entry to an 
 * according XPM icon.
 */
struct MapItem
{
  int status;
  const char ** xpm_data;

  MapItem (int status_, const char **xpm_data_)
    : status (status_), xpm_data (xpm_data_)
  {
  }
};


/** array of icons and corresponding status */
static const MapItem MAP_ICON_ARRAY [] =
{
  MapItem (svn_wc_status_unversioned,           (const char**)nonsvn_file_xpm),
  MapItem (svn_wc_status_normal,                (const char**)normal_file_xpm),
  MapItem (svn_wc_status_added,                 (const char**)added_file_xpm),
  MapItem (svn_wc_status_missing,               (const char**)missing_file_xpm),
  MapItem (svn_wc_status_deleted,               (const char**)deleted_file_xpm),
  MapItem (svn_wc_status_replaced,              (const char**)replaced_file_xpm),
  MapItem (svn_wc_status_modified,              (const char**)modified_file_xpm),
  MapItem (svn_wc_status_merged,                (const char**)merged_file_xpm),
  MapItem (svn_wc_status_conflicted,            (const char**)conflicted_file_xpm),

  MapItem (IMG_INDX_FOLDER,                     (const char**)folder_xpm),
  MapItem (IMG_INDX_VERSIONED_FOLDER,           (const char**)versioned_folder_xpm),

  MapItem (IMG_INDX_SORT_DOWN,                  (const char**)sort_down_xpm),
  MapItem (IMG_INDX_SORT_UP,                    (const char**)sort_up_xpm),

  MapItem (IMG_INDX_MODIFIED_VERSIONED_FOLDER,  (const char**)modified_versioned_folder_xpm),
  MapItem (IMG_INDX_NEWER_FILE,                 (const char**)newer_file_xpm),
  MapItem (IMG_INDX_NEWER_FOLDER,               (const char**)newer_folder_xpm),
  MapItem (IMG_INDX_MODIFIED_NEWER,             (const char**)modified_newer_file_xpm),
  MapItem (IMG_INDX_EXTERNALS_FOLDER,           (const char**)externals_folder_xpm)
};

static const size_t MAP_ICON_COUNT = 
  sizeof (MAP_ICON_ARRAY) / sizeof (MAP_ICON_ARRAY [0]);

/** the same for icons with local lock */
static const MapItem MAP_LOCK_ICON_ARRAY [] =
{
  MapItem (svn_wc_status_normal,                (const char**)locked_normal_file_xpm),
  MapItem (svn_wc_status_missing,               (const char**)locked_missing_file_xpm),
  MapItem (svn_wc_status_deleted,               (const char**)locked_deleted_file_xpm),
  MapItem (svn_wc_status_replaced,              (const char**)locked_replaced_file_xpm),
  MapItem (svn_wc_status_modified,              (const char**)locked_modified_file_xpm),
  MapItem (svn_wc_status_merged,                (const char**)locked_merged_file_xpm),
  MapItem (svn_wc_status_conflicted,            (const char**)locked_conflicted_file_xpm),
  MapItem (IMG_INDX_NEWER_FILE,                 (const char**)locked_newer_file_xpm),
  MapItem (IMG_INDX_MODIFIED_NEWER,             (const char**)locked_modified_newer_file_xpm)
};

static const size_t MAP_LOCK_ICON_COUNT = 
  sizeof (MAP_LOCK_ICON_ARRAY) / sizeof (MAP_LOCK_ICON_ARRAY [0]);

/** the same for icons with repository lock */
static const MapItem MAP_REPLOCK_ICON_ARRAY [] =
{
  MapItem (svn_wc_status_normal,                (const char**)replock_normal_file_xpm),
  MapItem (svn_wc_status_missing,               (const char**)replock_missing_file_xpm),
  MapItem (svn_wc_status_deleted,               (const char**)replock_deleted_file_xpm),
  MapItem (svn_wc_status_replaced,              (const char**)replock_replaced_file_xpm),
  MapItem (svn_wc_status_modified,              (const char**)replock_modified_file_xpm),
  MapItem (svn_wc_status_merged,                (const char**)replock_merged_file_xpm),
  MapItem (svn_wc_status_conflicted,            (const char**)replock_conflicted_file_xpm),
  MapItem (IMG_INDX_NEWER_FILE,                 (const char**)replock_newer_file_xpm),
  MapItem (IMG_INDX_MODIFIED_NEWER,             (const char**)replock_modified_newer_file_xpm)
};

static const size_t MAP_REPLOCK_ICON_COUNT = 
  sizeof (MAP_REPLOCK_ICON_ARRAY) / sizeof (MAP_REPLOCK_ICON_ARRAY [0]);


/**
 * private struct that hide implementation details
 * to users of @a FileListCtrl
 */
struct FileListCtrl::Data
{
public:
  wxWindow * Parent;
  wxImageList * ImageListSmall;
  wxString Path;

  svn::Context * Context;
  Columns ColumnList;

  int SortColumn;
  bool IncludePath;
  bool SortAscending;
  bool DirtyColumns;
  bool FlatMode;
  bool WithUpdate;
  bool ShowUnversioned;
  bool IgnoreExternals;

  /**
   * This table holds information about image index in a image list.
   * It will be accessed using a status code as a first index and locked
   * flag as a second index.
   * For every index that equals a valid status code there should be
   * a valid value which represents an index in the image list
   * when the second index is 0 (meaning that file is not locked).
   * When second index equals 1 (for locked files), there are
   * corresponding image indeces for some states.
   */
  std::map<int,int> ImageIndexArray;

  bool ColumnVisible [COL_COUNT];
  int ColumnIndex [COL_COUNT];
  int ColumnWidth [COL_COUNT];

  Data ();
  ~Data ();

  int
  GetImageIndex (const svn::Status & status);

  int
  GetSortImageIndex (bool sortDown);

  int
  GetRealColumn (int clickedColumn);

  static int wxCALLBACK
  CompareFunction (long item1, long item2, long sortData);

  void
  ReadConfig ();

  void
  WriteConfig ();
};


/** default constructor */
FileListCtrl::Data::Data ()
  : Context (0), SortColumn (COL_NAME),
    IncludePath (true), SortAscending (true),
    DirtyColumns (true), FlatMode (false),
    WithUpdate (false), ShowUnversioned (true),
    IgnoreExternals (false)
{
  ImageListSmall = new wxImageList (16, 16, TRUE);

  /**
   * form neccessary ImageIndexArray and ImageListSmall arrays
   * from two previous arrays, first common, than for locked items.
   */
  size_t i;
  size_t lock_offset = 0;
  for(i=0; i < MAP_ICON_COUNT; i++)
  {
    const MapItem & item = MAP_ICON_ARRAY [i];

    ImageIndexArray [item.status + lock_offset] = i;
    ImageListSmall->Add (wxIcon (item.xpm_data));
  }

  lock_offset += IMG_INDX_COUNT;
  for(i=0; i < MAP_LOCK_ICON_COUNT; i++)
  {
    const MapItem & item = MAP_LOCK_ICON_ARRAY [i];

    ImageIndexArray [item.status + lock_offset] = MAP_ICON_COUNT + i;
    ImageListSmall->Add (wxIcon (item.xpm_data));
  }

  lock_offset += IMG_INDX_COUNT;
  for(i=0; i < MAP_REPLOCK_ICON_COUNT; i++)
  {
    const MapItem & item = MAP_REPLOCK_ICON_ARRAY [i];

    ImageIndexArray [item.status + lock_offset] = MAP_LOCK_ICON_COUNT + MAP_ICON_COUNT + i;
    ImageListSmall->Add (wxIcon (item.xpm_data));
  }
}

/** destructor */
FileListCtrl::Data::~Data ()
{
  delete ImageListSmall;
}

/**
 * A safe wrapper for getting images - avoids array bounds
 * exceptions.
 */
int
FileListCtrl::Data::GetImageIndex (const svn::Status & status)
{
  int imageIndex = 0;

  size_t lock_offset = 0;
  if (status.isRepLock ())
    lock_offset = 2 * IMG_INDX_COUNT;
  else if (status.isLocked ())
    lock_offset = IMG_INDX_COUNT;

  bool newer =
    (status.reposTextStatus () == svn_wc_status_modified) ||
    (status.reposPropStatus () == svn_wc_status_modified);

  if (status.isVersioned ())
  {
    int textIndex = status.textStatus ();
    int propIndex = status.propStatus ();

    if (IsDir (&status))
    {
      if ((textIndex == svn_wc_status_modified) || (propIndex == svn_wc_status_modified))
      {
        imageIndex = ImageIndexArray[IMG_INDX_MODIFIED_VERSIONED_FOLDER + lock_offset];
      }
      else if (textIndex == svn_wc_status_external)
      {
        imageIndex = ImageIndexArray[IMG_INDX_EXTERNALS_FOLDER + lock_offset];
      }
      else
      {
        imageIndex = ImageIndexArray[IMG_INDX_VERSIONED_FOLDER + lock_offset];
      }
      const int normalIndex = ImageIndexArray[IMG_INDX_VERSIONED_FOLDER + lock_offset];

      if ((imageIndex == normalIndex) && newer)
        imageIndex = ImageIndexArray[IMG_INDX_NEWER_FOLDER + lock_offset];
    }
    else
    {
      if ((textIndex >= 0) && (textIndex <= IMG_INDX_COUNT))
      {
        if ((textIndex == svn_wc_status_normal) && (propIndex > svn_wc_status_normal))
        {
          if ((propIndex >= 0) && (propIndex <= IMG_INDX_COUNT) )
            imageIndex = ImageIndexArray[propIndex + lock_offset];
        }
        else
          imageIndex = ImageIndexArray[textIndex + lock_offset];
      }
   
      if (newer)
      {
        if (imageIndex == ImageIndexArray[svn_wc_status_normal + lock_offset])
          imageIndex = ImageIndexArray[IMG_INDX_NEWER_FILE + lock_offset];
        else
          imageIndex = ImageIndexArray[IMG_INDX_MODIFIED_NEWER + lock_offset];
      }
    }
  }
  else
  {
    wxString wxFullPath = Utf8ToLocal (status.path ());

    // unversioned entries dont carry dir info
    // with them. must find this out by ourself
    if (wxDirExists (wxFullPath))
    {
      imageIndex = ImageIndexArray[IMG_INDX_FOLDER + lock_offset];
    }
    else if (wxFileExists (wxFullPath))
    {
      imageIndex = ImageIndexArray[svn_wc_status_unversioned + lock_offset];
    }
    else
    {
      imageIndex = ImageIndexArray[svn_wc_status_none + lock_offset];
    }
  }

  return imageIndex;
}

/**
 * A safe wrapper for getting images for sorting.
 */
inline int
FileListCtrl::Data::GetSortImageIndex (bool sortDown)
{
  if (sortDown)
    return ImageIndexArray[IMG_INDX_SORT_DOWN];

  return ImageIndexArray[IMG_INDX_SORT_UP];
}

/**
 * get the real column. @a clickedColumn is only the index
 * of the visible column that got clicked. If there are
 * invisible columns this translates to something completely different
 */
inline int
FileListCtrl::Data::GetRealColumn (int clickedColumn)
{
  int skipped = 0;
  for (int i = 0; (i < COL_COUNT && (i < clickedColumn+skipped || !ColumnVisible[i])); i++)
  {
    if (!ColumnVisible[i])
      skipped++;
  }
  return clickedColumn + skipped;
}

/**
 * callback function for @a wxListCtrl::SortColumns
 */
int wxCALLBACK
FileListCtrl::Data::CompareFunction (long item1, long item2, long sortData)
{
  svn::Status * ps1 = (svn::Status *) item1;
  svn::Status * ps2 = (svn::Status *) item2;
  Data *data = (Data *) (sortData);

  if (ps1 && ps2)
    return CompareItems (ps1, ps2, data->SortColumn,
                         data->SortAscending,
                         data->IncludePath,
                         data->Path.length ());
  else
    return 0;
}

/**
 * read the preferences for the filelist
 * in this case the visibilty and width of the columns
 */
inline void
FileListCtrl::Data::ReadConfig ()
{
  // Get settings from config file
  wxConfigBase *config = wxConfigBase::Get ();

  SortColumn = config->Read (ConfigSortColumn, (long) 0);
  SortAscending = config->Read (ConfigSortOrder, (long) 1) ? true : false;

  config->Read (ConfigIncludePath,     &IncludePath,     (bool) true);
  config->Read (ConfigFlatView,        &FlatMode);
  config->Read (ConfigWithUpdate,      &WithUpdate);
  config->Read (ConfigShowUnversioned, &ShowUnversioned, (bool) true); 
  config->Read (ConfigIgnoreExternals, &IgnoreExternals, (bool) false);

  for (int col = 0; col < COL_COUNT; col++)
  {
    wxString key;
    key.Printf (ConfigColumnVisibleFmt, ColumnList[col].name);
    ColumnVisible[col] = config->Read (key, (long) 1) != 0;

    key.Printf (ConfigColumnWidthFmt, ColumnList[col].name);
    long width = (long)GetDefaultWidth (col);
    ColumnWidth[col] = config->Read (key, width);
  }
  ColumnVisible[COL_PATH] = FlatMode;
}

/**
 * save the preferences for the filelist
 */
inline void
FileListCtrl::Data::WriteConfig ()
{
  // Write settings to config file
  wxConfigBase *config = wxConfigBase::Get ();

  config->Write (ConfigSortColumn, (long) SortColumn);
  config->Write (ConfigIncludePath,       IncludePath);
  config->Write (ConfigSortOrder, (long) (SortAscending ? 1 : 0));
  config->Write (ConfigFlatView,          FlatMode);
  config->Write (ConfigWithUpdate,        WithUpdate);
  config->Write (ConfigShowUnversioned,   ShowUnversioned);
  config->Write (ConfigIgnoreExternals,   IgnoreExternals);

  // loop through all the columns
  for (int col=0; col < COL_COUNT; col++)
  {
    wxString key;
    key.Printf (ConfigColumnWidthFmt, ColumnList[col].name);
    config->Write (key, (long) ColumnWidth[col]);

    key.Printf (ConfigColumnVisibleFmt, ColumnList[col].name);
    config->Write (key, (long) ColumnVisible[col]);
  }
}


BEGIN_EVENT_TABLE (FileListCtrl, wxListCtrl)
  EVT_KEY_DOWN (FileListCtrl::OnKeyDown)
  EVT_LEFT_DCLICK (FileListCtrl::OnDoubleClick)
  EVT_LIST_COL_CLICK (FILELIST_CTRL, FileListCtrl::OnColumnLeftClick)
  EVT_LIST_COL_END_DRAG (FILELIST_CTRL, FileListCtrl::OnColumnEndDrag)
  EVT_CONTEXT_MENU (FileListCtrl::OnContextMenu)
END_EVENT_TABLE ()

FileListCtrl::FileListCtrl (wxWindow * parent, const wxWindowID id,
                            const wxPoint & pos, const wxSize & size)
  : wxListView (parent, id, pos, size, wxLC_REPORT)
{
  m = new Data ();

  // set this file list control to use the image list
  SetImageList (m->ImageListSmall, wxIMAGE_LIST_SMALL);

  m->ReadConfig ();

  m->DirtyColumns = true;
  m->Parent = parent;
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
  svn::Path pathUtf8 (PathUtf8 (m->Path));

  // delete all the items in the list to display the new ones
  DeleteAllItems ();

  UpdateColumns ();

  wxLogStatus (_("Listing entries in '%s'"), m->Path.c_str ());

  // Hide the list to speed up inserting
  Hide ();

  svn::Client client (m->Context);

  const svn::StatusEntries statusVector =
    client.status (pathUtf8.c_str (), m->FlatMode, true, m->WithUpdate, false, m->IgnoreExternals);

  svn::StatusEntries::const_iterator it;
  for (it = statusVector.begin (); it != statusVector.end (); it++)
  {
    const svn::Status & status = *it;

    CreateLables (status, pathUtf8);
  }

  SortItems (Data::CompareFunction, (long) this->m);

  Show ();

  wxLogStatus (_("Ready"),"");
}

void
FileListCtrl::CreateLables (const svn::Status & status, const svn::Path & basePathUtf8)
{
  wxString values[COL_COUNT];
  svn::Path fullPath (status.path ());

  const size_t pathUtf8Length = basePathUtf8.length () + 1;
  const bool isUrl (basePathUtf8.isUrl ());

  const bool isDot = (fullPath.length () <= pathUtf8Length);
  svn::Path path;

  if (isDot)
    values[COL_NAME] = wxT(".");
  else
  {
    path = (fullPath.substr (pathUtf8Length));
    path = path.unescape ();

    values[COL_NAME] = Utf8ToLocal (path.basename ());
  }

  if (m->ColumnVisible[COL_PATH] || m->ColumnVisible[COL_EXTENSION])
  {
    std::string dir, filename, ext;
    fullPath.split (dir, filename, ext);

    if (isDot)
      values[COL_PATH] = wxT(".");
    else
    {
      if (isUrl)
        values[COL_PATH] = Utf8ToLocal (path.c_str ());
      else
        values[COL_PATH] = Utf8ToLocal (path.native ());
    }

   values[COL_EXTENSION] = Utf8ToLocal (ext);
  }

  int i = GetItemCount ();
  int imageIndex = m->GetImageIndex (status);

  // User want to see unversioned entries?
  if (status.isVersioned() || m->ShowUnversioned)
  {
    InsertItem (i, values[COL_NAME], imageIndex);

    // The item data will be used to sort the list:
    SetItemData (i, (long)new svn::Status (status));    // The control now owns this data
    // and must delete it in due course.

    if (status.isVersioned ())
    {
      const svn::Entry & entry = status.entry ();

      values[COL_REV].Printf (wxT("%ld"), entry.revision ());
      values[COL_CMT_REV].Printf (wxT("%ld"), entry.cmtRev ());

      values[COL_AUTHOR] = Utf8ToLocal (entry.cmtAuthor ());

      // date formatting
      values[COL_CMT_DATE] = FormatDateTime (entry.cmtDate ());
      values[COL_TEXT_TIME] = FormatDateTime (entry.textTime ());
      values[COL_PROP_TIME] = FormatDateTime (entry.propTime ());

      if (status.isLocked ())
      {
        values[COL_LOCK_OWNER] = Utf8ToLocal (status.lockOwner ());
        values[COL_LOCK_COMMENT] = Utf8ToLocal (status.lockComment ());
      }

      values[COL_URL] = Utf8ToLocal (entry.url ());
      values[COL_REPOS] = Utf8ToLocal (entry.repos ());
      values[COL_UUID] = Utf8ToLocal (entry.uuid ());

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
        break;
      }
      values[COL_SCHEDULE] = schedule;

      if (entry.isCopied ())
      {
        wxString tmp (Utf8ToLocal (entry.copyfromUrl()));
        values[COL_COPIED].Printf (wxT("%s, %ld"),
          tmp.c_str (),
          entry.copyfromRev ());
      }

      values[COL_CONFLICT_OLD] = Utf8ToLocal (entry.conflictOld ());
      values[COL_CONFLICT_NEW] = Utf8ToLocal (entry.conflictNew ());
      values[COL_CONFLICT_WRK] = Utf8ToLocal (entry.conflictWrk ());
      values[COL_CHECKSUM] = Utf8ToLocal (entry.checksum ());
    }
    switch (status.textStatus ())
    {
    case svn_wc_status_none:
      break;
    case svn_wc_status_normal:
      // empty text
      if (status.reposTextStatus () == svn_wc_status_modified)
        values[COL_TEXT_STATUS] = _("outdated");
      break;
    default:
      values[COL_TEXT_STATUS] =
        StatusDescription (status.textStatus ());
      break;
    }
    switch (status.propStatus ())
    {
    case svn_wc_status_none:
      break;
    case svn_wc_status_normal:
      // empty text
      if (status.reposPropStatus () == svn_wc_status_modified)
        values[COL_PROP_STATUS] = _("outdated");
      break;
    default:
      values[COL_PROP_STATUS] =
        StatusDescription (status.propStatus ());
      break;
    }

    // set the text for all visible items
    // (ignore column 0 since this is already set with
    // InsertItem
    for (int col = 1; col < COL_COUNT; col++)
    {
      int index = m->ColumnIndex[col];
      if (index != -1)
        SetItem (i, index, values[col]);
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
    const int index = arr.Item (i);
    svn::Status * status = (svn::Status*)GetItemData (index);

    if (status == 0)
      continue;

    v.push_back (status->path ());
  }

  return svn::Targets (v);
}

unsigned int
FileListCtrl::GetSelectionActionFlags () const
{
  IndexArray arr = GetSelectedItems ();
  size_t i, counter = 0;
  unsigned int flags = 0;

  for (i = 0; i < arr.GetCount (); i++)
  {
    const int index = arr.Item (i);
    svn::Status * status = (svn::Status*)GetItemData (index);

    if (status == 0)
      continue;

    ++counter;
    if (IsDir (status))
    {
      flags |= Action::IS_DIR;
    }
    if (svn::Url::isValid (status->path ()))
    {
      flags |= Action::RESPOSITORY_TYPE;
    }
    else
    {
      if (status->isVersioned ())
      {
        flags |= Action::VERSIONED_WC_TYPE;
      }
      else
      {
        flags |= Action::UNVERSIONED_WC_TYPE;
      }
    }
  }

  if (counter)
  {
    flags |= counter == 1 ? Action::SINGLE_TARGET : Action::MULTIPLE_TARGETS;
  }

  return flags;
}

svn::Context *
FileListCtrl::GetContext () const
{
  return m->Context;
}

void
FileListCtrl::SetContext (svn::Context * Context)
{
  m->Context = Context;
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
FileListCtrl::OnDoubleClick (wxMouseEvent & event)
{
  int flag;

  // Don't post if the click didn't hit anything
  if (HitTest (ScreenToClient (wxGetMousePosition ()), flag) >= 0)
    if (!PostMenuEvent (this, ID_Default_Action))
      event.Skip ();
}

void
FileListCtrl::OnColumnLeftClick (wxListEvent & event)
{
  int clickedColumn = event.GetColumn ();

  // First we have to get the real column
  int column = m->GetRealColumn (clickedColumn);

  // A second click on the current sort column reverses the order of sorting.
  if (column == m->SortColumn)
  {
    m->SortAscending = !m->SortAscending;
  }
  else
  {
    m->SortColumn = column;
    m->SortAscending = true;
  }

  ApplySortChanges ();

  ActionEvent Event (m->Parent, TOKEN_UPDATE_ASCENDING);
  Event.Post ();

  Event.init (m->Parent, TOKEN_UPDATE_SORTING);
  Event.Post ();
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
  for (int col = 0; col < COL_COUNT; col++)
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
FileListCtrl::OnContextMenu (wxContextMenuEvent & event)
{
  wxPoint clientPt (ScreenToClient (event.GetPosition ()));
  ShowMenu (clientPt);
}

void
FileListCtrl::ShowMenu (wxPoint & pt)
{
  wxMenu menu;

  // if there is exactly one file selected, then
  // we are going to add filetype specific entries
  if (GetSelectedItemCount () == 1)
  {
    long item = GetNextItem (-1, wxLIST_NEXT_ALL,
                             wxLIST_STATE_SELECTED);
    svn::Status * status = (svn::Status*)GetItemData (item);

    AppendVerbMenu (&menu, status);
  }

  AppendModifyMenu (&menu);
  menu.AppendSeparator ();
  AppendQueryMenu (&menu);

  // Check for disabled items
  RapidSvnFrame* frame = (RapidSvnFrame*) wxGetApp ().GetTopWindow ();
  frame->TrimDisabledMenuItems (menu);

  PopupMenu (&menu);
}

inline void
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
      bool sortDown = m->SortAscending ? true : false;
      item.m_image = m->GetSortImageIndex (sortDown);
    }
    else
    {
      item.m_image = -1;
    }
    SetColumn (index, item);
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
  for (col = 0; col < COL_COUNT; col++)
  {
    if (m->ColumnVisible[col])
    {
      m->ColumnIndex[col] = index++;
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
    InsertColumn (0, wxEmptyString);
  }

  // Now set the captions and widths
  for (col = 0; col < COL_COUNT; col++)
  {
    int index = m->ColumnIndex[col];
    if (index != -1)
    {
      wxListItem item;
      item.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_WIDTH;
      item.m_text = m->ColumnList[col].caption;
      item.m_width = m->ColumnWidth[col];
      SetColumn (index, item);
    }
  }

  SetColumnImages ();

  m->DirtyColumns = false;
}

inline void
FileListCtrl::ApplySortChanges ()
{
  SetColumnImages ();
  SortItems (Data::CompareFunction, (long) this->m);
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
    case COL_LOCK_OWNER:
    case COL_LOCK_COMMENT:
    case COL_EXTENSION:
      visible = false;
      break;

    case COL_PATH:
      visible = m->FlatMode;
      break;

    default:
      visible = true;
      break;
    }
    SetColumnVisible (col, visible);
    SetColumnWidth (col, GetDefaultWidth (col));
    m->DirtyColumns = true;
  }
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
    m->SortAscending = true;
  }
  m->DirtyColumns = true;
}

bool
FileListCtrl::GetColumnVisible (const int col) const
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

inline void
FileListCtrl::SetColumnWidth (const int col, const int width)
{
  m->ColumnWidth[col] = width;

  int index = m->ColumnIndex[col];
  if (index != -1)
  {
    wxListCtrl::SetColumnWidth (index, width);
  }
}

int
FileListCtrl::GetSortColumn () const
{
  return m->SortColumn;
}

void
FileListCtrl::SetSortColumn (const int col)
{
  if (col >= 0 && col <= COL_COUNT)
    m->SortColumn = col;
}


bool
FileListCtrl::GetIncludePath () const
{
  return m->IncludePath;
}

void
FileListCtrl::SetIncludePath (bool value)
{
  m->IncludePath = value;

  ApplySortChanges ();
}

bool
FileListCtrl::GetSortAscending () const
{
  return m->SortAscending;
}

void
FileListCtrl::SetSortAscending (bool ascending)
{
  m->SortAscending = ascending;

  ApplySortChanges ();
}

bool
FileListCtrl::IsFlat () const
{
  return m->FlatMode;
}

void
FileListCtrl::SetFlat (const bool flat)
{
  m->FlatMode = flat;
  SetColumnVisible (COL_PATH, flat);
}

bool
FileListCtrl::GetWithUpdate () const
{
  return m->WithUpdate;
}

void
FileListCtrl::SetWithUpdate (bool value)
{
  m->WithUpdate = value;
}

bool
FileListCtrl::GetShowUnversioned () const
{
  return m->ShowUnversioned;
}

void
FileListCtrl::SetShowUnversioned (bool value)
{
  m->ShowUnversioned = value;
}

bool
FileListCtrl::GetIgnoreExternals () const
{
  return m->IgnoreExternals;
}

void
FileListCtrl::SetIgnoreExternals (bool value)
{
  m->IgnoreExternals = value;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
