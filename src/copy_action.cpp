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
#include "wx/wx.h"
#include "wx/filename.h"

// svncpp
#include "svncpp/exception.hpp"
#include "svncpp/client.hpp"

// app
#include "copy_action.hpp"
#include "ids.hpp"
#include "svn_notify.hpp"


CopyAction::CopyAction (wxWindow * parent, Tracer * tr, 
                        const wxString & path,
                        const svn::Targets & targets )
  : Action (parent, tr, false), m_path (path), m_targets(targets)
{
}

bool
CopyAction::Prepare ()
{
  // nothing to do here
  return true;
}

bool
CopyAction::Perform ()
{
  bool result = true;
  svn::Client client;
  SvnNotify notify (GetTracer ());
  client.notification (&notify);

  m_src = m_targets.targets ()[0].c_str ();
  m_dest = DestinationPath (m_src);

  if(m_dest.IsEmpty ())
    return false;

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
    result = false;
  }

  return result;
}

/**
 * Returns the properly formatted destination file name.
 */
wxString
CopyAction::DestinationPath (wxString src)
{
  wxString dest;
  wxFileName file (src);
  wxDirDialog dialog (GetParent (), 
                      _T ("Select a directory to copy to"), 
                      m_path);

  if (dialog.ShowModal () != wxID_OK)
    return _T ("");


  dest = dialog.GetPath ();
  if(dest.Right (4) == ".svn")
  {
      wxMessageDialog dlg (GetParent (), 
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
