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
#include "checkout_action.hpp"
#include "checkout_dlg.hpp"
#include "ids.hpp"
#include "utils.hpp"

CheckoutAction::CheckoutAction (wxWindow * parent)
  : Action (parent, _("Checkout"), GetBaseFlags ())
{
}

bool
CheckoutAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  const std::vector<svn::Path> & v = GetTargets ();

  svn::Path selectedUrl ("");

  if (v.size () == 1)
  {
    svn::Path selectedPath (v [0]);

    if (selectedPath.isUrl ())
    {
      selectedUrl = selectedPath;
    }
  }

  CheckoutDlg dlg (GetParent (), selectedUrl);

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  m_data = dlg.GetData ();
  return true;
}

bool
CheckoutAction::Perform ()
{
  svn::Client client (GetContext ());

  TrimString(m_data.DestFolder);
  UnixPath(m_data.DestFolder);
  TrimString(m_data.RepUrl);

  long revnum = -1;
  svn::Revision revision (svn::Revision::HEAD);
  svn::Revision pegRevision;

  // Did the user request a specific revision?:
  if (!m_data.UseLatest)
  {
    TrimString(m_data.Revision);
    if (!m_data.Revision.IsEmpty ())
    {
      m_data.Revision.ToLong(&revnum, 10);  // If this fails, revnum is unchanged.
    }
    revision = svn::Revision(revnum);
  }

  revnum = -1;
  // Did the user request a specific peg revision?:
  if (svn::SUPPORTS_PEG && !m_data.NotSpecified)
  {
    TrimString(m_data.PegRevision);
    if (!m_data.PegRevision.IsEmpty ())
    {
      m_data.PegRevision.ToLong(&revnum, 10);  // If this fails, revnum is unchanged.
    }
    if (revnum != -1)
      pegRevision = svn::Revision(revnum);
  }
  
  wxSetWorkingDirectory (m_data.DestFolder);

  svn::Path repUrlUtf8 (PathUtf8 (m_data.RepUrl));
  svn::Path destFolderUtf8 (PathUtf8 (m_data.DestFolder));

  bool ignoreExternals = svn::SUPPORTS_EXTERNALS ? m_data.IgnoreExternals : false;

  client.checkout (repUrlUtf8.c_str (), 
                   destFolderUtf8, 
                   revision,
                   m_data.Recursive,
                   ignoreExternals,
                   pegRevision);

  // now post event to add bookmark to bookmarks
  if (m_data.Bookmarks)
  {
    wxCommandEvent event = CreateActionEvent (TOKEN_ADD_BOOKMARK);
    event.SetString (m_data.DestFolder);

    PostEvent (event);
  }
 
  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
