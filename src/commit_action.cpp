
#include "svncpp/modify.h"
#include "include.h"
#include "wx/resource.h"
#include "commit_dlg.h"
#include "rapidsvn_app.h"
#include "commit_action.h"
#include "svn_notify.h"

CommitAction::CommitAction (wxFrame * frame, apr_pool_t * __pool, 
                            Tracer * tr, apr_array_header_t * trgts)
                            : ActionThread (frame, __pool), targets (trgts)
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
  svn::Modify modify;
  SvnNotify notify (GetTracer ());
  modify.notification (&notify);
  long revision;

  modify.username (user);
  modify.password (pass);

  for (int i = 0; i < targets->nelts; i++)
  {
    const char *target = ((const char **) (targets->elts))[i];

    try
    {
      revision = modify.commit (target, logMsg.c_str (), recursive);

      wxString str =
          wxString::Format ("Committed revision %" SVN_REVNUM_T_FMT ".",
                            revision);
      GetTracer ()->Trace (str);
    }
    catch (svn::ClientException &e)
    {
      PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                       ACTION_EVENT);
    }
  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);

  return NULL;
}
