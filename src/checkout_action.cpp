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
  m_pFrame = frame;
  SetTracer (tr, FALSE);        // do not own the tracer
  m_pFrame = frame;
}

void
CheckoutAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.
  CheckoutDlg *coDlg = new CheckoutDlg(m_pFrame, &Data);

  if (coDlg->ShowModal () == wxID_OK)
  {
    // #### TODO: check errors and throw an exception
    // create the thread
    Create ();

    // here we start the action thread
    Run ();
  }
  
  // destroy the dialog
  coDlg->Close (TRUE);
}

void *
CheckoutAction::Entry ()
{
  AuthBaton auth_baton (pool, Data.User, Data.Password);
  svn_client_revision_t rev;

  svn_wc_notify_func_t notify_func = NULL;
  void *notify_baton = NULL;

  TrimString(Data.DestFolder);
  UnixPath(Data.DestFolder);
  TrimString(Data.ModuleName);
  
  long revnum = -1;
  // Did the user request a specific revision?:
  if (!Data.UseLatest)
  {
    TrimString(Data.Revision);
    if (!Data.Revision.IsEmpty ())
      Data.Revision.ToLong(&revnum, 10);  // If this fails, revnum is unchanged.
  }

  memset (&rev, 0, sizeof (rev));

  svn_cl__get_notifier (&notify_func, &notify_baton,
                        TRUE, FALSE, GetTracer (), pool);

  if (!Data.UseLatest)
    revision_from_number (&rev, revnum);

  svn_error_t *err = svn_client_checkout (notify_func,
                                          notify_baton,
                                          auth_baton.auth_obj,
                                          Data.ModuleName.c_str (),
                                          Data.DestFolder.c_str (),
                                          &rev,
                                          Data.Recursive,
                                          NULL,
                                          pool);

  if (err)
  {
    PostDataEvent (TOKEN_SVN_INTERNAL_ERROR, err, ACTION_EVENT);
  }
  
  return NULL;
}
