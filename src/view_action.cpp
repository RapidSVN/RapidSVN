/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
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
#include "svncpp/client.hpp"
#include "svncpp/path.hpp"

// app
#include "view_action.hpp"
#include "tviewer.hpp"
#include "utils.hpp"

ViewAction::ViewAction (wxWindow * parent, const GetData & data)
  : Action (parent, _("View"), GetBaseFlags ()),
    m_data (data)
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
  wxSetWorkingDirectory (GetPath ().c_str ());

  svn::Path path (m_data.path.c_str ());
  svn::Client client (GetContext ());
  std::string text = client.cat (path, m_data.revision);

  wxString title;
  title.Printf (_("View %s Revision %ld"),
                path.c_str (),
                m_data.revision.revnum ());
  TextViewer * viewer = new TextViewer (title);
  viewer->SetText (text.c_str ());
  viewer->Show (true);

  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
