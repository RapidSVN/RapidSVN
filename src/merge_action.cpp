
#include "svncpp/modify.h"
#include "include.h"
#include "wx/resource.h"
#include "merge_dlg.h"
#include "tracer.h"
#include "trace_update.h"
#include "rapidsvn_app.h"
#include "merge_action.h"
#include "svn_notify.h"

MergeAction::MergeAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr)
            : ActionThread (frame, __pool)
{
  SetTracer (tr, FALSE);        // do not own the tracer
  m_pFrame = frame;
}

void
MergeAction::Perform ()
{
  MergeDlg *mrgDlg = new MergeDlg(m_pFrame, &Data);

  if (mrgDlg->ShowModal () == wxID_OK)
  {
    // #### TODO: check errors and throw an exception
    // create the thread
    Create ();

    // here we start the action thread
    Run ();
  }

  // destroy the dialog
  mrgDlg->Close (TRUE);
}

void *
MergeAction::Entry ()
{
  svn::Modify modify;
  SvnNotify notify (GetTracer ());
  modify.notification (&notify);

  modify.username (Data.User.c_str ());
  modify.password (Data.Password.c_str ());

  wxString targetPath =
    wxGetApp ().GetAppFrame ()->GetFolderBrowser ()->GetPath ();

  const char *targetpath = ".";

  // Set current working directory to point to the path
  // in the folder browser (the path where the merge will be 
  // performed)
  if (!wxSetWorkingDirectory (targetPath))
  {
    PostStringEvent (TOKEN_VSVN_INTERNAL_ERROR,
                     wxT ("Could not se working directory to ") +
                     targetPath, ACTION_EVENT);
    return NULL;
  }


  long rev1 = MergeAction::getRevision (Data.Path1Rev1);
  long rev2 = MergeAction::getRevision (Data.Path2Rev);
  try
  {
    modify.merge (Data.Path1.c_str (), rev1, Data.Path2.c_str (), rev2, 
                  targetPath, Data.Force, Data.Recursive);
  }
  catch (svn::ClientException &e)
  {
    PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                     ACTION_EVENT);
    GetTracer ()->Trace ("Merge failed:");
    GetTracer ()->Trace (e.description ());
  }
  
  return NULL;
}

long 
MergeAction::getRevision (wxString & str)
{
  unsigned long rev;
  TrimString (str);

  if (!str.IsEmpty ())
  {
    str.ToULong (&rev, 10);
    return rev;
  }

  return -2; // head
}

