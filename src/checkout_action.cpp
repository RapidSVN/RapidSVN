
#include "svncpp/modify.h"
#include "include.h"
#include "wx/resource.h"
#include "utils.h"
#include "checkout_dlg.h"
#include "rapidsvn_app.h"
#include "checkout_action.h"
#include "svn_notify.h"

CheckoutAction::CheckoutAction (wxFrame * frame, apr_pool_t * __pool, 
                                Tracer * tr):ActionThread (frame, __pool)
{
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
  svn::Modify modify;
  SvnNotify notify (GetTracer ());
  modify.notification (&notify);

  modify.username (Data.User);
  modify.password (Data.Password);

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

  try
  {
    modify.checkout (Data.ModuleName, Data.DestFolder, 
                     revnum, Data.Recursive);
  }
  catch (svn::ClientException &e)
  {
      PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                       ACTION_EVENT);
      GetTracer ()->Trace ("Checkout failed:");
      GetTracer ()->Trace (e.description ());
  }
 
  return NULL;
}
