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
#include "svncpp/status.hpp"

// app
#include "switch_action.hpp"
#include "update_dlg.hpp"
#include "utils.hpp"

SwitchAction::SwitchAction (wxWindow * parent) 
 : Action (parent, _("Switch"), actionWithSingleTarget)
{
}

bool
SwitchAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  // first try to get the URL for the target
  wxString url = "";
  {
    svn::Path path = GetTarget ();
    svn::Client client (GetContext ());
    svn::Status status (client.singleStatus (path.c_str ()));
    url = status.entry ().url ();
  }

  // create flags for the dialog
  int flags = UpdateDlg::WITH_URL;

  UpdateDlg dlg (GetParent (), _("Switch"), flags,
                 true);
  dlg.GetData ().url = url;

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  m_data = dlg.GetData ();

  return true;
}

bool
SwitchAction::Perform ()
{
  svn::Path path = GetTarget ();
  const char * url = m_data.url.c_str ();
  svn::Revision revision (svn::Revision::HEAD);

  if (!m_data.useLatest)
  {
    svn_revnum_t revnum;
    TrimString (m_data.revision);
    m_data.revision.ToLong (&revnum);
    revision = revnum;
  }

  svn::Client client (GetContext ());
  client.doSwitch (path, url, revision,
                   m_data.recursive);

  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
