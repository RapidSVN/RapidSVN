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
#include "move_action.hpp"
#include "destination_dlg.hpp"

MoveAction::MoveAction (wxWindow * parent, 
                                int kind) 
 : Action (parent, "", GetBaseFlags ()),
   m_kind (kind)
{
  const char * name;
  switch (kind)
  {
  case MOVE_MOVE:
    name = _("Move");
    break;
  case MOVE_COPY:
    name = _("Copy");
    break;
  case MOVE_RENAME:
    name = _("Rename");
    break;
  default:
    name = "";
    break;
  }

  SetName (name);
}

bool
MoveAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  // create flags for the dialog
  int flags = 0;
  switch (m_kind)
  {
  case MOVE_MOVE:
  case MOVE_RENAME:
    flags = DestinationDlg::WITH_FORCE;
    break;
  case MOVE_COPY:
    break;
  default: // invalid kind
    return false;
  }

  // create description for the dialog
  const char * descr;
  switch (m_kind)
  {
  case MOVE_COPY:
  case MOVE_MOVE:
    descr = _("Select destination:");
    break;
  case MOVE_RENAME:
    descr = _("Enter new name:");
    break;
  default:
    return false;
  }

  DestinationDlg dlg (GetParent (), GetName (),
                      descr, flags);

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  m_destination = dlg.GetDestination ();
  m_force = dlg.GetForce ();

  return true;
}

bool
MoveAction::Perform ()
{
  svn::Client client;

  svn::Path srcPath = GetTarget ();
  svn::Path destPath (m_destination.c_str ());
  svn::Revision unusedRevision;

  switch (m_kind)
  {
  case MOVE_COPY:
    client.copy (srcPath, unusedRevision, destPath);
    break;

  case MOVE_MOVE:
    client.move (srcPath, unusedRevision, destPath, m_force);
    break;

  case MOVE_RENAME:
    {
      std::string basename;
      std::string dirpath;
      srcPath.split (dirpath, basename);
      destPath = dirpath.c_str ();
      destPath.addComponent (m_destination.c_str ());

      client.move (srcPath, unusedRevision, destPath, m_force);
    }
    break;
  }
  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
