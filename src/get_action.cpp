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
#include "ids.hpp"
#include "get_action.hpp"
#include "svn_notify.hpp"
#include "tracer.hpp"
#include "utils.hpp"

GetAction::GetAction (wxWindow * parent, const GetData & data)
  : Action (parent, _("Update"), actionWithoutTarget),
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
  SvnNotify notify (GetTracer ());
  client.notification (&notify);

  wxSetWorkingDirectory (GetPath ().c_str ());
  client.update (m_data.path.c_str (),
                 m_data.revision,
                 false);

  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
