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
#include "wx/filefn.h"

// svncpp
#include "svncpp/exception.hpp"
#include "svncpp/utils.hpp"

// app
#include "cleanup_action.hpp"
#include "ids.hpp"
#include "tracer.hpp"
#include "utils.hpp"

CleanupAction::CleanupAction (wxWindow * parent, svn::Path & path, 
                              Tracer * tr, bool own)
  : Action (parent, tr, own), m_path (path)
{
}

bool
CleanupAction::Prepare ()
{
  return true;
}

bool
CleanupAction::Perform ()
{
  bool result = false;
  try
  {
    wxSetWorkingDirectory (m_path.c_str ());
    svn::Utils::cleanup (m_path);
    Trace (_T ("Cleanup of working directory finished"));
    result = true;
  }
  catch (svn::ClientException &e)
  {
    PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, wxT (e.description ()), 
                     ACTION_EVENT);
    GetTracer ()->Trace ("Cleanup failed:");
    GetTracer ()->Trace (e.description ());
  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);
  return result;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
