/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#include "svncpp/modify.hpp"
#include "include.hpp"
#include "wx/resource.h"
#include "import_dlg.hpp"
#include "tracer.hpp"
#include "rapidsvn_app.hpp"
#include "import_action.hpp"
#include "svn_notify.hpp"

ImportAction::ImportAction (wxFrame * frame, Tracer * tr)
  :ActionThread (frame)
{
  SetTracer (tr, FALSE);        // do not own the tracer
  m_pFrame = frame;
}

void
ImportAction::Perform ()
{
  ImportDlg *impDlg = new ImportDlg(m_pFrame, &m_data);

  if (impDlg->ShowModal () == wxID_OK)
  {
    // If path is not specified, get the current selection from 
    // the folder browser
    if (m_data.Path.IsEmpty ())
      m_data.Path = wxGetApp ().GetAppFrame ()->GetFolderBrowser ()->GetPath ();
      
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

  modify.username (m_data.User);
  modify.password (m_data.Password);
  
  // if new entry is empty, the_new_entry must be left NULL.
  if (!m_data.NewEntry.IsEmpty ())
    the_new_entry = m_data.NewEntry.c_str ();

  try
  {
    modify.import (m_data.Path.c_str (), m_data.Repository.c_str (), the_new_entry,
                   m_data.LogMessage.c_str(), m_data.Recursive);
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

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
