
#include "svncpp/modify.h"
#include "include.h"
#include "wx/resource.h"
#include "rapidsvn_app.h"
#include "commit_action.h"
#include "svn_notify.h"

CommitAction::CommitAction (wxFrame * frame, apr_pool_t * __pool, 
                            Tracer * tr, apr_array_header_t * trgts)
                            : ActionThread (frame, __pool), targets (trgts)
{
  thisframe = frame;
  SetTracer (tr, FALSE);        // do not own the tracer
}

void
CommitAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.

  CommitDlg *ciDlg = new CommitDlg(thisframe, &Data);

  if (ciDlg->ShowModal () == wxID_OK)
  {
    // #### TODO: check errors and throw an exception
    // create the thread
    Create ();

    // here we start the action thread
    Run ();

    ////////////////////////////////////////////////////////////
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

  modify.username (Data.User);
  modify.password (Data.Password);

  for (int i = 0; i < targets->nelts; i++)
  {
    const char *target = ((const char **) (targets->elts))[i];

    try
    {
      revision = modify.commit (target, Data.LogMessage.c_str (), Data.Recursive);

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
