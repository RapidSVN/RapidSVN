/*
 * ====================================================================
 * Copyright (c) 2002-2012 The RapidSVN Group.  All rights reserved.
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
                               wxT("*.*"), wxFD_OPEN | wxFD_FILE_MUST_EXIST, parent);
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

struct UpdateCounter
{
public:
  int conflicted;
  int merged;
  int added;
  int deleted;
  int updated;
  int externalCounter;

  UpdateCounter() : conflicted(0), merged(0), added(0), deleted(0), updated(0), externalCounter(0) {}

  void Reset()
  {
    conflicted = 0;
    merged = 0;
    added = 0;
    deleted = 0;
    updated = 0;
    externalCounter = 0;
  }

  wxString GetMessage() const
  {
    wxString message;
    AppendToMessage(_("Conflicted"), conflicted, message);
    AppendToMessage(_("Merged"), merged, message);
    AppendToMessage(_("Added"), added, message);
    AppendToMessage(_("Deleted"), deleted, message);
    AppendToMessage(_("Updated"), updated, message);
    return message;
  }

private:
  void AppendToMessage(const wxString &label, int count, wxString &output) const
  {
    if (count > 0)
    {
      if (!output.IsEmpty())
      {
        output += wxT(", ");
      }
      output += label;
      output += wxT(": ");
      output += wxString::Format(wxT("%d"), count);
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
Listener::Trace(LogItemType type, const wxString & action, const wxString & msg)
{
  if (m->tracer)
  {
    m->tracer->Trace(type, action, msg);
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

static struct
{
  LogItemType type;
  const wxChar * name;
}
ACTION_NAMES [] =
{
  {LogItem_Added, _("Add")},               // svn_wc_notify_add,
  {LogItem_Normal, _("Copy")},             // svn_wc_notify_copy,
  {LogItem_Added, _("Delete")},            // svn_wc_notify_delete,
  {LogItem_Normal, _("Restore")},          // svn_wc_notify_restore,
  {LogItem_Normal, _("Revert")},           // svn_wc_notify_revert,
  {LogItem_Normal, NULL },                 // NOT USED HERE svn_wc_notify_failed_revert,
  {LogItem_Normal, _("Resolved")},         // svn_wc_notify_resolved,
  {LogItem_Normal, _("Skip")},             // svn_wc_notify_skip,
  {LogItem_Deleted,_("Deleted")},          // svn_wc_notify_update_delete,
  {LogItem_Added,_("Added")},              // svn_wc_notify_update_add,
  {LogItem_Updated,_("Updated")},          // svn_wc_notify_update_update,
  {LogItem_Normal, NULL},                  // NOT USED HERE svn_wc_notify_update_completed,
  {LogItem_Normal, _("External")},         // svn_wc_notify_update_external,
  {LogItem_Normal, NULL},                  // NOT USED HERE svn_wc_notify_status_completed
  {LogItem_Normal, NULL},                  // NOT USED HERE svn_wc_notify_status_external
  {LogItem_Updated,_("Modified")},         // svn_wc_notify_commit_modified,
  {LogItem_Added,_("Added")},              // svn_wc_notify_commit_added,
  {LogItem_Deleted,_("Deleted")},          // svn_wc_notify_commit_deleted,
  {LogItem_Normal, _("Replaced")},         // svn_wc_notify_commit_replaced,
  {LogItem_Normal, NULL},                  // NOT USED HERE svn_wc_notify_commit_postfix_txdelta
  {LogItem_Normal, NULL},                  // NOT USED HERE svn_wc_notify_blame_revision
  {LogItem_Normal, _("Locked")},           // svn_wc_notify_locked
  {LogItem_Normal, _("Unlocked")},         // svn_wc_notify_unlocked
  {LogItem_Normal, _("Failed to lock")},   // svn_wc_notify_failed_lock
  {LogItem_Normal, _("Failed to unlock")}, // svn_wc_notify_failed_unlock
  {LogItem_Normal, NULL},                  //   svn_wc_notify_exists
  {LogItem_Normal, NULL},                  //   svn_wc_notify_changelist_set
  {LogItem_Normal, NULL},                  //   svn_wc_notify_changelist_clear
  {LogItem_Normal, NULL},                  //   svn_wc_notify_changelist_moved
  {LogItem_Normal, NULL},                  //   svn_wc_notify_merge_begin
  {LogItem_Normal, NULL},                  //   svn_wc_notify_foreign_merge_begin
  {LogItem_Updated, _("Replaced")},        //   svn_wc_notify_update_replace
  {LogItem_Normal, NULL},                  //   svn_wc_notify_property_added
  {LogItem_Normal, NULL},                  //   svn_wc_notify_property_modified
  {LogItem_Normal, NULL},                  //   svn_wc_notify_property_deleted
  {LogItem_Normal, NULL},                  //   svn_wc_notify_property_deleted_nonexistent
  {LogItem_Normal, NULL},                  //   svn_wc_notify_revprop_set
  {LogItem_Normal, NULL},                  //   svn_wc_notify_revprop_deleted
  {LogItem_Normal, NULL},                  //   svn_wc_notify_merge_completed
  {LogItem_Conflicted, _("Tree conflict")},//   svn_wc_notify_tree_conflict
  {LogItem_Normal, NULL},                  //   svn_wc_notify_failed_external
  {LogItem_Normal, NULL},                  //   svn_wc_notify_update_started
  {LogItem_Normal, NULL},                  //   svn_wc_notify_update_skip_obstruction
  {LogItem_Normal, NULL},                  //   svn_wc_notify_update_skip_working_only
  {LogItem_Normal, NULL},                  //   svn_wc_notify_update_skip_access_denied
  {LogItem_Normal, NULL},                  //   svn_wc_notify_update_external_removed
  {LogItem_Normal, NULL},                  //   svn_wc_notify_update_shadowed_add
  {LogItem_Normal, NULL},                  //   svn_wc_notify_update_shadowed_update
  {LogItem_Normal, NULL},                  //   svn_wc_notify_update_shadowed_delete
  {LogItem_Normal, NULL},                  //   svn_wc_notify_merge_record_info
  {LogItem_Normal, NULL},                  //   svn_wc_notify_upgraded_path
  {LogItem_Normal, NULL},                  //   svn_wc_notify_merge_record_info_begin
  {LogItem_Normal, NULL},                  //   svn_wc_notify_merge_elide_info
  {LogItem_Normal, NULL},                  //   svn_wc_notify_patch
  {LogItem_Normal, NULL},                  //   svn_wc_notify_patch_applied_hunk
  {LogItem_Normal, NULL},                  //   svn_wc_notify_patch_rejected_hunk
  {LogItem_Normal, NULL},                  //   svn_wc_notify_patch_hunk_already_applied
  {LogItem_Normal, NULL},                  //   svn_wc_notify_commit_copied
  {LogItem_Normal, NULL},                  //   svn_wc_notify_commit_copied_replaced
  {LogItem_Normal, NULL},                  //   svn_wc_notify_url_redirect
  {LogItem_Normal, NULL},                  //   svn_wc_notify_path_nonexistent
  {LogItem_Normal, NULL},                  //   svn_wc_notify_exclude
  {LogItem_Normal, NULL},                  //   svn_wc_notify_failed_conflict
  {LogItem_Normal, NULL},                  //   svn_wc_notify_failed_missing
  {LogItem_Normal, NULL},                  //   svn_wc_notify_failed_out_of_date
  {LogItem_Normal, NULL},                  //   svn_wc_notify_failed_no_parent
  {LogItem_Normal, NULL},                  //   svn_wc_notify_failed_locked
  {LogItem_Normal, NULL},                  //   svn_wc_notify_failed_forbidden_by_server
  {LogItem_Normal, NULL},                  //   svn_wc_notify_skip_conflicted
  {LogItem_Updated, _("Lock broken")},     //   svn_wc_notify_update_broken_lock
  {LogItem_Normal, NULL},                  //   svn_wc_notify_failed_obstruction
  {LogItem_Conflicted, _("Conflicted")},   //   svn_wc_notify_conflict_resolver_starting
  {LogItem_Normal, NULL},                  //   svn_wc_notify_conflict_resolver_done
  {LogItem_Normal, NULL},                  //   svn_wc_notify_left_local_modifications
  {LogItem_Normal, NULL},                  //   svn_wc_notify_foreign_copy_begin
  {LogItem_Normal, NULL},                  //   svn_wc_notify_move_broken
  {LogItem_Normal, NULL},                  //   svn_wc_notify_cleanup_external
  {LogItem_Normal, NULL},                  //   svn_wc_notify_failed_requires_target
  {LogItem_Normal, NULL},                  //   svn_wc_notify_info_external
  {LogItem_Normal, NULL},                  //   svn_wc_notify_commit_finalizing
};
void
Listener::TraceDefaultMessage(svn_wc_notify_action_t action, const char * path)
{


  static const int MAX_ACTION = svn_wc_notify_commit_finalizing;

  if (ACTION_NAMES[action].name != NULL && action >= 0 && action <= MAX_ACTION)
  {
    Trace(ACTION_NAMES[action].type, ACTION_NAMES[action].name, Utf8ToLocal(path).c_str());
  }
#if 0 // TEST TEST TEST
  else if(action > MAX_ACTION)
  {
    Trace(LogItem_Normal, wxString::Format("Action %d", action), Utf8ToLocal(path).c_str());
  }
#endif
}

void
Listener::contextNotify(const char *path,
                        svn_wc_notify_action_t action,
                        svn_node_kind_t kind,
                        const char * WXUNUSED(mime_type),
                        svn_wc_notify_state_t content_state,
                        svn_wc_notify_state_t prop_state,
                        svn_revnum_t revision)
{
  static UpdateCounter updateCounter;

  TraceDefaultMessage(action, path);
  switch (action)
  {
  case svn_wc_notify_update_add:
    if ((content_state == svn_wc_notify_state_conflicted) || (prop_state == svn_wc_notify_state_conflicted))
    {
      updateCounter.conflicted++;
    }
    else
    {
      updateCounter.added++;
    }
    break;

  case svn_wc_notify_update_delete:
    updateCounter.deleted++;
    break;

  case svn_wc_notify_update_update:
    if ((kind == svn_node_dir) && ((prop_state == svn_wc_notify_state_inapplicable) || (prop_state == svn_wc_notify_state_unknown) || (prop_state == svn_wc_notify_state_unchanged)))
    {
      break;
    }
    if ((content_state == svn_wc_notify_state_conflicted) || (prop_state == svn_wc_notify_state_conflicted))
    {
      updateCounter.conflicted++;
    }
    else if ((content_state == svn_wc_notify_state_merged) || (prop_state == svn_wc_notify_state_merged))
    {
      updateCounter.merged++;
    }
    else if ((content_state == svn_wc_notify_state_changed) || (prop_state == svn_wc_notify_state_changed))
    {
      updateCounter.updated++;
    }
    break;

  case svn_wc_notify_update_external:
    updateCounter.externalCounter++;
    break;

  case svn_wc_notify_update_completed:
    if (updateCounter.externalCounter == 0)
    {
      Trace(LogItem_Normal, _("Completed"), wxString::Format(_("At revision %ld"), revision));
      wxString info = updateCounter.GetMessage();
      if (!info.IsEmpty())
      {
        Trace(LogItem_Normal, wxEmptyString, info);
      }
      updateCounter.Reset();
    }
    else
    {
      Trace(LogItem_Normal, _("Completed"), wxString::Format(_("%s at revision %ld"), Utf8ToLocal(path).c_str(), revision));
      updateCounter.externalCounter--;
    }
    break;

  default:
    break;
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
