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

// app
#include "import_dlg.hpp"
#include "import_action.hpp"
#include "utils.hpp"

ImportAction::ImportAction (wxWindow * parent)
  :Action (parent, _("Import"), GetBaseFlags ())
{
}

bool
ImportAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  ImportDlg dlg (GetParent ());

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  m_data = dlg.GetData ();

  return true;
}

bool
ImportAction::Perform ()
{
  svn::Client client (GetContext ());
  std::string PathUtf8 (LocalToUtf8(m_data.Path));
  std::string RepositoryUtf8 (LocalToUtf8(m_data.Repository));
  std::string LogMessageUtf8 (LocalToUtf8(m_data.LogMessage));
  client.import (svn::Path (PathUtf8), RepositoryUtf8.c_str (),
                 LogMessageUtf8.c_str (), m_data.Recursive);

  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
