#include "include.h"
#include "wx/resource.h"
#include "merge_dlg.h"
#include "auth_baton.h"
#include "tracer.h"
#include "trace_update.h"
#include "rapidsvn_app.h"
#include "merge_action.h"

static void
GetRevFromString (svn_client_revision_t * rev, wxString str)
{
  unsigned long nr;
  TrimString (str);

  rev->kind = svn_client_revision_unspecified;
  if (!str.IsEmpty ())
  {
    rev->kind = svn_client_revision_number;
    str.ToULong (&nr, 10);
    rev->value.number = nr;
  }
}

MergeAction::MergeAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr):ActionThread (frame,
              __pool)
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

      GetRevFromString (&rev_start, mrgDlg->rev_start->GetValue ());
      GetRevFromString (&rev_end, mrgDlg->rev_end->GetValue ());
      GetRevFromString (&rev_second, mrgDlg->rev_second->GetValue ());

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
  AuthBaton auth_baton (pool, user, pass);

  //const svn_delta_editor_t *trace_editor = NULL;
  svn_wc_notify_func_t trace_editor = NULL;     // brm
  //svn_wc_notify_action_t *trace_editor = NULL;

  void *trace_edit_baton = NULL;

  const char *parent_dir = NULL;
  const char *entry = NULL;
  svn_boolean_t using_alternate_syntax = FALSE;
  svn_error_t *err = NULL;

  wxString targetPath =
    wxGetApp ().GetAppFrame ()->GetFolderBrowser ()->GetPath ();

  const char *sourcepath1 = NULL;
  const char *sourcepath2 = NULL;
  const char *targetpath = ".";

  svn_client_revision_t start_revision;
  svn_client_revision_t end_revision;

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


  sourcepath1 = path1.c_str ();

  if (rev_start.kind == svn_client_revision_unspecified)
  {
    start_revision.kind = svn_client_revision_head;

    if (rev_second.kind == svn_client_revision_unspecified)
      end_revision.kind = svn_client_revision_head;
    else
    {
      end_revision.kind = rev_second.kind;
      end_revision.value.number = rev_second.value.number;
    }

    // path2 should be specified if rev_start.kind 
    // is unspecified
    sourcepath2 = path2.c_str ();
  }
  else
  {
    start_revision.kind = rev_start.kind;
    start_revision.value.number = rev_start.value.number;

    if (rev_end.kind == svn_client_revision_unspecified)
    {
      sourcepath2 = path2.c_str ();

      if (rev_second.kind == svn_client_revision_unspecified)
        end_revision.kind = svn_client_revision_head;
      else
      {
        end_revision.kind = rev_second.kind;
        end_revision.value.number = rev_second.value.number;
      }
    }
    else
    {
      // use path1 if rev_end.kind is unspecified
      sourcepath2 = path1.c_str ();

      end_revision.kind = rev_end.kind;
      end_revision.value.number = rev_end.value.number;
    }
  }

  err =
    svn_wc_get_actual_target (targetPath.c_str (), &parent_dir, &entry, pool);
  if (err)
  {
    PostDataEvent (TOKEN_SVN_INTERNAL_ERROR, err, ACTION_EVENT);
    return NULL;
  }

  err = svn_client_merge (trace_editor, trace_edit_baton, auth_baton.auth_obj, sourcepath1, &start_revision, sourcepath2, &end_revision, targetpath, !recursive,        // non-recursive
                          force, pool);

  if (err)
    err = svn_cl__may_need_force (err);

  if (err)
  {
    PostDataEvent (TOKEN_SVN_INTERNAL_ERROR, err, ACTION_EVENT);
  }

  return NULL;
}


MergeAction::~MergeAction ()
{
#if defined(__WXMSW__)
  delete Merge_Dialog;
#endif
}

void
MergeAction::GetNotifier (svn_wc_notify_func_t * notify_func_p,
                          void **notify_baton_p, svn_boolean_t is_checkout,
                          svn_boolean_t suppress_final_line,
                          apr_pool_t * pool)
{

}
