#include "include.h"
#include "wx/resource.h"
#include "utils.h"
#include "checkout_dlg.h"
#include "notify.h"
#include "auth_baton.h"
#include "rapidsvn_app.h"
#include "checkout_action.h"

CheckoutAction::CheckoutAction (wxFrame * frame, apr_pool_t * __pool, 
                                Tracer * tr):ActionThread (frame, __pool)
{
  thisframe = frame;
  SetTracer (tr, FALSE);        // do not own the tracer
  revnum = 0;
  rev_specified = false;
}

void
CheckoutAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.
  CheckoutDlg *coDlg = new CheckoutDlg(thisframe);

  if (coDlg->ShowModal () != ID_BUTTON_OK)
    return;

  destinationFolder = coDlg->destFolder->GetValue ();
  TrimString (destinationFolder);
  UnixPath (destinationFolder);
  moduleName = coDlg->moduleName->GetValue ();
  TrimString (moduleName);
  user = coDlg->user->GetValue ();
  pass = coDlg->pass->GetValue ();
  recursive = coDlg->recursive->GetValue ();

  // get revision number from dialog
  // #### TODO: check errors
  wxString tmpstr = coDlg->revision->GetValue ();
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

  // destroy the dialog
  coDlg->Close (TRUE);
}

void *
CheckoutAction::Entry ()
{
  AuthBaton auth_baton (pool, user, pass);
  svn_client_revision_t rev;

  svn_wc_notify_func_t notify_func = NULL;
  void *notify_baton = NULL;

  memset (&rev, 0, sizeof (rev));

  svn_cl__get_notifier (&notify_func, &notify_baton,
                        TRUE, FALSE, GetTracer (), pool);

  if (rev_specified)
    revision_from_number (&rev, revnum);

  svn_error_t *err = svn_client_checkout (notify_func,
                                          notify_baton,
                                          auth_baton.auth_obj,
                                          moduleName.c_str (),
                                          destinationFolder.c_str (),
                                          &rev,
                                          recursive,
                                          NULL,
                                          pool);

  if (err)
  {
    PostDataEvent (TOKEN_SVN_INTERNAL_ERROR, err, ACTION_EVENT);
  }

  return NULL;
}
