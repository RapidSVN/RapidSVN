
#include "svncpp/modify.h"
#include "include.h"
#include "wx/resource.h"
#include "wx/filename.h"
#include "utils.h"
#include "notify.h"
#include "auth_baton.h"
#include "rapidsvn_app.h"
#include "copy_action.h"


CopyAction::CopyAction (wxFrame * frame, apr_pool_t * __pool, 
                        Tracer * tr, apr_array_header_t * trgts )
                        : ActionThread (frame, __pool)
{
  targets = trgts;
  pool = __pool;
  
  SetTracer (tr, FALSE);        // do not own the tracer
}

void
CopyAction::Perform ()
{
  ////////////////////////////////////////////////////////////
  // Here we are in the main thread.

  // #### TODO: check errors and throw an exception
  // create the thread
  Create ();

  // here we start the action thread
  Run ();
}

void *
CopyAction::Entry ()
{
  svn_client_revision_t rev;
  svn_client_commit_info_t *commit_info = NULL;
  svn_wc_notify_func_t notify_func = NULL;
  AuthBaton auth_baton (pool, _T (""), _T (""));
  void *notify_baton = NULL;

  apr_pool_t *subpool;
  svn_error_t *err = NULL;

  src = ((const char **) (targets->elts))[0];
  dest = DestinationPath (src);

  if(dest.IsEmpty ())
    return NULL;

  svn_cl__get_notifier (&notify_func, &notify_baton,
                        TRUE, FALSE, GetTracer (), pool);

  subpool = svn_pool_create (pool);

  memset (&rev, 0, sizeof (rev));
  rev.kind = svn_client_revision_unspecified;


  err = svn_client_copy (&commit_info,
                          src.c_str (),
                          &(rev),
                          dest.c_str (),
                          NULL,
                          auth_baton.auth_obj,
                          &svn_cl__get_log_message,
                          svn_cl__make_log_msg_baton (logMsg.c_str (), 
                                                      NULL, subpool),
                          notify_func, notify_baton,
                          subpool);

  if (err)
  {
    PostDataEvent (TOKEN_SVN_INTERNAL_ERROR, err, ACTION_EVENT);
  }
  else
  {
    if (commit_info && SVN_IS_VALID_REVNUM (commit_info->revision))
    {
      wxString str =
        wxString::Format ("Renamed revision %" SVN_REVNUM_T_FMT ".",
                          commit_info->revision);
      GetTracer ()->Trace (str);
    }
  }

  return NULL;
}

/**
 * Returns the properly formatted destination file name.
 */
wxString
CopyAction::DestinationPath (wxString src)
{
  wxString dest;
  wxFileName file (src);
  wxDirDialog dialog (wxGetApp ().GetAppFrame (), 
                      _T ("Select a directory to copy to"), 
                      wxGetApp ().GetAppFrame ()->GetFolderBrowser ()->GetPath ());

  if (dialog.ShowModal () != wxID_OK)
    return _T ("");


  dest = dialog.GetPath ();
  if(dest.Right (4) == ".svn")
  {
      wxMessageDialog dlg (wxGetApp ().GetAppFrame (), 
                           _T ("This is an invalid directory."),
                           _T ("Error"), wxOK);
      dlg.ShowModal ();

      return _T ("");
  }

  dest = dest + file.GetPathSeparators ().Left (1) + file.GetFullName ();
  dest.Replace(_T ("\\"), _T ("/"), true);

  return dest;
}
