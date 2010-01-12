/*
 * ====================================================================
 * Copyright (c) 2002-2010 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wx
#include "wx/wx.h"
#include "wx/filename.h"
#include "wx/intl.h"

// svncpp
#include "svncpp/url.hpp"
#include "svncpp/status_selection.hpp"


// app
#include "external_program_action.hpp"
#include "action_event.hpp"
#include "ids.hpp"
#include "preferences.hpp"
#include "utils.hpp"
#include "verblist.hpp"

ExternalProgramAction::ExternalProgramAction(
  wxWindow * parent, long verb_id, bool treat_as_folder)
    : Action(parent, _("Execute"), UPDATE_LATER),
    m_verb_id(verb_id),
    m_treat_as_folder(treat_as_folder),
    m_parent(parent)
{
}

bool
ExternalProgramAction::Prepare()
{
  return Action::Prepare();
}

bool
ExternalProgramAction::Perform()
{
  VerbList verb_list;
//  wxBusyCursor busy_cursor;

  // The actual target
  svn::Path path = GetTarget();
  if (path.isUrl())
    path = GetPathAsTempFile(path);

  wxString target_str = Utf8ToLocal(path.native().c_str());
  wxFileName target = target_str;

  // The target we'll pass to the external program
  wxString target_document = target.GetFullPath();
  Preferences prefs;

  if (m_treat_as_folder)
  {
    // Assuming target is a folder: use only folder part of filename if target
    // is really a file
    if (!wxDirExists(target_document))
      target_document = target.GetPath();
  }

  // Get verbs from the OS
  verb_list.InitFromDocument(target_document, m_treat_as_folder);

  // An explicit verb was chose that is not available
  if ((m_verb_id >= 0) && ((size_t)m_verb_id > verb_list.GetCount()))
  {
    // TODO: Trace this?
    return false;
  }

  if (m_treat_as_folder)
  {
    if (verb_list.GetCount() && !((m_verb_id == -1)
                                  && prefs.explorerAlways))
    {
      verb_list.Launch((m_verb_id == -1 ? 0 : m_verb_id));
    }
    else
    {
      wxString args(prefs.explorerArgs);
      TrimString(args);

      if (args.Length() == 0)
        args = wxT("\"") + target_document + wxT("\"");
      else
        args.Replace(wxT("%1"), target_document, true);

      wxString cmd(prefs.explorer + wxT(" ") + args);

      wxString msg;
      msg.Printf(_("Execute file explorer: %s"), cmd.c_str());
      Trace(msg);

      ActionEvent::Post(m_parent, TOKEN_CMD, cmd);
    }
  }
  else
  {
    if (verb_list.GetCount() && !((m_verb_id == -1)
                                  && prefs.editorAlways))
    {
      verb_list.Launch((m_verb_id == -1 ? 0 : m_verb_id));
    }
    else
    {
      wxString args(prefs.editorArgs);
      TrimString(args);

      if (args.Length() == 0)
        args = wxT("\"") + target_document + wxT("\"");
      else
        args.Replace(wxT("%1"), target_document, true);

      wxString cmd(prefs.editor + wxT(" ") + args);

      wxString msg;
      msg.Printf(_("Execute editor: %s"), cmd.c_str());
      Trace(msg);

      ActionEvent::Post(m_parent, TOKEN_CMD, cmd);
    }
  }

  return true;
}

bool
ExternalProgramAction::CheckStatusSel(const svn::StatusSel & statusSel)
{
  // works with all kinds of files and dirs,
  // but only ONE is allowed
  if (1 != statusSel.size())
    return false;

  // directories are allowed only local
  if (statusSel.hasUrl() && statusSel.hasDirs())
    return false;

  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
