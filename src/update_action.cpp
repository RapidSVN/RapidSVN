/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
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
#include "svncpp/exception.hpp"
#include "svncpp/client.hpp"
#include "svncpp/path.hpp"
#include "svncpp/targets.hpp"

// app
//REMOVE#include "include.hpp"
//REMOVE#include "rapidsvn_app.hpp"
#include "ids.hpp"
#include "update_action.hpp"
#include "update_dlg.hpp"
#include "svn_notify.hpp"
#include "tracer.hpp"
#include "utils.hpp"

UpdateAction::UpdateAction (wxWindow * parent, const svn::Targets & targets,
                            wxString & path, Tracer * tr, bool owns)
  : Action (parent, tr, owns), m_targets (targets), m_path (path)
{
}

bool
UpdateAction::Prepare ()
{
  UpdateDlg dlg (GetParent ());

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  m_data = dlg.GetData ();
  return true;
}

bool
UpdateAction::Perform ()
{
  svn::Context context (m_data.User, m_data.Password);
  svn::Client client (&context);
  SvnNotify notify (GetTracer ());
  client.notification (&notify);

  svn::Revision revision (svn::Revision::HEAD);
  // Did the user request a specific revision?:
  if (!m_data.UseLatest)
  {
    TrimString(m_data.Revision);
    if (!m_data.Revision.IsEmpty ())
    {
      svn_revnum_t revnum;
      m_data.Revision.ToLong(&revnum, 10);  // If this fails, revnum is unchanged.
      revision = svn::Revision (revnum);
    }
  }

  bool result = true;
  const std::vector<svn::Path> & v = m_targets.targets ();
  std::vector<svn::Path>::const_iterator it;
  wxSetWorkingDirectory (m_path);
  for (it = v.begin(); it != v.end(); it++)
  {
    const svn::Path & path = *it;

    try
    {
      client.update (path.c_str (), revision, true);
    }
    catch (svn::ClientException &e)
    {
      result = false;
      PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                       ACTION_EVENT);
      GetTracer ()->Trace ("Update failed:");
      GetTracer ()->Trace (e.description ());
    }
  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);
  return result;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
