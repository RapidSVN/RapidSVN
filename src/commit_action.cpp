/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
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
#include "svncpp/pool.hpp"
#include "svncpp/targets.hpp"

// app
#include "commit_action.hpp"
#include "commit_dlg.hpp"
#include "ids.hpp"
#include "utils.hpp"

CommitAction::CommitAction (wxWindow * parent)
  : Action (parent, _("Commit"), GetBaseFlags ())
{
}

bool
CommitAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  CommitDlg dlg(GetParent ());

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  m_recursive = dlg.GetRecursive ();
  m_message = dlg.GetMessage ();
  
  return true;
}

bool
CommitAction::Perform ()
{
  svn::Client client (GetContext ());

  const svn::Targets & targets = GetTargets ();

  std::string messageUtf8 (LocalToUtf8 (m_message));

  svn::Pool pool;
  long revision = 
    client.commit (targets.array (pool), messageUtf8.c_str (), 
                   m_recursive);
  wxString str;

  str = wxString::Format (wxT("%s %" SVN_REVNUM_T_FMT "."),
                             _("Committed revision"), revision);
  Trace (str);

  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
