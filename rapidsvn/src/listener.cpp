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

// app
#include "listener.hpp"
#include "cert_dlg.hpp"
#include "tracer.hpp"
#include "auth_dlg.hpp"
#include "commit_dlg.hpp"
#include "utils.hpp"

//TODO: An exact copy lives in action.cpp which is not used, unlike here
// I suspect we should get rid of one, but I'm not sure which at the  moment...
static const wxChar *
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
   * is ongoing operation to be cancelled?
   * will be evaluated by Listener::contextCancel
   */
  bool isCancelled;


  /**
   * the context under which the action will be
   * performed. this includes auth info and callback
   * addresses
   */
  svn::Context * context;

  svn::Targets targets;

  Data (wxWindow * parnt)
    : parent (parnt), tracer (0), ownTracer (false),
      isCancelled (false)
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
  std::string & password,
  bool & maySave)
{
  // TODO: show realm
  wxString LocalUsername (Utf8ToLocal (username));
  wxString LocalPassword (Utf8ToLocal (password));
  AuthDlg dlg (GetParent (), LocalUsername, LocalPassword);

  bool ok = dlg.ShowModal () == wxID_OK;
  if (ok)
  {
    LocalToUtf8 (dlg.GetUsername (), username);
    LocalToUtf8 (dlg.GetPassword (), password);
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
  const wxChar * actionString = 0;

  if (action >= 0 && action <= MAX_ACTION)
    actionString = ACTION_NAMES [action];

  if (actionString != 0)
  {
    wxString msg, wxpath (Utf8ToLocal (path));
    msg.Printf (wxT("%s: %s"), actionString, wxpath.c_str ());

    Trace (msg);
  }

  // Implement code to generate usefule messages that can be
  // transmitted with "Trace"
  wxSafeYield ();
}


bool
Listener::contextGetLogMessage (std::string & msg)
{
  CommitDlg dlg (GetParent (), true);

  bool ok = dlg.ShowModal () == wxID_OK;
  if (ok)
  {
    LocalToUtf8 (dlg.GetMessage (), msg);
  }
  return ok;
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
    _("Select Certificate File"), wxT(""), wxT(""), wxT(""),
    wxT("*.*"), wxOPEN | wxFILE_MUST_EXIST,
    GetParent ());

  if (filename.empty ())
    return false;

  //TODO
  LocalToUtf8 (filename, certFile);
  return true;
}

bool
Listener::contextSslClientCertPwPrompt (std::string & password,
                                        const std::string & realm,
                                        bool & maySave)
{
  //TODO
  wxString LocalPassword(Utf8ToLocal (password));
  AuthDlg dlg (GetParent (), wxEmptyString, LocalPassword,
               AuthDlg::HIDE_USERNAME);

  if (dlg.ShowModal () != wxID_OK)
    return false;

  LocalToUtf8 (dlg.GetPassword (), password);
  return true;
}


bool
Listener::contextCancel ()
{
  return false;

// Commented out the following code, because
// it caused a lot of flickering on Mac OS/X
// in the menu bar and was really slow.
//
// On the other hand, until we have some real
// threading this isnt of much use anyway 
//  wxSafeYield ();
//  return m->isCancelled;
}


bool
Listener::isCancelled () const
{
  return m->isCancelled;
}


void
Listener::cancel (bool value)
{
  m->isCancelled = value;
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
