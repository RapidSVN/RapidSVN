
#include "svncpp/modify.h"
#include "include.h"
#include "wx/resource.h"
#include "wx/filename.h"
#include "rapidsvn_app.h"
#include "svn_notify.h"
#include "copy_action.h"


CopyAction::CopyAction (wxFrame * frame, apr_pool_t * __pool, 
                        Tracer * tr, apr_array_header_t * trgts )
                        : ActionThread (frame, __pool)
{
  targets = trgts;
  
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
  svn::Modify modify;
  SvnNotify notify (GetTracer ());
  modify.notification (&notify);

  src = ((const char **) (targets->elts))[0];
  dest = DestinationPath (src);

  if(dest.IsEmpty ())
    return NULL;

  try
  {
    modify.copy (src, dest);
    GetTracer ()->Trace ("Copy successful");
  }
  catch (svn::ClientException &e)
  {
    PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                     ACTION_EVENT);
    GetTracer ()->Trace ("Copy failed:");
    GetTracer ()->Trace (e.description ());
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
