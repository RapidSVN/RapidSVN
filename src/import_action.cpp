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
#include "svncpp/exception.hpp"
#include "svncpp/client.hpp"

// app
#include "ids.hpp"
#include "import_dlg.hpp"
#include "tracer.hpp"
#include "import_action.hpp"
#include "svn_notify.hpp"

ImportAction::ImportAction (wxWindow * parent)
  :Action (parent, actionWithoutTarget)
{
}

bool
ImportAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  ImportDlg dlg (GetParent (), &m_data);

  if (dlg.ShowModal () != wxID_OK)
  {
    return false;
  }

  return true;
}

bool
ImportAction::Perform ()
{
  svn::Context context (m_data.User, m_data.Password);
  svn::Client client (&context);
  SvnNotify notify (GetTracer ());
  client.notification (&notify);
  const char *the_new_entry = NULL;
  bool result = true;

  // if new entry is empty, the_new_entry must be left NULL.
  if (!m_data.NewEntry.IsEmpty ())
    the_new_entry = m_data.NewEntry.c_str ();

  try
  {
    client.import (m_data.Path.c_str (), m_data.Repository.c_str (), the_new_entry,
                   m_data.LogMessage.c_str(), m_data.Recursive);
    GetTracer ()->Trace ("Import successful.");
  }
  catch (svn::ClientException &e)
  {
    PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, _(e.description ()), 
                     ACTION_EVENT);
    GetTracer ()->Trace ("Import failed:");
    GetTracer ()->Trace (e.description ());
    result = false;
  }

  return result;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
