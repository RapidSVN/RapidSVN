/*
 * ====================================================================
 * Copyright (c) 2002-2009 The RapidSvn Group.  All rights reserved.
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

// wxWidgets
#include "wx/wx.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/path.hpp"
#include "svncpp/status_selection.hpp"

// app
#include "action_event.hpp"
#include "exceptions.hpp"
#include "get_data.hpp"
#include "ids.hpp"
#include "preferences.hpp"
#include "utils.hpp"
#include "view_action.hpp"

struct ViewAction::Data
{
private:
  Action * action;

public:
  bool edit;
  GetData data;
  wxWindow * parent;

  Data(Action * action_)
      : action(action_)
  {
  }


  Data(Action * action_, GetData & data_)
      : action(action_), data(data_)
  {
  }
};

ViewAction::ViewAction(wxWindow * parent,
                       const GetData & data)
    : Action(parent, _("View"), DONT_UPDATE)
{
  m = new Data(this);
  m->parent = parent;
  m->data = data;
  m->edit = false;
}


ViewAction::ViewAction(wxWindow * parent)
    : Action(parent, _("Edit"), UPDATE_LATER)
{
  m = new Data(this);
  m->parent = parent;
  m->edit = true;
}

bool
ViewAction::Prepare()
{
  return true;
}

bool
ViewAction::Perform()
{
  Preferences prefs;

  if (prefs.editor.Length() == 0)
    throw RapidSvnEx(
      _("The Editor is not configured. Please check Edit->Preferences>Programs"));

  wxString path;

  if (m->edit)
    path = Utf8ToLocal(GetTarget().native().c_str());
  else
  {
    svn::Path pathUtf8(PathUtf8(m->data.path));
    path = Utf8ToLocal(
      GetPathAsTempFile(pathUtf8.c_str(),
                        m->data.revision).native().c_str());
  }

  wxString args(prefs.editorArgs);
  TrimString(args);

  if (args.Length() == 0)
    args = wxT("\"") + path + wxT("\"");
  else
    args.Replace(wxT("%1"), path, true);

  wxString cmd(prefs.editor + wxT(" ") + args);

  wxString msg;
  msg.Printf(_("Execute editor: %s"), cmd.c_str());
  Trace(msg);

  ActionEvent::Post(m->parent, TOKEN_CMD_VIEW, cmd);

  return true;
}

bool
ViewAction::CheckStatusSel(const svn::StatusSel & statusSel)
{
  if (1 != statusSel.size())
    return false;

  if (statusSel.hasDirs())
    return false;

  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
