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

// svncpp
#include "svncpp/exception.hpp"
#include "svncpp/modify.hpp"

// app
#include "include.hpp"
#include "merge_dlg.hpp"
#include "tracer.hpp"
#include "trace_update.hpp"
#include "rapidsvn_app.hpp"
#include "merge_action.hpp"
#include "svn_notify.hpp"

MergeAction::MergeAction (wxFrame * frame, Tracer * tr)
            : ActionThread (frame)
{
  SetTracer (tr, FALSE);        // do not own the tracer
  m_pFrame = frame;
}

void
MergeAction::Perform ()
{
  MergeDlg *mrgDlg = new MergeDlg(m_pFrame, &m_data);

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

  modify.username (m_data.User.c_str ());
  modify.password (m_data.Password.c_str ());

  wxString targetPath =
    wxGetApp ().GetAppFrame ()->GetFolderBrowser ()->GetPath ();

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


  long rev1 = MergeAction::getRevision (m_data.Path1Rev);
  long rev2 = MergeAction::getRevision (m_data.Path2Rev);
  try
  {
    modify.merge (m_data.Path1.c_str (), rev1, m_data.Path2.c_str (), rev2, 
                  targetPath, m_data.Force, m_data.Recursive);
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

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
