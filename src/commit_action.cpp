#include "include.h"
#include "wx/resource.h"
#include "utils.h"
#include "commit_dlg.h"
#include "notify.h"
#include "auth_baton.h"
#include "rapidsvn_app.h"
#include "commit_action.h"

CommitAction::CommitAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr, apr_array_header_t * trgts):ActionThread (frame, __pool),
  targets
  (trgts)
{
  SetTracer (tr, FALSE);        // do not own the tracer
  recursive = false;
}

void
CommitAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.

#if defined(__WXMSW__)
  // Load the .wxr 'file' from a .rc resource, under Windows.
  Commit_Dialog = wxLoadUserResource ("Commit_Dialog", "WXRDATA");
  // All resources in the file (only one in this case) get parsed
  // by this call.
  wxResourceParseString (Commit_Dialog);
#else
  // Simply parse the data pointed to by the variable Import_Dialog.
  wxResourceParseData (Commit_Dialog);
#endif

  CommitDlg *ciDlg = new CommitDlg;

  if (ciDlg->LoadFromResource (mainFrame, "Commit_Dialog"))
  {
    ciDlg->InitializeData ();

    if (ciDlg->ShowModal () == ID_BUTTON_OK)
    {
      logMsg = ciDlg->logMsg->GetValue ();
      user = ciDlg->user->GetValue ();
      pass = ciDlg->pass->GetValue ();
      recursive = ciDlg->recursive->GetValue ();

      // #### TODO: check errors and throw an exception
      // create the thread
      Create ();

      // here we start the action thread
      Run ();

      ////////////////////////////////////////////////////////////
    }
  }

  // destroy the dialog
  ciDlg->Close (TRUE);
}

void *
CommitAction::Entry ()
{
  AuthBaton auth_baton (pool, user, pass);

  svn_wc_notify_func_t notify_func = NULL;
  void *notify_baton = NULL;
  const char *base_dir = NULL;
  svn_revnum_t revnum = SVN_INVALID_REVNUM;
  svn_error_t *err = NULL;
  svn_client_commit_info_t *commit_info = NULL;

  svn_cl__get_notifier (&notify_func, &notify_baton,
                        FALSE, FALSE, GetTracer (), pool);

  if ((!targets) || (!targets->nelts))
  {
    const char *parent_dir, *base_name;

    err = svn_wc_get_actual_target (base_dir, &parent_dir, &base_name, pool);
    if (base_name)
      base_dir = apr_pstrdup (pool, parent_dir);
  }

  if (err)
  {
    PostDataEvent (TOKEN_SVN_INTERNAL_ERROR, err, ACTION_EVENT);
    PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);
    return NULL;
  }

  err = svn_client_commit (&commit_info, notify_func, notify_baton, auth_baton.auth_obj, targets, svn_cl__get_log_message, svn_cl__make_log_msg_baton (logMsg.c_str (), base_dir, pool), NULL, revnum, !recursive,      // non-recursive status
                           pool);

  if (err)
  {
    PostDataEvent (TOKEN_SVN_INTERNAL_ERROR, err, ACTION_EVENT);
  }
  else if (commit_info && SVN_IS_VALID_REVNUM (commit_info->revision))
  {
    wxString str =
      wxString::Format ("Committed revision %" SVN_REVNUM_T_FMT ".",
                        commit_info->revision);
    GetTracer ()->Trace (str);
  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);

  return NULL;
}
