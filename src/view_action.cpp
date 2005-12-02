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
#include "svncpp/path.hpp"

// app
#include "exceptions.hpp"
#include "preferences.hpp"
#include "utils.hpp"
#include "view_action.hpp"

ViewAction::ViewAction (wxWindow * parent,
                        const GetData & data)
  : Action (parent, _("View"), GetViewFlags ()),
    m_edit (false), m_data (data)
{
}


ViewAction::ViewAction (wxWindow * parent)
  : Action (parent, _("Edit"), GetEditFlags ()),
    m_edit (true)
{

}

bool
ViewAction::Prepare ()
{
  return true;
}

bool
ViewAction::Perform ()
{
  Preferences prefs;

  if (prefs.editor.Length () == 0)
    throw RapidSvnEx (
      _("The Editor is not configured. Please check Edit->Preferences>Programs"));

  wxString path;

  if (m_edit)
    path = Utf8ToLocal (GetTarget ().c_str ());
  else
  {
    svn::Path pathUtf8 (PathUtf8 (m_data.path));
    path = Utf8ToLocal  (
        GetPathAsTempFile(pathUtf8.c_str (),
            m_data.revision).c_str ());
  }

  wxString args (prefs.editorArgs);
  TrimString (args);

  if (args.Length () == 0)
    args = wxT("\"") + path + wxT("\"");
  else
    args.Replace (wxT("%1"), path, true);

  wxString cmd (prefs.editor + wxT(" ") + args);
  wxExecute (cmd);

  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
