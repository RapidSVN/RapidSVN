#include "include.h"
#include "wx/resource.h"
#include "utils.h"
#include "copy_dlg.h"
#include "notify.h"
#include "auth_baton.h"
#include "rapidsvn_app.h"
#include "copy_action.h"


CopyAction::CopyAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr):ActionThread (frame,
              __pool)
{
  SetTracer (tr, FALSE);        // do not own the tracer
  revnum = 0;
  rev_specified = false;
}

void
CopyAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.

#if defined(__WXMSW__)
  // Load the .wxr 'file' from a .rc resource, under Windows.
  Copy_Dialog = wxLoadUserResource ("Copy_Dialog", "WXRDATA");
  // All resources in the file (only one in this case) get parsed
  // by this call.
  wxResourceParseString (Copy_Dialog);
#else
  // Simply parse the data pointed to by the variable Import_Dialog.
  wxResourceParseData (Copy_Dialog);
#endif

  CopyDlg *cpDlg = new CopyDlg;

  if (cpDlg->LoadFromResource (mainFrame, "Copy_Dialog"))
  {
    cpDlg->InitializeData ();

    if (cpDlg->ShowModal () == ID_BUTTON_OK)
    {
      dest = cpDlg->dest->GetValue ();
      TrimString (dest);

      if (!svn_path_is_url (dest.c_str ()))
        UnixPath (dest);

      src = cpDlg->src->GetValue ();
      TrimString (src);

      if (!svn_path_is_url (src.c_str ()))
        UnixPath (src);

      logMsg = cpDlg->logMsg->GetValue ();
      TrimString (logMsg);

      user = cpDlg->user->GetValue ();
      pass = cpDlg->pass->GetValue ();

      // get revision number from dialog
      // #### TODO: check errors
      wxString tmpstr = cpDlg->revision->GetValue ();
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
  cpDlg->Close (TRUE);
}

void *
CopyAction::Entry ()
{
  AuthBaton auth_baton (pool, user, pass);
  svn_client_revision_t rev;
  svn_client_commit_info_t *commit_info = NULL;
  svn_boolean_t src_is_url;
  svn_boolean_t dst_is_url;

  svn_wc_notify_func_t notify_func = NULL;
  void *notify_baton = NULL;

  memset (&rev, 0, sizeof (rev));
  rev.kind = svn_client_revision_unspecified;

  src_is_url = svn_path_is_url (src.c_str ());
  dst_is_url = svn_path_is_url (dest.c_str ());

  if ((!src_is_url) && (!dst_is_url))
    ;                           // no notification ( WC -> WC )
  else if ((!src_is_url) && (dst_is_url))
  {
    /* 
       WC->URL
       No notifier here also.
     */
  }
  else if ((src_is_url) && (!dst_is_url))
  {
    /* URL->WC : Use checkout-style notification. */
    svn_cl__get_notifier (&notify_func, &notify_baton,
                          TRUE, FALSE, GetTracer (), pool);
  }
  else
    /* URL->URL : No notification needed. */
    ;

  if (rev_specified)
    revision_from_number (&rev, revnum);

  svn_error_t *err = svn_client_copy (&commit_info,
                                      src.c_str (),
                                      &rev,
                                      dest.c_str (),
                                      auth_baton.auth_obj,
                                      &svn_cl__get_log_message,
                                      svn_cl__make_log_msg_baton (logMsg.
                                                                  c_str (),
                                                                  NULL, pool),
                                      notify_func, notify_baton,
                                      pool);

  if (err)
  {
    PostDataEvent (TOKEN_SVN_INTERNAL_ERROR, err, ACTION_EVENT);
  }
  else
  {
    if (commit_info && SVN_IS_VALID_REVNUM (commit_info->revision))
    {
      wxString str =
        wxString::Format ("Committed revision %" SVN_REVNUM_T_FMT ".",
                          commit_info->revision);
      GetTracer ()->Trace (str);
    }
  }

  return NULL;
}
