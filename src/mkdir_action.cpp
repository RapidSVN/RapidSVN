
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
  thisframe = frame;
  SetTracer (tr, FALSE);        // do not own the tracer
}

void
MkdirAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.

  MkdirDlg *mkDlg = new MkdirDlg(thisframe, &Data);

  if (mkDlg->ShowModal () == wxID_OK)
  {
    UnixPath (Data.Target);
    TrimString (Data.Target);
    
    // #### TODO: check errors and throw an exception
    // create the thread
    Create ();

    // here we start the action thread
    Run ();
     ////////////////////////////////////////////////////////////
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
  modify.username (Data.User);
  modify.password (Data.Password);

  try
  {
    modify.mkdir (Data.Target, Data.LogMessage);
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
