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
#include "listener.hpp"
#include "cert_dlg.hpp"
#include "tracer.hpp"
#include "auth_dlg.hpp"

static const char *
ACTION_NAMES [] =
{
  _("Add"),           // svn_wc_notify_add,
  _("Copy"),          // svn_wc_notify_copy,
  _("Delete"),        // svn_wc_notify_delete,
  _("Restore"),       // svn_wc_notify_restore,
  _("Revert"),        // svn_wc_notify_revert,
  NULL ,              // NOT USED HERE svn_wc_notify_failed_revert,
  _("Resolved"),      // svn_wc_notify_resolved,
  _("Skip"),          // NOT USED HERE svn_wc_notify_skip,
  _("Deleted"),       // svn_wc_notify_update_delete,
  _("Added"),         // svn_wc_notify_update_add,
  _("Updated"),       // svn_wc_notify_update_update,
  NULL,               // NOT USED HERE svn_wc_notify_update_completed,
  NULL,               // NOT USED HERE svn_wc_notify_update_external,
  NULL,               // NOT USED HERE svn_wc_notify_status_completed
  NULL,               // NOT USED HERE svn_wc_notify_status_external
  _("Modified"),      // svn_wc_notify_commit_modified,
  _("Added"),         // svn_wc_notify_commit_added,
  _("Deleted"),       // svn_wc_notify_commit_deleted,
  _("Replaced"),      // svn_wc_notify_commit_replaced,
  NULL                // NOT USED HERE svn_wc_notify_commit_postfix_txdelta
};
const int MAX_ACTION = svn_wc_notify_commit_postfix_txdelta;


struct Listener::Data
{
public:
  /**
   * The parent
   */
  wxWindow * parent;
    

  /**
   * This member variable will take the address 
   * of a trace object allocated in a class 
   * derived from ActionThread. It will be used
   * from the svn_delta_editor callbacks.
   */
  Tracer * tracer;
  
  
  /**
   * If ownTracer is TRUE, then this class
   * is responsible for deleting the tracer.
   */
  bool ownTracer;


  /**
   * the context under which the action will be
   * performed. this includes auth info and callback
   * addresses
   */
  svn::Context * context;

  svn::Targets targets;

  Data (wxWindow * parnt)
    :  parent (parnt), tracer (0), ownTracer (false)
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

Listener::Listener (wxWindow * parent)
{
  m = new Data (parent);
}

Listener::~Listener ()
{
  delete m;
}


void 
Listener::SetTracer (Tracer * t, bool own)
{
  m->tracer = t;
  m->ownTracer = own;
}

Tracer *
Listener::GetTracer ()
{
  return m->tracer;
}

wxWindow *
Listener::GetParent ()
{
  return m->parent;
}

void
Listener::Trace (const wxString & msg)
{
  if (m->tracer)
  {
    m->tracer->Trace (msg);
  }

  wxSafeYield ();
}

void
Listener::TraceError (const wxString & msg)
{
  Trace (msg);
}

void
Listener::SetParent (wxWindow * parent)
{
  m->parent = parent;
}


void
Listener::SetContext (svn::Context * context)
{
  m->context = context;
}

svn::Context * 
Listener::GetContext ()
{
  return m->context;
}


bool 
Listener::contextGetLogin (
  const std::string & realm,
  std::string & username, 
  std::string & password)
{
  // TODO: show realm
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
Listener::contextNotify (const char *path,
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
Listener::contextGetLogMessage (std::string & msg)
{
  // Implement code to ask for a log message
  return false;
}


svn::ContextListener::SslServerTrustAnswer
Listener::contextSslServerTrustPrompt (
  const svn::ContextListener::SslServerTrustData & data,
  apr_uint32_t & acceptedFailures)
{
  CertDlg dlg (GetParent (), data);

  dlg.ShowModal ();
  acceptedFailures = dlg.AcceptedFailures ();

  return dlg.Answer ();  
}


bool
Listener::contextSslClientCertPrompt (std::string & certFile)
{
  wxString filename = wxFileSelector (
    _("Select Certificate File"), "", "", "",
    "*.*", wxOPEN | wxFILE_MUST_EXIST,
    GetParent ());

  if (filename.empty ())
    return false;

  //TODO
  certFile = filename.c_str ();
  return true;
}

bool
Listener::contextSslClientCertPwPrompt (std::string & password)
{
  //TODO
  AuthDlg dlg (GetParent (), "", password.c_str (), 
               AuthDlg::HIDE_USERNAME);

  if (dlg.ShowModal () != wxID_OK)
    return false;

  password = dlg.GetPassword ();
  return true;
}



/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
