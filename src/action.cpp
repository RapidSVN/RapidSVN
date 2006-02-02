/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
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

// wxWidgets
#include "wx/wx.h"
#include "wx/utils.h"

// svncpp
#include "svncpp/client.hpp"
// #include "svncpp/m_check.hpp"

// app
#include "action.hpp"
#include "config.hpp"
#include "tracer.hpp"
#include "rapidsvn_app.hpp"
#include "utils.hpp"

// subversion api
#include "svn_wc.h"

//TODO: This isn't actually used, and an exact copy lives in listener.cpp which is used
// I suspect we should get rid of one, but I'm not sure which at the  moment...
// static const wxChar *
// ACTION_NAMES [] =
// {
//   _("Add"),              // svn_wc_notify_add,
//   _("Copy"),             // svn_wc_notify_copy,
//   _("Delete"),           // svn_wc_notify_delete,
//   _("Restore"),          // svn_wc_notify_restore,
//   _("Revert"),           // svn_wc_notify_revert,
//   NULL ,                 // NOT USED HERE svn_wc_notify_failed_revert,
//   _("Resolved"),         // svn_wc_notify_resolved,
//   _("Skip"),             // NOT USED HERE svn_wc_notify_skip,
//   _("Deleted"),          // svn_wc_notify_update_delete,
//   _("Added"),            // svn_wc_notify_update_add,
//   _("Updated"),          // svn_wc_notify_update_update,
//   NULL,                  // NOT USED HERE svn_wc_notify_update_completed,
//   NULL,                  // NOT USED HERE svn_wc_notify_update_external,
//   NULL,                  // NOT USED HERE svn_wc_notify_status_completed
//   NULL,                  // NOT USED HERE svn_wc_notify_status_external#include "svncpp/m_check.hpp"

//   _("Modified"),         // svn_wc_notify_commit_modified,
//   _("Added"),            // svn_wc_notify_commit_added,
//   _("Deleted"),          // svn_wc_notify_commit_deleted,
//   _("Replaced"),         // svn_wc_notify_commit_replaced,
//   NULL,                  // NOT USED HERE svn_wc_notify_commit_postfix_txdelta
//   NULL,                  // NOT USED HERE svn_wc_notify_blame_revision
//   _("Locked"),           // svn_wc_notify_locked
//   _("Unlocked"),         // svn_wc_notify_unlocked
//   _("Failed to lock"),   // svn_wc_notify_failed_lock
//   _("Failed to unlock")  // svn_wc_notify_failed_unlock
// };

// #if CHECK_SVN_SUPPORTS_LOCK
// const int MAX_ACTION = svn_wc_notify_failed_unlock;
// #else
// const int MAX_ACTION = svn_wc_notify_commit_postfix_txdelta;
// #endif

#ifdef USE_SIMPLE_WORKER
void
SafeYield ()
{
  wxSafeYield ();
}
#else
void
SafeYield () {};
#endif


struct Action::Data
{
public:
  /**
   * The parent
   */
  wxWindow * parent;

  /** the name of the action */
  wxString name;

  /** flags for the action */
  unsigned int flags;

  /**
   * This member variable will take the address
   * of a trace object allocated in a class
   * derived from ActionThread. It will be used
   * from the svn_delta_editor callbacks.
   */
  Tracer * tracer;

  /**
   * If ownTracer is TRUE, then the ActionThread class
   * is responsible for deleting the tracer.
   */
  bool ownTracer;

  /**
   * the context under which the action will be
   * performed. this includes auth info and callback
   * addresses
   */
  svn::Context * context;

  /**
   * the path where the action will take place
   */
  svn::Path path;


  svn::Targets targets;

  Data (wxWindow * parnt, const wxString & nam, unsigned int flgs)
    :  parent (parnt), name (nam), flags (flgs),
       tracer (0), ownTracer (false)
  {
  }

  virtual ~Data ()
  {
    if (tracer && ownTracer)
    {
      delete tracer;
    }
  }

};


const unsigned int Action::DONT_UPDATE          = 0x0001;
const unsigned int Action::UPDATE_LATER         = 0x0002;
const unsigned int Action::WITHOUT_TARGET       = 0x0004;
const unsigned int Action::SINGLE_TARGET        = 0x0008;
const unsigned int Action::MULTIPLE_TARGETS     = 0x0010;
const unsigned int Action::RESPOSITORY_TYPE     = 0x0020;
const unsigned int Action::VERSIONED_WC_TYPE    = 0x0040;
const unsigned int Action::UNVERSIONED_WC_TYPE  = 0x0080;
const unsigned int Action::IS_DIR               = 0x0100;
const unsigned int Action::UPDATE_TREE          = 0x0200;

// combined flags
const unsigned int Action::TARGET_QUANTITY_MASK = SINGLE_TARGET|MULTIPLE_TARGETS;
const unsigned int Action::TARGET_TYPE_MASK     = RESPOSITORY_TYPE|VERSIONED_WC_TYPE|UNVERSIONED_WC_TYPE;


Action::Action (wxWindow * parent, const wxString & name, unsigned int flgs)
{
  m = new Data (parent, name, flgs);
}

Action::~Action ()
{
  delete m;
}

void
Action::PostStringEvent (int code, wxString str, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetString (str);

  // send in a thread-safe way
  wxPostEvent (m->parent, event);

  SafeYield ();
}

void
Action::PostDataEvent (int code, void *data, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetClientData (data);

  // send in a thread-safe way
  wxPostEvent (m->parent, event);

  SafeYield ();
}

void
Action::SetTracer (Tracer * t, bool own)
{
  m->tracer = t;
  m->ownTracer = own;
}

Tracer *
Action::GetTracer ()
{
  return m->tracer;
}

wxWindow *
Action::GetParent ()
{
  return m->parent;
}

void
Action::Trace (const wxString & msg)
{
  if (m->tracer)
  {
    m->tracer->Trace (msg);
  }

  SafeYield ();
}

void
Action::TraceError (const wxString & msg)
{
  Trace (msg);
}

void
Action::SetParent (wxWindow * parent)
{
  m->parent = parent;
}

void
Action::SetPath (const svn::Path & path)
{
  m->path = path;
}

const svn::Path &
Action::GetPath ()
{
  return m->path;
}

void
Action::SetContext (svn::Context * context)
{
  m->context = context;
}

svn::Context *
Action::GetContext ()
{
  return m->context;
}

void
Action::SetTargets (const svn::Targets & targets)
{
  m->targets = targets;
}

const svn::Targets &
Action::GetTargets ()
{
  return m->targets;
}

bool
Action::Prepare ()
{
  bool result = true;

  if ((GetFlags () & Action::WITHOUT_TARGET) == 0)
  {
    unsigned int quantityFlags = GetFlags() & Action::TARGET_QUANTITY_MASK;
    if (quantityFlags == Action::SINGLE_TARGET)
    {
      if (m->targets.size () != 1)
      {
        wxMessageBox (_("Please select only one file or directory."),
                      _("Error"),
                      wxOK | wxICON_HAND);
        result = false;
      }
    }
    else
    {
      if (m->targets.size () < 1)
      {
        wxMessageBox (_("Nothing selected."),
                      _("Error"),
                      wxOK | wxICON_HAND);
        result = false;
      }
    }
  }

  if (result)
  {
    wxSetWorkingDirectory (Utf8ToLocal(m->path.c_str ()));
  }

  return result;
}

const svn::Path
Action::GetTarget ()
{
  return m->targets.target ();
}

const wxString &
Action::GetName () const
{
  return m->name;
}

void
Action::SetName (const wxString & name)
{
  m->name = name;
}

void
Action::PostEvent (wxEvent & event)
{
  wxEvtHandler * handler;

  handler = GetParent ();

  if (handler == 0)
    return;

  wxPostEvent (handler, event);
}


unsigned int
Action::GetFlags () const
{
  return m->flags;
}


void
Action::SetFlags (unsigned int flags)
{
  m->flags = flags;
}

svn::Path
Action::GetPathAsTempFile (const svn::Path & path,
           const svn::Revision & revision)
{
  svn::Client client (GetContext ());

  wxString revStr;
  if (revision.kind () == revision.HEAD)
    revStr = _("HEAD");
  else if (revision.kind() == revision.BASE)
    revStr = _("BASE");
  else
    revStr.Printf (wxT("%") wxT(SVN_REVNUM_T_FMT), revision.revnum ());

  wxString msg, wxpath (Utf8ToLocal (path.c_str ()));
  msg.Printf (_("Get file %s rev. %s"),
              wxpath.c_str (), revStr.c_str ());
  Trace (msg);

  svn::Path dstPath ("");
  client.get (dstPath, path, revision);

  // Remember this temporary file so we can delete it when the application exits
  ::wxGetApp ().OptionallyRegisterTempFile (
      Utf8ToLocal (dstPath.c_str ()));

  return dstPath;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
