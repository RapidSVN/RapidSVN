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
#include <vector>

// wxwindows
#include "wx/wx.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/status.hpp"

// app
#include "ids.hpp"
#include "utils.hpp"
#include "tracer.hpp"

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
 * Some wxWindows released have know bugs with menu entries
 * with bitmaps. wxWindows 2.4.1 doesnt fire events when
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
  path.Replace("\\", "/");
#endif
  return path;
}

void
GetStatusText (wxString & str, svn_wc_status_kind st)
{
  switch (st)
  {
  case svn_wc_status_none:
    str = _("Non-svn");
    break;
  case svn_wc_status_normal:
    str = _("Normal");
    break;
  case svn_wc_status_added:
    str = _("Added");
    break;
  case svn_wc_status_missing:
    str = _("Missing");
    break;
  case svn_wc_status_deleted:
    str = _("Deleted");
    break;
  case svn_wc_status_replaced:
    str = _("Replaced");
    break;
  case svn_wc_status_modified:
    str = _("Modified");
    break;
  case svn_wc_status_merged:
    str = _("Merged");
    break;
  case svn_wc_status_conflicted:
    str = _("Conflicted");
    break;
  case svn_wc_status_unversioned:
  default:
    str = _("Unversioned");
    break;
  }
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
  const char *ELLIPSIS = "...";
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
  AppendMenuItem (parentMenu, ID_Update, _("&Update..."),
                  wxBITMAP (update));
  AppendMenuItem (parentMenu, ID_Commit, _("Co&mmit..."),
                  wxBITMAP (commit));

  parentMenu->AppendSeparator ();

  AppendMenuItem (parentMenu, ID_Property, _("&Properties..."),
                  wxBITMAP (info));

  parentMenu->AppendSeparator ();

  AppendMenuItem (parentMenu, ID_Add, _("&Add"),
                  wxBITMAP (add));
  AppendMenuItem (parentMenu, ID_AddRecursive, _("Add r&ecursive"));
  AppendMenuItem (parentMenu, ID_Delete, _("&Delete"),
                  wxBITMAP (delete));

  parentMenu->AppendSeparator ();

  AppendMenuItem (parentMenu, ID_Revert, _("&Revert"),
                  wxBITMAP (revert));
  AppendMenuItem (parentMenu, ID_Resolve, _("Re&solve conflicts"),
                  wxBITMAP (resolve));

  parentMenu->AppendSeparator ();
  
  AppendMenuItem (parentMenu, ID_Copy, _("&Copy..."));
  AppendMenuItem (parentMenu, ID_Move, _("M&ove..."));
  AppendMenuItem (parentMenu, ID_Rename, _("Re&name..."));
  AppendMenuItem (parentMenu, ID_Mkdir, _("Make &directory..."));
}

void 
AppendQueryMenu (wxMenu * parentMenu)
{
  AppendMenuItem (parentMenu, ID_Log, _("&Log"),
                  wxBITMAP (log));
  AppendMenuItem (parentMenu, ID_Info, _("&Info"),
                  wxBITMAP (info));
}

void
AppendBookmarksMenu (wxMenu * parentMenu)
{
  AppendMenuItem (*parentMenu, ID_AddWcBookmark);
  AppendMenuItem (*parentMenu, ID_AddRepoBookmark);
  AppendMenuItem (*parentMenu, ID_RemoveBookmark);
}

bool
CheckRevision (const char * revstring)
{
  wxString value (revstring);
  bool ok;

  TrimString (value);
  if (value.Length () <= 0)
  {
    ok = false;
  }
  else
  {
    long revnum;
    value.ToLong (&revnum);

    ok = revnum >= 0;
  }

  return ok;
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
  const char * caption = "";
  wxBitmap bitmap;

  switch (id)
  {
  case ID_AddWcBookmark:
    caption = _("&Add Working Copy...");
    bitmap = wxBitmap (add_wc_bookmark_xpm);
    break;

  case ID_AddRepoBookmark:
    caption = _("Add &Repository...");
    break;
  
  case ID_RemoveBookmark:
    caption = _("&Remove Bookmark...");
    bitmap = wxBitmap (remove_bookmark_xpm);
    break;

  case ID_Login:
    caption =  _("Login...");
    break;

  case ID_Update:
    caption = _("Update...");
    bitmap = wxBitmap (update_xpm);
    break;

  case ID_Commit:
    caption = _("Commit...");
    bitmap = wxBitmap (commit_xpm);
    break;

  case ID_Quit:
    caption = _("E&xit");
    break;

  case ID_Refresh:
    caption = _("Refresh\tF5");
    break;

  case ID_RefreshWithUpdate:
    caption = _("Refresh (with Update)\tCTRL+F5");
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
  case ID_Explore:
    caption = _("Explore...\tF2");
  }
  wxMenuItem * item = AppendMenuItem (&menu, id, caption, bitmap);

  return item;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
