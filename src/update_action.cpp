
#include "svncpp/modify.h"
#include "include.h"
#include "wx/resource.h"
#include "rapidsvn_app.h"
#include "update_dlg.h"
#include "update_action.h"
#include "svn_notify.h"

UpdateAction::UpdateAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr, apr_array_header_t * trgts):ActionThread (frame, __pool),
  targets
  (trgts)
{
  SetTracer (tr, FALSE);        // do not own the tracer
  revnum = -1;
  rev_specified = false;
}

void
UpdateAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.

#if defined(__WXMSW__)
  // Load the .wxr 'file' from a .rc resource, under Windows.
  Update_Dialog = wxLoadUserResource ("Update_Dialog", "WXRDATA");
  // All resources in the file (only one in this case) get parsed
  // by this call.
  wxResourceParseString (Update_Dialog);
#else
  // Simply parse the data pointed to by the variable Import_Dialog.
  wxResourceParseData (Update_Dialog);
#endif

  UpdateDlg *upDlg = new UpdateDlg;

  if (upDlg->LoadFromResource (mainFrame, "Update_Dialog"))
  {
    upDlg->InitializeData ();

    if (upDlg->ShowModal () == ID_BUTTON_OK)
    {
      user = upDlg->user->GetValue ();
      pass = upDlg->pass->GetValue ();

      // get revision number from dialog
      // #### TODO: check errors
      wxString tmpstr = upDlg->revision->GetValue ();
      TrimString (tmpstr);
      if (tmpstr.IsEmpty ())
        rev_specified = false;
      else
      {
        rev_specified = true;
        tmpstr.ToULong (&revnum, 10);
      }


      // #### TODO: check errors and throw an exception
      // create the thread
      Create ();

      // here we start the action thread
      Run ();

      ////////////////////////////////////////////////////////////
    }
  }

  // destroy the dialog
  upDlg->Close (TRUE);
}

void *
UpdateAction::Entry ()
{
  svn::Modify modify;
  SvnNotify notify (GetTracer ());
  modify.notification (&notify);

  modify.username (user);
  modify.password (pass);

  for (int i = 0; i < targets->nelts; i++)
  {
    const char *target = ((const char **) (targets->elts))[i];

    try
    {
      modify.update (target, revnum, true);
    }
    catch (svn::ClientException &e)
    {
      PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                       ACTION_EVENT);
      GetTracer ()->Trace ("Update failed:");
      GetTracer ()->Trace (e.description ());
    }
  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);

  return NULL;
}
