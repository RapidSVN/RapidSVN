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
      if(delDlg->force->GetValue () == 1)
        force = true;

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
  svn_wc_notify_func_t notify_func = NULL;
  void *notify_baton = NULL;

  svn::Modify modify;
  svn::Error Err;

  svn_cl__get_notifier (&notify_func, &notify_baton,
                        TRUE, FALSE, GetTracer (), pool);

  for (int i = 0; i < targets->nelts; i++)
  {
    const char *target = ((const char **) (targets->elts))[i];

    if(!modify.remove (target, force))
      Err.setError (modify.getError ());
    else
      GetTracer ()->Trace ("Deletion successful");
  }

  if(Err.exists ())
  {
    PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (Err.message ()), ACTION_EVENT);
    GetTracer ()->Trace ("Could not delete");
  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);

  return NULL;
}
