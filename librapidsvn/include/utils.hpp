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
 * along with this program (in the file GPL.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _RAPIDSVN_UTILS_H_INCLUDED_
#define _RAPIDSVN_UTILS_H_INCLUDED_

// stl
#include "svncpp/string_wrapper.hpp"

// wx
#include "wx/dynarray.h"

// svn
#include "svn_wc.h"

// forward declarations
class wxString;
class Tracer;
class wxMenu;
class wxCommandEvent;
class wxFileName;
class wxCheckBox;
class wxTextCtrl;

namespace svn
{
  class Status;
  class Path;
  class Context;
}

/** Type of log items; also used for trace events */
enum LogItemType
{
  LogItem_Normal,
  LogItem_Error,
  LogItem_Added,
  LogItem_Deleted,
  LogItem_Conflicted,
  LogItem_Updated,
  LogItemTypeCount
};

/**
 * declaration of an array with @a svn_revnum_t elements
 */
#ifdef WX_DEFINE_ARRAY_LONG
WX_DEFINE_ARRAY_LONG(svn_revnum_t, RevnumArray);
#else
WX_DEFINE_ARRAY(svn_revnum_t, RevnumArray);
#endif


/**
 * Create pseudo-unix paths on windows for use with svn (reverse backslashes,
 * but leave the rest as it is)
 *
 * NOTE: Does nothing on non-windows platforms
 *
 * @param path Native path to convert
 * @return string with all backslashes converted to forward slashes
 */
wxString &
UnixPath(wxString & path);

/**
 * Trim whitespace at start and end of string
 * (Convenience function for trimming first left, then right)
 */
void
TrimString(wxString & str);

/**
 * Post a menu command event with the given ID.
 *
 * Used for converting non-command events to command events so they'll move up
 * the GUI hierarchy.
 */
bool
PostMenuEvent(wxEvtHandler *source, long id);

/**
 * Create a correctly sized button with an ellipsis (three dots)
 */
wxButton *
CreateEllipsisButton(wxWindow *parent, long id);

/**
 * Append entries for the "Modify" menu
 *
 * @param parentMenu menu that will receive the items
 */
void
AppendModifyMenu(wxMenu * parentMenu);

/**
 * Append entries for the "Repository" menu
 *
 * @param parentMenu menu that will receive the items
 */
void
AppendBookmarksMenu(wxMenu * parentMenu);

/**
 * Append entries for the "Query" menu
 *
 * @param parentMenu menu that will receive the items
 * @param modified Whether the item(s) to query are modified
 */
void
AppendQueryMenu(wxMenu * parentMenu, bool modified);

/**
 * Append entries for the "Query" menu for log item.
 *
 * @param parentMenu menu that will receive the items
 */
void
AppendLogItemQueryMenu(wxMenu *parentMenu);

/**
 * Append entries for "verbs" (Win32 only).
 * Since this feature is context specific (based on
 * the selected file), @a status has to be passed.
 * Works only with one selected file
 *
 * @param parentMenu menu that will receive the items
 * @param status status instance of the selected item
 */
void
AppendVerbMenu(wxMenu * parentMenu, svn::Status * status);


/**
 * Checks if the given string contains a valid
 * revision number
 *
 * @param revstring revision string
 * @return true if valid revision
 */
bool
CheckRevision(const wxString & revstring);


/**
 * Creates a menu item for @a id (with whistles and bells, uh,
 * bitmaps)
 *
 * @param menu menu where the new item will be appended
 * @param id menu item to create
 */
wxMenuItem *
AppendMenuItem(wxMenu & menu, int id);


/**
 * Tries to interpret @a revstring a revision number
 *
 * @param revstring string with revision number
 * @param revnum revision number
 * @return success?
 * @retval true valid revision number
 */
bool
ParseRevision(const wxString & revstring, svn_revnum_t & revnum);


/**
 * Format a date/time value to standard format
 *
 * @param data
 * @return formatted date/time string
 */
wxString
FormatDateTime(apr_time_t date);


/**
 * beautify a path so drive letters (if existing) are uppercase
 *
 * Examples:
 * 1. Local Unix path (leave alone)
 *    Before:  /home/users/xela/work/rapidsvn/src/svncpp
 *    After:   /home/users/xela/work/rapidsvn/src/svncpp
 *
 * 2. Local Windows path (uppercase drive letter)
 *    Before:  d:\Documents and Settings\alex\Application Data
 *    After:   D:\Documents and Settings\alex\Application Data
 *
 * 3. Repository URL (lowercase url schema)
 *    Before:  https://svn.collab.net/repos/rapidsvn/trunk/src/svncpp
 *    After:   https://svn.collab.net/repos/rapidsvn/trunk/src/svncpp
 *
 * Jobs to do:
 * - Uppercase Windows drive letters
 * - Lowecase url schemas
 *
 * @param path input path
 * @return beatified path
 */
wxString
BeautifyPath(const wxString & path);


/**
 * creates a textual description for a status value
 *
 * @param kind
 * @return brief textual description
 */
wxString
StatusDescription(const svn_wc_status_kind kind);


/**
 * converts a string from local encoding (like Ansi on
 * Windows) to utf8
 *
 * @see Utf8ToLocal
 *
 * @param srcLocal string in local encoding
 * @return string in utf8 encoding
 */
std::string
LocalToUtf8(const wxString & srcLocal);

/**
 * converts a string from local encoding (like Ansi on
 * Windows) to utf8
 *
 * @see Utf8ToLocal
 *
 * @param srcLocal string in local encoding
 * @param dstUtf8 string in utf8 encoding
 */
void
LocalToUtf8(const wxString & srcLocal, std::string & dstUtf8);

/**
 * converts a string from local encoding to a utf8 path.
 * Urls get escaped
 *
 * @param path path string in local encoding
 * @return path in utf8
 */
svn::Path
PathUtf8(const wxString & path);


/**
 * converts an utf8 path to a string in local encoding
 * with native path separators
 *
 * @param path source
 * @param path string in local encoding
 */
wxString
PathToNative(const svn::Path & path);

/**
 * converts a string from utf to the local encoding
 * (like Ansi on Windows)
 *
 * @see LocalToUtf8
 *
 * @param srcUtf8 string in utf8 encoding
 * @return string in local encoding
 */
wxString
Utf8ToLocal(const char* srcUtf8);

/**
 * converts a string from utf to the local encoding
 * (like Ansi on Windows)
 *
 * @see LocalToUtf8
 *
 * @param srcUtf8 string in utf8 encoding
 * @return string in local encoding
 */
wxString
Utf8ToLocal(const std::string & srcUtf8);

/**
 * Convert a character array to a wxBitmap.
 * You have to ensure the contents of the array string
 * contains a valid image
 *
 * @remark The suggestion to this code is from
 *         http://www.wxwidgets.org/index.php/Embedding_PNG_Images
 *
 * @see EMBEDDED_BITMAP
 *
 * @para data array with the bitmap data
 * @para len number of elements in the array
 * @return the bitmap
 */
wxBitmap
EmbeddedBitmap(const unsigned char * data, size_t len);

/**
 * Macro that saves you from having to
 * add the @a len parameter to @ref EmbeddedBitmap
 */
#define EMBEDDED_BITMAP(data) EmbeddedBitmap (data, sizeof (data))


/**
 * Opens a webpage in the default Browser.
 *
 * @param url URL to open
 */
void
OpenURL(const wxString & url);

/**
 * Converts svn::Path to native full path expression
 * (e.g. C:\foo\bar) using view mode and current folder
 * selection status.
 *
 * [Path Type / Flat Mode]
 * URL /  -  : Outputs URL always.
 * WS  / No  : Input path may be relative (if reflesh with update
 *             checed), but all files must be in FolderBrowser
 *             selected path.
 * WS  / Yes : FolderBrowser selection is not reliable because
 *             various pathes are shown in file list. But input
 *             path is always full path in this mode.
 *
 * @param target svn::Path formatted path object
 * @param base path of current selection status in FolderBrowser
 * @param flat true if flat mode is selected
 *
 * @return native file path (if in WS) or URL (if in repository)
 */
wxString
FullNativePath(const svn::Path & target, const wxString & base, bool flat);

/**
 * Compares filenames.
 * The comparison is performed according to the platform's rules,
 * i.e. it is case insensitive on Windows, but case sensitive everywhere
 * else.
 */
bool
PathIsSame(const wxString& p1, const wxString& p2);

/**
  * Checks that folder on path contents a modified child/children.
  *
  * @param path path to folder
  * @param context pointer to current context
  *
  * @return true if folder contents modified children,
  *         otherwise false
  */
bool
HasModifiedChildren(const svn::Path & path, svn::Context * context);

/**
 * Checks whether @a executable can be found path and returns the
 * absolute path
 */
wxString
FindExecutableInPath(const wxString & executable);


/**
 * This works almost like @ref ::wxDirExists, but in extension of
 * @ref wxDirExists this call expands ~ (home dir) and environment
 * variable before checking the existence
 *
 * @param dir
 */
bool
IsValidDir(const wxString & dir);


/**
 * Helper function that enables/disables a control only if
 * the enable status differs.
 * This avoids flickering on some platforms.
 *
 * @param ctrl
 * @param enable
 */
void
EnableCtrl(wxWindow * ctrl, bool enable);


/** Updates the value of a checkbox flicker-free */
void
UpdateValue(wxCheckBox * ctrl, bool value);

void
UpdateValue(wxTextCtrl * ctrl, const wxString & value);

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
