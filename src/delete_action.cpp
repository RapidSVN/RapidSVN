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

// svncpp
#include "svncpp/exception.hpp"
#include "svncpp/client.hpp"

// app
#include "delete_action.hpp"
#include "delete_dlg.hpp"
#include "ids.hpp"
#include "svn_notify.hpp"

DeleteAction::DeleteAction (wxWindow * parent) 
  : Action (parent, _("Delete"), actionWithTargets)
{
}

bool
DeleteAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  DeleteDlg dlg (GetParent (), &Data);

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  return true;
}

bool
DeleteAction::Perform ()
{
  svn::Client client;
  SvnNotify notify (GetTracer ());
  client.notification (&notify);

  const std::vector<svn::Path> & v = GetTargets ();
  std::vector<svn::Path>::const_iterator it;

  for (it = v.begin (); it != v.end (); it++)
  {
    const svn::Path & path = *it;

    client.remove (path.c_str (), Data.Force);
  }

  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
