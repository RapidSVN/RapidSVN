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
#include "svncpp/client.hpp"

// app
#include "copymove_action.hpp"
#include "destination_dlg.hpp"
#include "ids.hpp"
#include "svn_notify.hpp"

CopyMoveAction::CopyMoveAction (wxWindow * parent, 
                                bool copy) 
 : Action (parent, "", actionWithSingleTarget),
   m_copy (copy)
{
  if (copy)
  {
    SetName (_("Copy"));
  }
  else
  {
    SetName (_("Move"));
  }
}

bool
CopyMoveAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  DestinationDlg dlg (GetParent (), GetName (),
                      _("Select destination:"),
                      m_destination);

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  m_destination = dlg.GetDestination ();

  return true;
}

bool
CopyMoveAction::Perform ()
{
  svn::Client client;
  SvnNotify notify (GetTracer ());
  client.notification (&notify);

  svn::Path srcPath = GetTarget ();
  svn::Path destPath (m_destination.c_str ());
  svn::Revision unusedRevision;

  if (m_copy)
  {
    client.copy (srcPath, unusedRevision, destPath);
  }
  else
  {
    client.move (srcPath, unusedRevision, destPath, false);
  }
  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
