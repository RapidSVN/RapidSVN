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
#include "wx/intl.h"
#include "wx/event.h"
#include "wx/button.h"
#include "wx/filesys.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/status_selection.hpp"
#include "svncpp/targets.hpp"

// app
#include "action_event.hpp"
#include "userresolve_action.hpp"
#include "ids.hpp"
#include "preferences.hpp"
#include "utils.hpp"

UserResolveAction::UserResolveAction(wxWindow * parent)
    : Action(parent, _("Resolve"))
{
}

bool
UserResolveAction::Prepare()
{
  // No dialog for UserResolve.
  return Action::Prepare();
}

bool
UserResolveAction::Perform()
{
  Preferences prefs;

  if (prefs.mergeTool.Length() == 0)
  {
    TraceError(_("No merge tool set in the preferences"));
    return false;
  }

  const svn::StatusVector & v = GetStatusVector();
  svn::StatusVector::const_iterator it;

  svn::Client client(GetContext());
  for (it = v.begin(); it != v.end(); it++)
  {
    const svn::Path & path = it->path();
    const svn::Entry & entry = it->entry();

    wxFileName resultPath = Utf8ToLocal(path.native().c_str());
    wxFileName minePath(resultPath.GetPath(), Utf8ToLocal(entry.conflictWrk()));
    wxFileName basePath(resultPath.GetPath(), Utf8ToLocal(entry.conflictOld()));
    wxFileName theirsPath(resultPath.GetPath(), Utf8ToLocal(entry.conflictNew()));

    if ((!resultPath.FileExists()) || (!minePath.FileExists()) ||
        (!basePath.FileExists()) ||  (!theirsPath.FileExists()))
    {
      continue;
    }

    // prepare command line to execute
    wxString args(prefs.mergeToolArgs);

    TrimString(args);

    if (args.Length() == 0)
      args.Printf(wxT("\"%s\" \"%s\" \"%s\" \"%s\""), basePath.GetFullPath().c_str(),
                  theirsPath.GetFullPath().c_str(),
                  minePath.GetFullPath().c_str(),
                  resultPath.GetFullPath().c_str());
		else
    {
      args.Replace(wxT("%1"), basePath.GetFullPath().c_str(), true);
      args.Replace(wxT("%2"), theirsPath.GetFullPath().c_str(), true);
      args.Replace(wxT("%3"), minePath.GetFullPath().c_str(), true);
      args.Replace(wxT("%4"), resultPath.GetFullPath().c_str(), true);
    }

    wxString cmd(prefs.mergeTool + wxT(" ") + args);

    wxString msg;
    msg.Printf(_("Execute merge tool: %s"), cmd.c_str());
    Trace(msg);

    ActionEvent::Post(GetParent(), TOKEN_CMD_MERGE, cmd);
  }

  return true;
}

bool
UserResolveAction::CheckStatusSel(const svn::StatusSel & statusSel)
{
  // well, we allow only a single+versioned+local file
  if (!statusSel.hasVersioned())
    return false;

  if (statusSel.size() != 1)
    return false;

  if (!statusSel.hasLocal())
    return false;

  if (!statusSel.hasFiles())
    return false;

  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
