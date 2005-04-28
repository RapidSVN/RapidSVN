/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
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
    std::string pathUtf8 (LocalToUtf8 (m_data.path));
    path = Utf8ToLocal  (
        GetPathAsTempFile(svn::Path (pathUtf8.c_str ()),
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
