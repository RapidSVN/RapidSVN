
#include "svncpp/modify.h"
#include "include.h"
#include "wx/resource.h"
#include "utils.h"
#include "mkdir_dlg.h"
#include "notify.h"
#include "auth_baton.h"
#include "rapidsvn_app.h"
#include "mkdir_action.h"

MkdirAction::MkdirAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr):ActionThread (frame,
              __pool)
{
  SetTracer (tr, FALSE);        // do not own the tracer
}

void
MkdirAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.

#if defined(__WXMSW__)
  // Load the .wxr 'file' from a .rc resource, under Windows.
  Mkdir_Dialog = wxLoadUserResource ("Mkdir_Dialog", "WXRDATA");
  // All resources in the file (only one in this case) get parsed
  // by this call.
  wxResourceParseString (Mkdir_Dialog);
#else
  // Simply parse the data pointed to by the variable Import_Dialog.
  wxResourceParseData (Mkdir_Dialog);
#endif

  MkdirDlg *mkDlg = new MkdirDlg;

  if (mkDlg->LoadFromResource (mainFrame, "Mkdir_Dialog"))
  {
    mkDlg->InitializeData ();

    if (mkDlg->ShowModal () == ID_BUTTON_OK)
    {
      target = mkDlg->target->GetValue ();
      TrimString (target);
      logMsg = mkDlg->logMsg->GetValue ();
      user = mkDlg->user->GetValue ();
      pass = mkDlg->pass->GetValue ();

      // #### TODO: check errors and throw an exception
      // create the thread
      Create ();

      // here we start the action thread
      Run ();

      ////////////////////////////////////////////////////////////
    }
  }

  // destroy the dialog
  mkDlg->Close (TRUE);
}

void *
MkdirAction::Entry ()
{
  AuthBaton auth_baton (pool, user, pass);

  svn_wc_notify_func_t notify_func = NULL;
  void *notify_baton = NULL;

  svn_error_t *err = NULL;
  svn_client_commit_info_t *commit_info = NULL;

  svn_cl__get_notifier (&notify_func, &notify_baton,
                        FALSE, FALSE, GetTracer (), pool);

  err = svn_client_mkdir (&commit_info,
                          target.c_str (),
                          auth_baton.auth_obj,
                          &svn_cl__get_log_message,
                          svn_cl__make_log_msg_baton (logMsg.c_str (), NULL,
                                                      pool), notify_func,
                          notify_baton, pool);

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
