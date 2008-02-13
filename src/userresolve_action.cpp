/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
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
#include "wx/intl.h"
#include "wx/event.h"
#include "wx/button.h"
#include "wx/filesys.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/targets.hpp"

// app
#include "action_event.hpp"
#include "userresolve_action.hpp"
#include "ids.hpp"
#include "preferences.hpp"
#include "utils.hpp"

UserResolveAction::UserResolveAction (wxWindow * parent)
  : Action (parent, _("Resolve"))
{
}

bool
UserResolveAction::Prepare ()
{
  // No dialog for UserResolve.
  return Action::Prepare ();
}

bool
UserResolveAction::Perform ()
{
  Preferences prefs;

  if (prefs.mergeTool.Length () == 0)
  {
    TraceError (_("No merge tool set in the preferences"));
    return false;
  }

  const std::vector<svn::Path> v = GetTargets ();
  std::vector<svn::Path>::const_iterator it;

  svn::Client client (GetContext ());
  for (it = v.begin (); it != v.end (); it++)
  {
    const svn::Path & path = *it;
    
    wxString resultPath = Utf8ToLocal (path.native ().c_str ());
    wxString minePath   = resultPath + _(".mine");

    if (wxFileExists (resultPath) && wxFileExists (minePath))
    {
      wxString search = resultPath + _(".r*");

      wxFileSystem fileSystem;

      wxString r1 = fileSystem.FindFirst (search, wxFILE);
      wxString r2 = fileSystem.FindNext ();

      if (!r1.IsEmpty () && !r2.IsEmpty ())
      {
        long r1val;
        long r2val;
        
        if (r1.AfterLast ('r').ToLong (&r1val) && r2.AfterLast ('r').ToLong (&r2val))
        {
          wxString basePath;
          wxString theirsPath;

          if (r1val < r2val)
          {
            basePath   = r1;
            theirsPath = r2;
          }
          else
          {
            basePath   = r2;
            theirsPath = r1;
          }

          // prepare command line to execute
          wxString args (prefs.mergeToolArgs);

          TrimString (args);

          if (args.Length () == 0)
            args.Printf (wxT("\"%s\" \"%s\" \"%s\" \"%s\""), basePath.c_str (), 
                         theirsPath.c_str (),
                         minePath.c_str (),
                         resultPath.c_str ());
          else
          {
            args.Replace (wxT("%1"), basePath.c_str (), true);
            args.Replace (wxT("%2"), theirsPath.c_str (), true);
            args.Replace (wxT("%3"), minePath.c_str (), true);
            args.Replace (wxT("%4"), resultPath.c_str (), true);
          }

          wxString cmd (prefs.mergeTool + wxT(" ") + args);

          wxString msg;
          msg.Printf (_("Execute merge tool: %s"), cmd.c_str ());
          Trace (msg);
        
          ActionEvent::Post (GetParent (), TOKEN_CMD_MERGE, cmd);
        }
      }
    }
  }

  return true;
}

bool
UserResolveAction::CheckStatusSel (const svn::StatusSel & statusSel)
{
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
