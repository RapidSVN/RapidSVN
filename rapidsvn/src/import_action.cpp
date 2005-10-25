/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxWidgets
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
