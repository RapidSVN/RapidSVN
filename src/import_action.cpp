
#include "svncpp/modify.h"
#include "include.h"
#include "wx/resource.h"
#include "import_dlg.h"
#include "tracer.h"
#include "rapidsvn_app.h"
#include "import_action.h"
#include "svn_notify.h"

ImportAction::ImportAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr):ActionThread (frame,
              __pool)
{
  SetTracer (tr, FALSE);        // do not own the tracer
}

void
ImportAction::Perform ()
{
#if defined(__WXMSW__)
  // Load the .wxr 'file' from a .rc resource, under Windows.
  Import_Dialog = wxLoadUserResource ("Import_Dialog", "WXRDATA");
  // All resources in the file (only one in this case) get parsed
  // by this call.
  wxResourceParseString (Import_Dialog);
#else
  // Simply parse the data pointed to by the variable Import_Dialog.
  wxResourceParseData (Import_Dialog);
#endif

  ImportDlg *impDlg = new ImportDlg;

  if (impDlg->LoadFromResource (mainFrame, "Import_Dialog"))
  {
    impDlg->InitializeData ();

    if (impDlg->ShowModal () == ID_BUTTON_OK)
    {
      reposURL = impDlg->reposURL->GetValue ();
      TrimString (reposURL);
      path = impDlg->path->GetValue ();
      TrimString (path);
      new_entry = impDlg->new_entry->GetValue ();
      TrimString (new_entry);
      logMsg = impDlg->logMsg->GetValue ();
      user = impDlg->user->GetValue ();
      pass = impDlg->pass->GetValue ();
      recursive = impDlg->recursive->GetValue ();

      // If path is not specified, get the current selection from 
      // the folder browser
      if (path.IsEmpty ())
        path = wxGetApp ().GetAppFrame ()->GetFolderBrowser ()->GetPath ();

      // #### TODO: check errors and throw an exception
      // create the thread
      Create ();

      // here we start the action thread
      Run ();
    }
  }

  // destroy the dialog
  impDlg->Close (TRUE);
}

void *
ImportAction::Entry ()
{
  svn::Modify modify;
  SvnNotify notify (GetTracer ());
  modify.notification (&notify);
  const char *the_new_entry = NULL;

  modify.username (user);
  modify.password (pass);
  
  // if new entry is empty, the_new_entry must be left NULL.
  if (!new_entry.IsEmpty ())
    the_new_entry = new_entry.c_str ();

  try
  {
    modify.import (path.c_str (), reposURL.c_str (), the_new_entry,
                   logMsg.c_str(), recursive);
    GetTracer ()->Trace ("Import successful.");
  }
  catch (svn::ClientException &e)
  {
    PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                     ACTION_EVENT);
    GetTracer ()->Trace ("Import failed:");
    GetTracer ()->Trace (e.description ());
  }

  return NULL;
}

ImportAction::~ImportAction ()
{
#if defined(__WXMSW__)
  delete Import_Dialog;
#endif
}
