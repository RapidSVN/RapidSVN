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

static const char *
ACTION_NAMES [] =
{
  _("Add......."),           // svn_wc_notify_add,
  _("Copy......"),          // svn_wc_notify_copy,
  _("Delete...."),        // svn_wc_notify_delete,
  _("Restore..."),       // svn_wc_notify_restore,
  _("Revert...."),        // svn_wc_notify_revert,
  NULL ,              // NOT USED HERE svn_wc_notify_failed_revert,
  _("Resolve..."),       // svn_wc_notify_resolve,
  NULL,               // NOT USED HERE svn_wc_notify_status,
  _("Skip......"),          // NOT USED HERE svn_wc_notify_skip,
  _("Deleted..."),       // svn_wc_notify_update_delete,
  _("Added....."),         // svn_wc_notify_update_add,
  _("Updated..."),       // svn_wc_notify_update_update,
  NULL,               // NOT USED HERE svn_wc_notify_update_completed,
  NULL,               // NOT USED HERE svn_wc_notify_update_external,
  _("Modified.."),      // svn_wc_notify_commit_modified,
  _("Added....."),         // svn_wc_notify_commit_added,
  _("Deleted..."),       // svn_wc_notify_commit_deleted,
  _("Replaced.."),      // svn_wc_notify_commit_replaced,
  NULL                // NOT USED HERE svn_wc_notify_commit_postfix_txdelta
};
const int MAX_ACTION = svn_wc_notify_commit_postfix_txdelta;


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
    // WORKAROUND: CONVERT TO UTF8
    wxString usernameNative (dlg.GetUsername ());
    wxString passwordNative (dlg.GetPassword ());
    wxString usernameUtf8 (usernameNative.mb_str (wxConvUTF8));
    wxString passwordUtf8 (passwordNative.mb_str (wxConvUTF8));
    username = usernameUtf8.c_str ();
    password = passwordUtf8.c_str ();
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
  // Map an action to string and trace the action and path
  const char * actionString = 0;

  if (action >= 0 && action <= MAX_ACTION)
    actionString = ACTION_NAMES [action];

  if (actionString != 0)
  {
    wxString msg;
    msg.Printf ("%s: %s", actionString, path);

    Trace (msg);
  }

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

void
Action::SetName (const char * name)
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

bool
Action::contextAskQuestion (const std::string & question,
                            std::string & answer,
                            bool hide)
{
  wxString myAnswer;
  if (hide)
  {
    myAnswer = wxGetPasswordFromUser (
      question.c_str (), _("Question"), "", GetParent ());
  }
  else
  {
    myAnswer = wxGetTextFromUser (
      question.c_str (), _("Question"), "", GetParent ());
  }

  // WORKAROUND: CONVERT TO UTF8
  //answer = myAnswer.c_str ();
  wxString answerUtf8 (myAnswer.mb_str (wxConvUTF8));
  answer = answerUtf8.c_str ();
  return myAnswer.Length () > 0;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
