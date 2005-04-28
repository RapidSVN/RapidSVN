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
#include "ids.hpp"
#include "get_action.hpp"
#include "utils.hpp"

GetAction::GetAction (wxWindow * parent, const GetData & data)
  : Action (parent, _("Update"), GetBaseFlags ()),
    m_data (data)
{
}

bool
GetAction::Prepare ()
{
  return true;
}

bool
GetAction::Perform ()
{
  svn::Client client (GetContext ());

  wxSetWorkingDirectory (Utf8ToLocal (GetPath ().c_str ()));
  client.update (svn::Path (LocalToUtf8 (m_data.path)),
                 m_data.revision,
                 false);

  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
