/*
 * ====================================================================
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

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
  m_pFrame = frame;
}

void
ImportAction::Perform ()
{
  ImportDlg *impDlg = new ImportDlg(m_pFrame, &Data);

  if (impDlg->ShowModal () == wxID_OK)
  {
    // If path is not specified, get the current selection from 
    // the folder browser
    if (Data.Path.IsEmpty ())
      Data.Path = wxGetApp ().GetAppFrame ()->GetFolderBrowser ()->GetPath ();
      
    // #### TODO: check errors and throw an exception
    // create the thread
    Create ();

    // here we start the action thread
    Run ();
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

  modify.username (Data.User);
  modify.password (Data.Password);
  
  // if new entry is empty, the_new_entry must be left NULL.
  if (!Data.NewEntry.IsEmpty ())
    the_new_entry = Data.NewEntry.c_str ();

  try
  {
    modify.import (Data.Path.c_str (), Data.Repository.c_str (), the_new_entry,
                   Data.LogMessage.c_str(), Data.Recursive);
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

