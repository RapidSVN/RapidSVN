
#include "svncpp/modify.h"
#include "include.h"
#include "wx/resource.h"
#include "utils.h"
#include "mkdir_dlg.h"
#include "rapidsvn_app.h"
#include "mkdir_action.h"
#include "svn_notify.h"

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
      UnixPath (target);
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
  svn::Modify modify;
  SvnNotify notify (GetTracer ());
  modify.notification (&notify);

  modify.username (user);
  modify.password (user);

  try
  {
    modify.mkdir (target, logMsg);
  }
  catch (svn::ClientException &e)
  {
    PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                     ACTION_EVENT);
    GetTracer ()->Trace ("Mkdir failed:");
    GetTracer ()->Trace (e.description ());
  }

  return NULL;
}
