
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
}

void
MergeAction::Perform ()
{
#if defined(__WXMSW__)
  // Load the .wxr 'file' from a .rc resource, under Windows.
  Merge_Dialog = wxLoadUserResource ("Merge_Dialog", "WXRDATA");
  // All resources in the file (only one in this case) get parsed
  // by this call.
  wxResourceParseString (Merge_Dialog);
#else
  // Simply parse the data pointed to by the variable Merge_Dialog.
  wxResourceParseData (Merge_Dialog);
#endif

  MergeDlg *mrgDlg = new MergeDlg;

  if (mrgDlg->LoadFromResource (mainFrame, "Merge_Dialog"))
  {
    mrgDlg->InitializeData ();

    if (mrgDlg->ShowModal () == ID_BUTTON_OK)
    {
      path1 = mrgDlg->path1->GetValue ();
      TrimString (path1);
      path2 = mrgDlg->path2->GetValue ();
      TrimString (path2);

      rev1 = getRevision (mrgDlg->rev_start->GetValue ());
      rev_end = getRevision (mrgDlg->rev_end->GetValue ());
      rev2 = getRevision (mrgDlg->rev_second->GetValue ());

      user = mrgDlg->user->GetValue ();
      pass = mrgDlg->pass->GetValue ();

      recursive = mrgDlg->recursive->GetValue ();
      force = mrgDlg->force->GetValue ();

      // #### TODO: check errors and throw an exception
      // create the thread
      Create ();

      // here we start the action thread
      Run ();
    }
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

  modify.username (user.c_str ());
  modify.password (pass.c_str ());

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


  try
  {
    modify.merge (path1.c_str (), rev1, path2.c_str (), rev2, 
                  targetPath, force, recursive);
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

MergeAction::~MergeAction ()
{
#if defined(__WXMSW__)
  delete Merge_Dialog;
#endif
}
