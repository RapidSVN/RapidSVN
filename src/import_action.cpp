#include "include.h"
#include "wx/resource.h"
#include "import_dlg.h"
#include "auth_baton.h"
#include "tracer.h"
#include "notify.h"
#include "rapidsvn_app.h"
#include "import_action.h"

ImportAction::ImportAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr):ActionThread (frame,
              __pool)
{
  SetTracer (tr, FALSE);        // do not own the tracer
}

void
ImportAction::Perform ()
{
#if defined(__WXMSW__)
  // Load the .wxr 'file' from a .rc resource, under Windows.
  Import_Dialog = wxLoadUserResource ("Import_Dialog", "WXRDATA");
  // All resources in the file (only one in this case) get parsed
  // by this call.
  wxResourceParseString (Import_Dialog);
#else
  // Simply parse the data pointed to by the variable Import_Dialog.
  wxResourceParseData (Import_Dialog);
#endif

  ImportDlg *impDlg = new ImportDlg;

  if (impDlg->LoadFromResource (mainFrame, "Import_Dialog"))
  {
    impDlg->InitializeData ();

    if (impDlg->ShowModal () == ID_BUTTON_OK)
    {
      reposURL = impDlg->reposURL->GetValue ();
      TrimString (reposURL);
      path = impDlg->path->GetValue ();
      TrimString (path);
      new_entry = impDlg->new_entry->GetValue ();
      TrimString (new_entry);
      logMsg = impDlg->logMsg->GetValue ();
      user = impDlg->user->GetValue ();
      pass = impDlg->pass->GetValue ();
      recursive = impDlg->recursive->GetValue ();

      // If path is not specified, get the current selection from 
      // the folder browser
      if (path.IsEmpty ())
        path = wxGetApp ().GetAppFrame ()->GetFolderBrowser ()->GetPath ();

      // #### TODO: check errors and throw an exception
      // create the thread
      Create ();

      // here we start the action thread
      Run ();
    }
  }

  // destroy the dialog
  impDlg->Close (TRUE);
}

void *
ImportAction::Entry ()
{
  AuthBaton auth_baton (pool, user, pass);

  svn_wc_notify_func_t notify_func = NULL;
  void *notify_baton = NULL;
  const char *the_new_entry = NULL;
  svn_revnum_t revnum = SVN_INVALID_REVNUM;
  svn_client_commit_info_t *commit_info = NULL;

  svn_cl__get_notifier (&notify_func, &notify_baton,
                        FALSE, FALSE, GetTracer (), pool);

  // if new entry is empty, the_new_entry must be left NULL.
  if (!new_entry.IsEmpty ())
    the_new_entry = new_entry.c_str ();

  svn_error_t *err = svn_client_import (&commit_info,
                                        notify_func, notify_baton,
                                        auth_baton.auth_obj,
                                        path.c_str (),
                                        reposURL.c_str (),
                                        the_new_entry,
                                        &svn_cl__get_log_message,
                                        svn_cl__make_log_msg_baton (logMsg.
                                                                    c_str (),
                                                                    NULL,
                                                                    pool),
                                        NULL,
                                        revnum,
                                        !recursive,     // need non-recursive status
                                        pool);

  if (err == SVN_NO_ERROR)
  {
    if (commit_info && SVN_IS_VALID_REVNUM (commit_info->revision))
    {
      wxString str =
        wxString::Format ("Committed revision %" SVN_REVNUM_T_FMT ".",
                          commit_info->revision);
      GetTracer ()->Trace (str);
    }
  }
  else
  {
    PostDataEvent (TOKEN_SVN_INTERNAL_ERROR, err, ACTION_EVENT);
  }

  return NULL;
}

ImportAction::~ImportAction ()
{
#if defined(__WXMSW__)
  delete Import_Dialog;
#endif
}
