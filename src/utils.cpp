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
  case svn_wc_status_absent:
    str = _("Absent");
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

struct log_msg_baton
{
  const char *message;
  const char *base_dir;
};

void *
svn_cl__make_log_msg_baton (const char *message,
                            const char *base_dir, apr_pool_t * pool)
{
  log_msg_baton *baton = (log_msg_baton *) apr_palloc (pool, sizeof (*baton));

  baton->message = message;
  baton->base_dir = base_dir ? base_dir : ".";

  return baton;
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
  wxMenuItem * item;
  item = new wxMenuItem (parentMenu, ID_Update, _("&Update..."));
  item->SetBitmap (wxBITMAP (update));
  parentMenu->Append (item);

  item = new wxMenuItem (parentMenu, ID_Commit, _("Co&mmit..."));
  item->SetBitmap (wxBITMAP (commit));
  parentMenu->Append (item);

  parentMenu->AppendSeparator ();

  item = new wxMenuItem (parentMenu, ID_Property, _("&Properties..."));
  item->SetBitmap (wxBITMAP (info));
  parentMenu->Append (item);

  parentMenu->AppendSeparator ();

  item = new wxMenuItem (parentMenu, ID_Add, _("&Add"));
  item->SetBitmap (wxBITMAP (add));
  parentMenu->Append (item);

  item = new wxMenuItem (parentMenu, ID_Delete, _("&Delete"));
  item->SetBitmap (wxBITMAP (delete));
  parentMenu->Append (item);

  parentMenu->AppendSeparator ();

  item = new wxMenuItem (parentMenu, ID_Revert, _("&Revert"));
  item->SetBitmap (wxBITMAP (revert));
  parentMenu->Append (item);

  item = new wxMenuItem (parentMenu, ID_Resolve, _("Re&solve conflicts"));
  item->SetBitmap (wxBITMAP (resolve));
  parentMenu->Append (item);

  // Copy and rename menu
  parentMenu->AppendSeparator ();

  item = new wxMenuItem (parentMenu, ID_Copy, _("&Copy..."));
  //item->SetBitmap (wxBITMAP (copy));
  parentMenu->Append (item);

  item = new wxMenuItem (parentMenu, ID_Move, _("M&ove..."));
  //item->SetBitmap (wxBITMAP (rename));
  parentMenu->Append (item);

  item = new wxMenuItem (parentMenu, ID_Rename, _("Re&name..."));
  parentMenu->Append (item);
  //item->SetBitmap (wxBITMAP (rename));

  item = new wxMenuItem (parentMenu, ID_Mkdir, _("Make directory..."));
  
  parentMenu->Append (item);
}

void 
AppendQueryMenu (wxMenu * parentMenu)
{
  wxMenuItem * item;
  item = new wxMenuItem (parentMenu, ID_Log, _("&Log"));
  item->SetBitmap (wxBITMAP (log));
  parentMenu->Append (item);

  item = new wxMenuItem (parentMenu, ID_Info, _("&Info"));
  item->SetBitmap (wxBITMAP (info));
  parentMenu->Append (item);
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
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
