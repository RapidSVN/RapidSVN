#include "include.h"
#include "wx/resource.h"
#include "utils.h"
#include "delete_dlg.h"
#include "notify.h"
#include "auth_baton.h"
#include "rapidsvn_app.h"
#include "delete_action.h"

DeleteAction::DeleteAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr, apr_array_header_t * trgts):ActionThread (frame, __pool),
  targets
  (trgts)
{
  SetTracer (tr, FALSE);        // do not own the tracer
  force = false;
}

void
DeleteAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.

#if defined(__WXMSW__)
  // Load the .wxr 'file' from a .rc resource, under Windows.
  Delete_Dialog = wxLoadUserResource ("Delete_Dialog", "WXRDATA");
  // All resources in the file (only one in this case) get parsed
  // by this call.
  wxResourceParseString (Delete_Dialog);
#else
  // Simply parse the data pointed to by the variable Import_Dialog.
  wxResourceParseData (Delete_Dialog);
#endif

  DeleteDlg *delDlg = new DeleteDlg;

  if (delDlg->LoadFromResource (mainFrame, "Delete_Dialog"))
  {
    delDlg->InitializeData ();

    if (delDlg->ShowModal () == ID_BUTTON_OK)
    {
      logMsg = delDlg->logMsg->GetValue ();
      user = delDlg->user->GetValue ();
      pass = delDlg->pass->GetValue ();
      force = delDlg->force->GetValue ();

      // #### TODO: check errors and throw an exception
      // create the thread
      Create ();

      // here we start the action thread
      Run ();

      ////////////////////////////////////////////////////////////
    }
  }

  // destroy the dialog
  delDlg->Close (TRUE);
}

void *
DeleteAction::Entry ()
{
  AuthBaton auth_baton (pool, user, pass);

  svn_wc_notify_func_t notify_func = NULL;
  void *notify_baton = NULL;
  int i;
  apr_pool_t *subpool;
  svn_error_t *err = NULL;
  svn_client_commit_info_t *commit_info = NULL;

  svn_cl__get_notifier (&notify_func, &notify_baton,
                        TRUE, FALSE, GetTracer (), pool);

  subpool = svn_pool_create (pool);

  for (i = 0; i < targets->nelts; i++)
  {
    err = NULL;
    const char *target = ((const char **) (targets->elts))[i];

    commit_info = NULL;

    err = svn_client_delete (&commit_info, target, force,
                             auth_baton.auth_obj,
                             &svn_cl__get_log_message,
                             svn_cl__make_log_msg_baton (logMsg.c_str (),
                                                         NULL, subpool),
                             notify_func, notify_baton, subpool);
    if (err)
    {
      err = svn_cl__may_need_force (err);
      break;
    }

    if (commit_info && SVN_IS_VALID_REVNUM (commit_info->revision))
    {
      wxString str =
        wxString::Format ("Committed revision %" SVN_REVNUM_T_FMT ".",
                          commit_info->revision);
      GetTracer ()->Trace (str);
    }

    svn_pool_clear (subpool);
  }

  if (err)
  {
    PostDataEvent (TOKEN_SVN_INTERNAL_ERROR, err, ACTION_EVENT);
  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);

  svn_pool_destroy (subpool);

  return NULL;
}
