/*
 * ====================================================================
 * Copyright (c) 2002-2006 The RapidSvn Group.  All rights reserved.
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
#include "wx/app.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/m_check.hpp"

// app
#include "export_action.hpp"
#include "export_dlg.hpp"
#include "ids.hpp"
#include "utils.hpp"

ExportAction::ExportAction (wxWindow * parent)
  :Action (parent, _("Export"), GetBaseFlags ())
{
}

bool
ExportAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  const std::vector<svn::Path> & v = GetTargets ();

  svn::Path selectedPath ("");

  if (v.size () == 1)
  {
    selectedPath = v [0];
  }

  ExportDlg dlg (GetParent (), selectedPath);

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  m_data = dlg.GetData ();
  return true;
}

bool
ExportAction::Perform ()
{
  svn::Client client (GetContext ());

  TrimString(m_data.DestPath);
  UnixPath(m_data.DestPath);
  TrimString(m_data.SrcPath);
  TrimString(m_data.NativeEol);

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
  
  wxSetWorkingDirectory (m_data.DestPath);

  svn::Path srcPathUtf8 (PathUtf8 (m_data.SrcPath));
  svn::Path destPathUtf8 (PathUtf8 (m_data.DestPath));
  std::string nativeEolUtf8 (LocalToUtf8 (m_data.NativeEol));

  if (svn::SUPPORTS_EXTERNALS)
  {
//#if CHECK_SVN_SUPPORTS_EXTERNALS
    bool ignoreExternals = svn::SUPPORTS_EXTERNALS ? m_data.IgnoreExternals : false;

    client.doExport2 (srcPathUtf8.c_str (), 
                      destPathUtf8, 
                      revision,
                      m_data.Overwrite,
                      pegRevision,
                      ignoreExternals,
                      m_data.Recursive,
                      nativeEolUtf8.c_str ());
  }
  else
  {
    client.doExport (srcPathUtf8.c_str (), 
                     destPathUtf8, 
                     revision,
                     m_data.Overwrite);
  }
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
