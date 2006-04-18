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

// wxWidgets
#include "wx/wx.h"
#include "wx/thread.h"
#include "wx/utils.h"

// svncpp
#include "svncpp/m_check.hpp"

// app
#include "action_event.hpp"
#include "auth_dlg.hpp"
#include "cert_dlg.hpp"
#include "commit_dlg.hpp"
#include "config.hpp"
#include "ids.hpp"
#include "listener.hpp"
#include "tracer.hpp"
#include "utils.hpp"

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

  /**
   * Mutex, condition and flag for protecting shared data
   */
  wxMutex mutex;
  wxCondition * parentDoneSignal;
  bool dataReceived;

  /**
   * Shared between two threads data
   */
  std::string message;
  std::string username;
  std::string password;
  std::string certFile;

  ContextListener::SslServerTrustAnswer sslAnswer;
  ContextListener::SslServerTrustData sslData;
  apr_uint32_t acceptedFailures;

  Data (wxWindow * parent_)
    : parent (parent_), tracer (NULL), ownTracer (false),
      isCancelled (false), context (NULL), dataReceived (false)
  {
    parentDoneSignal = new wxCondition (mutex);
  }

  virtual ~Data ()
  {
    delete parentDoneSignal;

    if (tracer && ownTracer)
    {
      delete tracer;
    }
  }

  void
  SendEventToParent (int id) const
  {
    wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, id);
    wxPostEvent (parent, event);
  }

  void
  WaitForParent () const
  {
    // Initial condition check, do not remove or recompose it.
    // Necessary for the case if these lines are called AFTER sending a signal
    // by parent. If not perform this check, signal is lost and we will wait
    // for the parent eternally.
    if (!dataReceived)
    {
      parentDoneSignal->Wait();
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

void
Listener::contextNotify (const char *path,
                         svn_wc_notify_action_t action,
                         svn_node_kind_t kind,
                         const char *mime_type,
                         svn_wc_notify_state_t content_state,
                         svn_wc_notify_state_t prop_state,
                         svn_revnum_t revision)
{
  static const wxChar *
  ACTION_NAMES [] =
  {
    _("Add"),              // svn_wc_notify_add,
    _("Copy"),             // svn_wc_notify_copy,
    _("Delete"),           // svn_wc_notify_delete,
    _("Restore"),          // svn_wc_notify_restore,
    _("Revert"),           // svn_wc_notify_revert,
    NULL ,                 // NOT USED HERE svn_wc_notify_failed_revert,
    _("Resolved"),         // svn_wc_notify_resolved,
    _("Skip"),             // NOT USED HERE svn_wc_notify_skip,
    _("Deleted"),          // svn_wc_notify_update_delete,
    _("Added"),            // svn_wc_notify_update_add,
    _("Updated"),          // svn_wc_notify_update_update,
    NULL,                  // NOT USED HERE svn_wc_notify_update_completed,
    NULL,                  // NOT USED HERE svn_wc_notify_update_external,
    NULL,                  // NOT USED HERE svn_wc_notify_status_completed
    NULL,                  // NOT USED HERE svn_wc_notify_status_external
    _("Modified"),         // svn_wc_notify_commit_modified,
    _("Added"),            // svn_wc_notify_commit_added,
    _("Deleted"),          // svn_wc_notify_commit_deleted,
    _("Replaced"),         // svn_wc_notify_commit_replaced,
    NULL,                  // NOT USED HERE svn_wc_notify_commit_postfix_txdelta
    NULL,                  // NOT USED HERE svn_wc_notify_blame_revision
    _("Locked"),           // svn_wc_notify_locked
    _("Unlocked"),         // svn_wc_notify_unlocked
    _("Failed to lock"),   // svn_wc_notify_failed_lock
    _("Failed to unlock")  // svn_wc_notify_failed_unlock
  };

#if CHECK_SVN_SUPPORTS_LOCK
  static const int MAX_ACTION = svn_wc_notify_failed_unlock;
#else
  static const int MAX_ACTION = svn_wc_notify_commit_postfix_txdelta;
#endif

  if (ACTION_NAMES[action] != NULL && action >= 0 && action <= MAX_ACTION)
  {
    static wxString msg;
    msg.Printf (wxT("%s: %s"), ACTION_NAMES[action], Utf8ToLocal (path).c_str ());

    Trace (msg);
  }

#ifdef USE_SIMPLE_WORKER
  static apr_time_t last_access = apr_time_now ();
  if (apr_time_now () - last_access > 2000000)
  {
    last_access = apr_time_now ();
    wxSafeYield ();
  }
#endif
}

bool
Listener::contextGetLogin (const std::string & realm,
                           std::string & username,
                           std::string & password,
                           bool & maySave)
{
  m->username = username;
  m->password = password;

  m->SendEventToParent (SIG_GET_LOGIN);
  m->WaitForParent ();

  // Parent has done its job and signalled. Performing main condition check.
  bool success = m->dataReceived;
  // Reset flag for next callbacks
  m->dataReceived = false;

  if (success)
  {
    username = m->username;
    password = m->password;
  }
  return success;
}

void
Listener::callbackGetLogin ()
{
  wxMutexLocker lock (m->mutex);

  // TODO: show realm
  wxString LocalUsername (Utf8ToLocal (m->username));
  wxString LocalPassword (Utf8ToLocal (m->password));
  AuthDlg dlg (GetParent (), LocalUsername, LocalPassword);

  bool ok = dlg.ShowModal () == wxID_OK;
  if (ok)
  {
    LocalToUtf8 (dlg.GetUsername (), m->username);
    LocalToUtf8 (dlg.GetPassword (), m->password);
    m->dataReceived = true;
  }
  m->parentDoneSignal->Broadcast ();
}

bool
Listener::contextGetLogMessage (std::string & msg)
{
  m->message = msg;

  m->SendEventToParent (SIG_GET_LOG_MSG);
  m->WaitForParent ();

  bool success = m->dataReceived;
  m->dataReceived = false;

  if (success)
  {
    msg = m->message;
  }
  return success;
}

void
Listener::callbackGetLogMessage ()
{
  wxMutexLocker lock (m->mutex);
  CommitDlg dlg (GetParent (), true);

  bool ok = dlg.ShowModal () == wxID_OK;

  if (ok)
  {
    LocalToUtf8 (dlg.GetMessage (), m->message);
    m->dataReceived = true;
  }
  m->parentDoneSignal->Broadcast ();
}

svn::ContextListener::SslServerTrustAnswer
Listener::contextSslServerTrustPrompt (
  const svn::ContextListener::SslServerTrustData & data,
  apr_uint32_t & acceptedFailures)
{
  m->sslAnswer = DONT_ACCEPT;
  m->sslData = data;

  m->SendEventToParent (SIG_SSL_TRUST_PROMPT);
  m->WaitForParent ();

  // Parent has done its job and signalled. Performing main condition check.
  bool success = m->dataReceived;
  // Reset flag for next callbacks
  m->dataReceived = false;

  if (success)
  {
    acceptedFailures = m->acceptedFailures;
  }
  return m->sslAnswer;
}

void
Listener::callbackSslServerTrustPrompt ()
{
  wxMutexLocker lock (m->mutex);
  CertDlg dlg (GetParent (), m->sslData);

  bool ok = dlg.ShowModal () == wxID_OK;

  if (ok)
  {
    m->acceptedFailures = dlg.AcceptedFailures ();
    m->sslAnswer = dlg.Answer ();
    m->dataReceived = true;
  }
  m->parentDoneSignal->Broadcast ();
}

bool
Listener::contextSslClientCertPrompt (std::string & certFile)
{
  m->certFile = certFile;

  m->SendEventToParent (SIG_SSL_CERT_PW_PROMPT);
  m->WaitForParent ();

  // Parent has done its job and signalled. Performing main condition check.
  bool success = m->dataReceived;
  // Reset flag for next callbacks
  m->dataReceived = false;

  if (success)
  {
    certFile = m->certFile;
  }
  return success;
}

void
Listener::callbackSslClientCertPrompt ()
{
  wxMutexLocker lock (m->mutex);

  wxString filename = wxFileSelector (
    _("Select Certificate File"), wxT(""), wxT(""), wxT(""),
    wxT("*.*"), wxOPEN | wxFILE_MUST_EXIST,
    GetParent ());

  if (!filename.empty ())
  {
    LocalToUtf8 (filename, m->certFile);
    m->dataReceived = true;
  }
  m->parentDoneSignal->Broadcast ();
}

bool
Listener::contextSslClientCertPwPrompt (std::string & password,
                                        const std::string & realm,
                                        bool & maySave)
{
  m->password = password;

  m->SendEventToParent (SIG_SSL_CERT_PW_PROMPT);
  m->WaitForParent ();

  // Parent has done its job and signalled. Performing main condition check.
  bool success = m->dataReceived;
  // Reset flag for next callbacks
  m->dataReceived = false;

  if (success)
  {
    password = m->password;
  }
  return success;
}

void
Listener::callbackSslClientCertPwPrompt ()
{
  wxMutexLocker lock (m->mutex);

  wxString LocalPassword(Utf8ToLocal (m->password));
  AuthDlg dlg (GetParent (), wxEmptyString, LocalPassword,
               AuthDlg::HIDE_USERNAME);

  bool ok = dlg.ShowModal () == wxID_OK;

  if (ok)
  {
    LocalToUtf8 (dlg.GetPassword (), m->password);    
    m->dataReceived = true;
  }
  m->parentDoneSignal->Broadcast ();
}

bool
Listener::contextCancel ()
{
  return m->isCancelled;
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
