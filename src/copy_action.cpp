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


/**
 * right now we are accepting only target.
 */
CopyAction::CopyAction (wxWindow * parent)
  : Action (parent, actionWithSingleTarget)
{
}

bool
CopyAction::Prepare ()
{
  // nothing to do here
  return Action::Prepare ();
}

bool
CopyAction::Perform ()
{
  bool result = true;
  svn::Client client;
  SvnNotify notify (GetTracer ());
  client.notification (&notify);

  svn::Path src = GetTarget ();
  svn::Path dest = DestinationPath (src);

  if(!dest.isset ())
    return false;

  try
  {
    //TODO right now we are copying only HEAD
    //this should be configurable
    const svn::Revision head (svn::Revision::HEAD);
    client.copy (src, head, dest);
    GetTracer ()->Trace (_T("Copy successful"));
  }
  catch (svn::ClientException &e)
  {
    PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                     ACTION_EVENT);
    GetTracer ()->Trace (_T("Copy failed:"));
    GetTracer ()->Trace (e.description ());
    result = false;
  }

  return result;
}

/**
 * Returns the properly formatted destination file name.
 */
const svn::Path
CopyAction::DestinationPath (const svn::Path & src)
{
  wxString dest;
  wxFileName file (src.c_str ());
  wxDirDialog dialog (GetParent (), 
                      _T ("Select a directory to copy to"), 
                      GetPath ().c_str ());

  if (dialog.ShowModal () != wxID_OK)
    return _T ("");


  dest = dialog.GetPath ();
  //TODO replace textual constant with real const
  if(dest.Right (4) == ".svn")
  {
      wxMessageDialog dlg (GetParent (), 
                           _T ("This is an invalid directory."),
                           _T ("Error"), wxOK | wxICON_HAND);
      dlg.ShowModal ();

      return "";
  }

  //TODO clean this stuff up.
  dest = dest + file.GetPathSeparators ().Left (1) + file.GetFullName ();
  dest.Replace("\\", "/", true);

  return dest.c_str ();
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
