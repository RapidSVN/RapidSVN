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
#include "svncpp/exception.hpp"

// app
#include "ids.hpp"
#include "tracer.hpp"
#include "revert_action.hpp"
#include "svn_notify.hpp"

RevertAction::RevertAction (wxWindow * parent)
  : Action (parent, actionWithTargets)
{
}

bool
RevertAction::Prepare ()
{
  if (!Action::Prepare ())
  {
    return false;
  }

  wxMessageDialog dlg (GetParent (),
                       _("Do you want to revert local changes?"),
                       _("Revert"), wxYES_NO | wxICON_QUESTION);

  if (dlg.ShowModal () != wxID_YES)
  {
    return false;
  }

  return true;
}

bool
RevertAction::Perform ()
{
  svn::Client client;
  SvnNotify notify (GetTracer ());
  client.notification (&notify);
  bool result = true;

  const std::vector<svn::Path> & v = GetTargets ();
  std::vector<svn::Path>::const_iterator it;

  for (it = v.begin (); it != v.end (); it++)
  {
    const svn::Path & path = *it;

    try
    {
      client.revert (path.c_str (), false);
    }
    catch (svn::ClientException &e)
    {
      PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, e.description (), 
                       ACTION_EVENT);
      result = false;
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
