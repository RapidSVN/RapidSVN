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

// app
#include "action.hpp"
#include "tracer.hpp"
#include "auth_dlg.hpp"

struct Action::Data
{
public:
  /**
   * The parent
   */
  wxWindow * parent;
    
  /** the name of the action */
  wxString name;

  /**
   * Options for the action. 
   */
  ActionOptions options;

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

  Data (wxWindow * parnt, const wxString & nam, ActionOptions opts)
    :  parent (parnt), name (nam), options (opts), 
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

Action::Action (wxWindow * parent, const wxString & name, ActionOptions options)
{
  m = new Data (parent, name, options);
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

  wxSafeYield ();
}

void
Action::PostDataEvent (int code, void *data, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetClientData (data);

  // send in a thread-safe way
  wxPostEvent (m->parent, event);

  wxSafeYield ();
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

  wxSafeYield ();
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
  switch (m->options)
  {
  case actionWithoutTarget:
    break;

  case actionWithSingleTarget:
    if (m->targets.size () != 1)
    {
      wxMessageBox (_("Please select only one file or directory."),
                    _("Error"),
                    wxOK | wxICON_HAND);
      result = false;
    }
    break;

  case actionWithTargets:
    if (m->targets.size () < 1)
    {
      wxMessageBox (_("Nothing selected."),
                    _("Error"),
                    wxOK | wxICON_HAND);
      result = false;
    }
    break;
    
  default:
    // unknown option
    wxMessageBox (_("Internal error: unknown action option"),
                  _("Error"),
                  wxOK | wxICON_HAND);
    result = false;
  }

  if (result)
  {
    wxSetWorkingDirectory (m->path.c_str ());
  }

  return result;
}

const svn::Path 
Action::GetTarget ()
{
  return m->targets.target ();
}

ActionOptions
Action::GetOptions ()
{
  return m->options;
}

bool 
Action::contextGetLogin (std::string & username, 
                 std::string & password)
{
  AuthDlg dlg (GetParent (), username.c_str () , password.c_str ());

  bool ok = dlg.ShowModal () == wxID_OK;
  if (ok)
  {
    username = dlg.GetUsername ();
    password = dlg.GetPassword ();
  }

  return ok;
}

void
Action::contextNotify (const char *path,
        svn_wc_notify_action_t action,
        svn_node_kind_t kind,
        const char *mime_type,
        svn_wc_notify_state_t content_state,
        svn_wc_notify_state_t prop_state,
        svn_revnum_t revision)
{
  // Implement code to generate usefule messages that can be 
  // transmitted with "Trace"
  wxSafeYield ();
}

bool
Action::contextGetLogMessage (std::string & msg)
{
  // Implement code to ask for a log message
  return false;
}

const char * 
Action::GetName () const
{
  return m->name.c_str ();
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
