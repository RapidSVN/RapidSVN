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
// stl
#include <vector>

// wxWidgets
#include "wx/wx.h"
#include "wx/datetime.h"


// apr
#include "apr_strings.h"
#include "apr_time.h"


// svncpp
#include "svncpp/client.hpp"
#include "svncpp/status.hpp"

// app
#include "ids.hpp"
#include "utils.hpp"
#include "tracer.hpp"
#include "verblist.hpp"

// bitmaps
#include "res/bitmaps/add.xpm"
#include "res/bitmaps/commit.xpm"
#include "res/bitmaps/delete.xpm"
#include "res/bitmaps/info.xpm"
#include "res/bitmaps/log.xpm"
#include "res/bitmaps/resolve.xpm"
#include "res/bitmaps/revert.xpm"
#include "res/bitmaps/update.xpm"
#include "res/bitmaps/add_wc_bookmark.xpm"
#include "res/bitmaps/remove_bookmark.xpm"

/** configuration options */

/**
 * Some wxWidgets released have know bugs with menu entries
 * with bitmaps. wxWidgets 2.4.1 doesnt fire events when
 * using menu items with bitmaps attached.
 * Set this option to "false" if you encounter this.
 * Default: true
 */
static const bool UseBitmapMenus = false;

static wxMenuItem *
CreateMenuItem (
  wxMenu * parentMenu, int id, const wxString & text,
  const wxBitmap & bitmap)
{
  wxMenuItem * item = new wxMenuItem (parentMenu, id, text);
  if(UseBitmapMenus)
    item->SetBitmap (bitmap);
  return item;
}

static wxMenuItem *
CreateMenuItem (
  wxMenu * parentMenu, int id, const wxString & text)
{
  return new wxMenuItem (parentMenu, id, text);
}

static wxMenuItem *
AppendMenuItem (
  wxMenu * parentMenu, int id, const wxString & text,
  const wxBitmap & bitmap)
{
  wxMenuItem * item = CreateMenuItem (parentMenu, id, text, bitmap);
  parentMenu->Append (item);
  return item;
}

static wxMenuItem *
AppendMenuItem (
  wxMenu * parentMenu, int id, const wxString & text)
{
  wxMenuItem * item = CreateMenuItem (parentMenu, id, text);
  parentMenu->Append (item);
  return item;
}

wxString & UnixPath (wxString & path)
{
#ifdef _WIN32
  path.Replace(wxT("\\"), wxT("/"));
#endif
  return path;
}


void
TrimString (wxString & str)
{
  str.Trim (TRUE);
  str.Trim (FALSE);
}

bool PostMenuEvent (wxEvtHandler *source, long id)
{
  // This is the way it's done in wxFrame
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, id);

  event.SetEventObject (source);

  return source->ProcessEvent (event);
}

wxButton *
CreateEllipsisButton(wxWindow *parent, long id)
{
  const wxChar *ELLIPSIS = wxT("...");
  int ellipsis_width, ellipsis_height;

  wxButton *button = new wxButton (parent, id, ELLIPSIS);
  parent->GetTextExtent(ELLIPSIS, &ellipsis_width, &ellipsis_height);

  // HACK: Should get real button border size from somewhere
  button->SetSize(wxSize(ellipsis_width + 2 * 3 + 2 * 2, -1));

  return button;
}

void
AppendModifyMenu (wxMenu * parentMenu)
{
  AppendMenuItem (*parentMenu, ID_Edit);

  parentMenu->AppendSeparator ();
  AppendMenuItem (*parentMenu, ID_Update);
  AppendMenuItem (*parentMenu, ID_Commit);

  parentMenu->AppendSeparator ();

  AppendMenuItem (parentMenu, ID_Property, _("&Properties...\tCTRL-P"),
                  wxBitmap (info_xpm));

  parentMenu->AppendSeparator ();

  AppendMenuItem (parentMenu, ID_Add, _("&Add\tCTRL-A"),
                  wxBitmap (add_xpm));
  AppendMenuItem (parentMenu, ID_AddRecursive, _("Add r&ecursive"));
  AppendMenuItem (parentMenu, ID_Delete, _("&Delete\tDEL"),
                  wxBitmap (delete_xpm));

  parentMenu->AppendSeparator ();

  AppendMenuItem (parentMenu, ID_Revert, _("Re&vert\tCTRL-V"),
                  wxBitmap (revert_xpm));
  AppendMenuItem (parentMenu, ID_Resolve, _("Re&solve conflicts\tCTRL-S"),
                  wxBitmap (resolve_xpm));

  parentMenu->AppendSeparator ();

  AppendMenuItem (parentMenu, ID_Copy, _("&Copy...\tF5"));
  AppendMenuItem (parentMenu, ID_Move, _("M&ove...\tF6"));
  AppendMenuItem (parentMenu, ID_Rename, _("Re&name...\tCTRL-N"));
  AppendMenuItem (parentMenu, ID_Mkdir, _("Make &directory...\tF7"));
}

void
AppendQueryMenu (wxMenu * parentMenu)
{
  AppendMenuItem (parentMenu, ID_Diff, _("&Diff...\tCTRL+D"));
  AppendMenuItem (parentMenu, ID_DiffBase, _("&Diff to Base...\tCTRL+B"));
  AppendMenuItem (parentMenu, ID_DiffHead, _("&Diff to Head...\tCTRL+H"));
  parentMenu->AppendSeparator ();
  AppendMenuItem (parentMenu, ID_Log, _("&Log...\tCTRL-L"),
                  wxBitmap (log_xpm));
  AppendMenuItem (parentMenu, ID_Info, _("&Info..."),
                  wxBitmap (info_xpm));
}

void
AppendBookmarksMenu (wxMenu * parentMenu)
{
  AppendMenuItem (*parentMenu, ID_AddWcBookmark);
  AppendMenuItem (*parentMenu, ID_AddRepoBookmark);
  AppendMenuItem (*parentMenu, ID_RemoveBookmark);
}

bool
CheckRevision (const wxString & revstring)
{
  svn_revnum_t revnum;

  return ParseRevision (revstring, revnum);
}

wxCommandEvent
CreateActionEvent (int token)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, ACTION_EVENT);
  event.SetInt (token);

  return event;
}

wxMenuItem *
AppendMenuItem (wxMenu & menu, int id)
{
  wxString caption;
  wxBitmap bitmap;

  switch (id)
  {
  case ID_Edit:
    caption = _("&Edit...\tF3");
    break;

  case ID_AddWcBookmark:
    caption = _("&Add Existing Working Copy...");
    bitmap = wxBitmap (add_wc_bookmark_xpm);
    break;

  case ID_AddRepoBookmark:
    caption = _("Add Existing &Repository...");
    break;

  case ID_RemoveBookmark:
    caption = _("&Remove Bookmark...");
    bitmap = wxBitmap (remove_bookmark_xpm);
    break;

  case ID_CreateRepository:
    caption = _("Create New Repository...");
    break;

  case ID_Checkout:
    caption = _("Checkout New Working Copy...");
    break;

  case ID_Login:
    caption =  _("Login...");
    break;

  case ID_Update:
    caption = _("&Update...\tCTRL-U");
    bitmap = wxBitmap (update_xpm);
    break;

  case ID_Commit:
    caption = _("Co&mmit...\tCTRL-M");
    bitmap = wxBitmap (commit_xpm);
    break;

  case ID_Quit:
    caption = _("E&xit\tCTRL-Q");
    break;

  case ID_Refresh:
    caption = _("Refresh\tCTRL-R");
    break;

  case ID_Preferences:
    caption = _("Preferences...");
    break;

  case ID_Cleanup:
    caption = _("Cleanup");
    break;

  case ID_Column_Reset:
    caption = _("Reset Columns");
    break;

  case ID_Stop:
    caption = _("&Stop");
    break;

  case ID_Explore:
    caption = _("Explore...\tF2");
  }
  wxMenuItem * item = AppendMenuItem (&menu, id, caption, bitmap);

  return item;
}


bool
ParseRevision (const wxString & revstring, svn_revnum_t & revnum)
{
  wxString value (revstring);

  TrimString (value);
  if (value.Length () <= 0)
    return false;

  return value.ToLong (&revnum);
}


bool
ParseDateTime (const wxString & datestring, apr_time_t & date)
{
  wxString value (datestring);

  TrimString (value);

  if (value.Length () <= 0)
    return false;

  // parse the string using the current locale setting
  // (we check only if the complete parsing failed,
  //  not if only the partial string could be parsed)
  wxDateTime dateTime;
  if (dateTime.ParseFormat (datestring, wxT("%c")) ==0)
    return false;

  apr_time_ansi_put (&date, dateTime.GetTicks ());
  return true;
}


bool
CheckDateTime (const wxString & datestring)
{
  apr_time_t time;

  return ParseDateTime (datestring, time);
}


wxString
FormatDateTime (apr_time_t date, wxString fmt)
{
  wxString wxstrtime;

  if (date == 0)
    return wxstrtime;

  apr_time_exp_t exp_time;
  char timestr[80];

  apr_time_exp_lt (&exp_time, date);
  apr_size_t size;
  apr_status_t apr_err =
    apr_strftime (timestr, &size, sizeof (timestr),
                  "%x %X", &exp_time);

  /* if that failed, just leave the string empty */
  if (!apr_err)
    wxstrtime = wxString::FromAscii(timestr)  ;

  return wxstrtime;
}


wxString
BeautifyPath (const wxString & path)
{
  int pos = path.Find (wxT(":"));

  if (pos <= 0)
    return path;

  // ok. we do have a dot. So is it an url or
  // a windows drive?
  wxString start (path.Left (pos));

  // lowercase the Drive / URL schema part
  return start.Lower () + path.Mid (pos);
}


wxString
StatusDescription (const svn_wc_status_kind kind)
{
  switch (kind)
  {
  case svn_wc_status_none:
    return _("none");
    break;
  case svn_wc_status_normal:
    return _("normal");
    break;
  case svn_wc_status_added:
    return _("added");
    break;
  case svn_wc_status_missing:
    return _("missing");
    break;
  case svn_wc_status_deleted:
    return _("deleted");
    break;
  case svn_wc_status_replaced:
    return _("replaced");
    break;
  case svn_wc_status_modified:
    return _("modified");
    break;
  case svn_wc_status_merged:
    return _("merged");
    break;
  case svn_wc_status_conflicted:
    return _("conflicted");
    break;
  case svn_wc_status_unversioned:
  default:
    return _("unversioned");
    break;
  }
}

#if !wxUSE_UNICODE
static wxMBConv *
GetWXLocalConv ()
{
#if defined(__WXGTK20__)
  // In wxGTK 2.0+, for some reason wvConvCurrent
  // is always set to wxConvUTF8 - which is not correct!
  // We will instead echo some logic in wxEntryStart
  // to choose between wxConvLibc & wxConvLocal.
  static wxMBConv *wxConvCorrect = NULL;

  if (wxConvCorrect == NULL)
  {
    if (wxOKlibc ())
    {
      wxConvCorrect = &wxConvLibc;
    }
    else
    {
      wxConvCorrect = &wxConvLocal;
    }
  }

  return wxConvCorrect;
#else
  // Use what wxWidgets has already selected to be the correct conversion
  return wxConvCurrent;
#endif
}
#endif

wxString
Utf8ToLocal (const char* srcUtf8)
{
#if wxUSE_UNICODE
  wxString dst (srcUtf8, wxConvUTF8);
#else
  wxString dst (wxConvUTF8.cMB2WC(srcUtf8), *GetWXLocalConv ());
#endif

  return dst;
}

wxString
Utf8ToLocal (const std::string& srcUtf8)
{
  return Utf8ToLocal(srcUtf8.c_str());
}

std::string
LocalToUtf8 (const wxString & srcLocal)
{
#if wxUSE_UNICODE
  std::string dst (srcLocal.mb_str (wxConvUTF8));
#else
  wxString wxdst (srcLocal.wc_str (*GetWXLocalConv ()), wxConvUTF8);
  std::string dst (wxdst.mb_str ());
#endif

  return dst;
}

void
LocalToUtf8 (const wxString & srcLocal, std::string & dstUtf8)
{
#if wxUSE_UNICODE
  dstUtf8 = srcLocal.mb_str (wxConvUTF8);
#else
  wxString wxdst (srcLocal.wc_str (*GetWXLocalConv ()), wxConvUTF8);
  dstUtf8 = wxdst.mb_str ();
#endif
}

void
AppendVerbMenu (wxMenu * parentMenu, svn::Status * status)
{
    wxASSERT (status);

    // Append file verbs
    try
    {
      VerbList verbList;

      // We don't want verbs on directories, even though they exist
      bool isADirectory = status->entry ().kind () == svn_node_dir;
      if (!isADirectory)
        verbList.InitFromDocument (Utf8ToLocal (status->path ()), isADirectory );

      if (verbList.GetCount () == 0)
        return;

      wxMenu * menu = new wxMenu ();

      size_t i = 0;
      for (;
           (i < verbList.GetCount ()) &&
           (i < (ID_Verb_Max - ID_Verb_Min + 1)); i++)
      {
        wxMenuItem *pItem;
        // TODO: Convert verb names to unicode on the fly if needed (or make
        // verblist follow wxWidgets' unicode setting)
        // TODO: Note: I think verbList.GetName is a Windows code-paged char *, not UTF 8, so don't use Utf8ToLocal like elsewhere
        // Someone will need to put the correct mappings in eventually
        pItem = new wxMenuItem (menu, ID_Verb_Min + i, wxString::FromAscii (verbList.GetName (i)));
        //pItem->SetBitmap (wxBITMAP (?))
        menu->Append (pItem);
      }

      parentMenu->Append (ID_Open, _("Open..."), menu);
    }
    catch (std::exception)
    {
      // Failed assembling verbs.
      // TODO: Report this error in the status bar?

    }

}



/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
