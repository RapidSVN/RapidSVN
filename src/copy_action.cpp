/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxwindows
#include "wx/filename.h"

// svncpp
#include "svncpp/exception.hpp"
#include "svncpp/client.hpp"

// app
#include "include.hpp"
#include "rapidsvn_app.hpp"
#include "svn_notify.hpp"
#include "copy_action.hpp"


CopyAction::CopyAction (wxFrame * frame, Tracer * tr, 
                        const svn::Targets & targets )
  : ActionThread (frame), m_targets(targets)
{
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
  svn::Client client;
  SvnNotify notify (GetTracer ());
  client.notification (&notify);

  m_src = m_targets.targets ()[0].c_str ();
  m_dest = DestinationPath (m_src);

  if(m_dest.IsEmpty ())
    return NULL;

  try
  {
    //TODO right now we are copying only HEAD
    //this should be configurable
    const svn::Revision head (svn::Revision::HEAD);
    client.copy (m_src.c_str (), head, m_dest.c_str ());
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
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
