/*
 * ====================================================================
 * Copyright (c) 2002-2010 The RapidSVN Group.  All rights reserved.
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
 * along with this program (in the file GPL.txt.
 * If not, see <http://www.gnu.org/licenses/>.
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
#include "commit_log_dlg.hpp"
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

  svn::ContextListener::SslServerTrustAnswer sslServerTrustAnswer;
  svn::ContextListener::SslServerTrustData sslServerTrustData;
  apr_uint32_t acceptedFailures;


  Data(wxWindow * parent_)
      : parent(parent_), tracer(NULL), ownTracer(false),
      isCancelled(false), context(NULL), dataReceived(false)
  {
    parentDoneSignal = new wxCondition(mutex);
  }

  virtual ~Data()
  {
    delete parentDoneSignal;

    if (tracer && ownTracer)
    {
      delete tracer;
    }
  }

  /**
   * in a multi threading environment, we have
   * to forward the request as an event to
   * the main thread; this one handles the event
   * and returns the result.
   *
   * if we are already in the main thread we
   * can skip the shit and wait here
   */
  void
  sendSignalAndWait(int id)
  {
    wxCommandEvent event(wxEVT_COMMAND_MENU_SELECTED, id);

    if (::wxIsMainThread())
    {
      // single threaded environment or
      // already in the main thread
      handleEvent(event);
    }
    else
    {
      wxPostEvent(parent, event);
      parentDoneSignal->Wait();
    }
  }


  /**
   * The callback functions will call this
   * function to signal they are done;
   * if we are already in the main thread
   * we do nothing.
   */
  void signalDone()
  {
    /**
     * @todo we have to see whether this works in Linux
     * as well. Maybe we may call this only if
     * @ref sendSignalAndWait was called from a different
     * thread.
     */
    parentDoneSignal->Broadcast();
  }


  void
  callbackSslClientCertPrompt()
  {
    wxMutexLocker lock(mutex);
    wxString localCertFile = wxFileSelector(
                               _("Select Certificate File"), wxT(""), wxT(""), wxT(""),
                               wxT("*.*"), wxOPEN | wxFILE_MUST_EXIST, parent);
    LocalToUtf8(localCertFile, certFile);

    dataReceived = !localCertFile.empty();

    signalDone();
  }


  void
  callbackGetLogin()
  {
    wxMutexLocker lock(mutex);

    // TODO: show realm
    wxString LocalUsername(Utf8ToLocal(username));
    wxString LocalPassword(Utf8ToLocal(password));
    AuthDlg dlg(parent, LocalUsername, LocalPassword);

    bool ok = dlg.ShowModal() == wxID_OK;
    if (ok)
    {
      LocalToUtf8(dlg.GetUsername(), username);
      LocalToUtf8(dlg.GetPassword(), password);
      dataReceived = true;
    }

    signalDone();
  }


  void
  callbackGetLogMessage()
  {
    wxMutexLocker lock(mutex);
    CommitLogDlg dlg(parent);

    bool ok = dlg.ShowModal() == wxID_OK;

    if (ok)
    {
      LocalToUtf8(dlg.GetMessage(), message);
      dataReceived = true;
    }

    signalDone();
  }


  void
  callbackSslServerTrustPrompt()
  {
    wxMutexLocker lock(mutex);
    CertDlg dlg(parent, sslServerTrustData);

    dlg.ShowModal();
    acceptedFailures = dlg.AcceptedFailures();

    sslServerTrustAnswer = dlg.Answer();
    dataReceived = true;

    signalDone();
  }


  void
  callbackSslClientCertPwPrompt()
  {
    wxMutexLocker lock(mutex);
    wxString LocalPassword(Utf8ToLocal(password));
    AuthDlg dlg(parent, wxEmptyString, LocalPassword,
                AuthDlg::HIDE_USERNAME);

    dataReceived = dlg.ShowModal() == wxID_OK;

    if (dataReceived)
      LocalToUtf8(dlg.GetPassword(), password);

    signalDone();
  }


  void
  handleEvent(wxCommandEvent & event)
  {
    switch (event.GetId())
    {
    case SIG_GET_LOG_MSG:
      callbackGetLogMessage();
      break;

    case SIG_GET_LOGIN:
      callbackGetLogin();
      break;

    case SIG_SSL_SERVER_TRUST_PROMPT:
      callbackSslServerTrustPrompt();
      break;

    case SIG_SSL_CLIENT_CERT_PROMPT:
      callbackSslClientCertPrompt();
      break;

    case SIG_SSL_CLIENT_CERT_PW_PROMPT:
      callbackSslClientCertPwPrompt();
      break;

    default:
      // Oh well, no default reaction *sigh*
      ;
    }
  }

};

Listener::Listener(wxWindow * parent)
{
  m = new Data(parent);
}

Listener::~Listener()
{
  delete m;
}

void
Listener::SetTracer(Tracer * t, bool own)
{
  m->tracer = t;
  m->ownTracer = own;
}

Tracer *
Listener::GetTracer()
{
  return m->tracer;
}

wxWindow *
Listener::GetParent()
{
  return m->parent;
}

void
Listener::Trace(const wxString & msg)
{
  if (m->tracer)
  {
    m->tracer->Trace(msg);
  }
}

void
Listener::SetParent(wxWindow * parent)
{
  m->parent = parent;
}

void
Listener::SetContext(svn::Context * context)
{
  m->context = context;
}

svn::Context *
Listener::GetContext()
{
  return m->context;
}

void
Listener::contextNotify(const char *path,
                        svn_wc_notify_action_t action,
                        svn_node_kind_t WXUNUSED(kind),
                        const char * WXUNUSED(mime_type),
                        svn_wc_notify_state_t WXUNUSED(content_state),
                        svn_wc_notify_state_t WXUNUSED(prop_state),
                        svn_revnum_t WXUNUSED(revision))
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

  static const int MAX_ACTION = svn_wc_notify_failed_unlock;

  if (ACTION_NAMES[action] != NULL && action >= 0 && action <= MAX_ACTION)
  {
    static wxString msg;
    msg.Printf(wxT("%s: %s"), ACTION_NAMES[action], Utf8ToLocal(path).c_str());

    Trace(msg);
  }

#ifdef USE_SIMPLE_WORKER
  static apr_time_t last_access = apr_time_now();
  if (apr_time_now() - last_access > 2000000)
  {
    last_access = apr_time_now();
    wxSafeYield();
  }
#endif
}

bool
Listener::contextGetLogin(const std::string & /*realm*/,
                          std::string & username,
                          std::string & password,
                          bool & /*maySave*/)
{
  m->username = username;
  m->password = password;

  m->sendSignalAndWait(SIG_GET_LOGIN);

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

bool
Listener::contextGetLogMessage(std::string & msg)
{
  m->message = msg;

  m->sendSignalAndWait(SIG_GET_LOG_MSG);

  bool success = m->dataReceived;
  m->dataReceived = false;

  if (success)
  {
    msg = m->message;
  }
  return success;
}


svn::ContextListener::SslServerTrustAnswer
Listener::contextSslServerTrustPrompt(
  const svn::ContextListener::SslServerTrustData & data,
  apr_uint32_t & /*acceptedFailures*/)
{
  m->sslServerTrustData = data;
  m->sendSignalAndWait(SIG_SSL_SERVER_TRUST_PROMPT);
  m->dataReceived = false;

  return m->sslServerTrustAnswer;
}


bool
Listener::contextSslClientCertPrompt(std::string & certFile)
{
  m->sendSignalAndWait(SIG_SSL_CLIENT_CERT_PROMPT);

  bool success = m->dataReceived;
  m->dataReceived = false;

  if (!success)
    return false;

  certFile = m->certFile;

  return true;
}


bool
Listener::contextSslClientCertPwPrompt(std::string & password,
                                       const std::string & /*realm*/,
                                       bool & /*maySave*/)
{
  /// @todo @ref realm isnt used yet
  /// @todo @ref maySave isnt used yet
  m->password = password;

  m->sendSignalAndWait(SIG_SSL_CLIENT_CERT_PW_PROMPT);

  bool success = m->dataReceived;
  m->dataReceived = false;

  if (!success)
    return false;

  password = m->password;
  return true;
}

bool
Listener::contextCancel()
{
  return m->isCancelled;
}

bool
Listener::isCancelled() const
{
  return m->isCancelled;
}

void
Listener::cancel(bool value)
{
  m->isCancelled = value;
}

void
Listener::handleEvent(wxCommandEvent & event)
{
  m->handleEvent(event);
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
