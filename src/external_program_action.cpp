/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
// svncpp
#include "svncpp/url.hpp"

// wx
#include "wx/wx.h"
#include "wx/filename.h"
#include "wx/intl.h"

// app
#include "external_program_action.hpp"
#include "preferences.hpp"
#include "verblist.hpp"

ExternalProgramAction::ExternalProgramAction (wxWindow * parent, long verb_id, 
  bool treat_as_folder) : Action (parent, _("Execute"), GetBaseFlags ()), 
  m_verb_id (verb_id), m_treat_as_folder (treat_as_folder)
{
}

bool
ExternalProgramAction::Prepare ()
{
  return Action::Prepare ();
}

bool
ExternalProgramAction::Perform ()
{
  VerbList verb_list;
  wxBusyCursor busy_cursor;

  // The actual target
  svn::Path path = GetTarget ();
  if (svn::Url::isValid (path.c_str ()))
  {
    path = GetPathAsTempFile(path);
  }
  
  wxString target_str = path.c_str ();
  wxFileName target = target_str;

  // The target we'll pass to the external program
  wxString target_document = target.GetFullPath ();
  Preferences prefs;

  if (m_treat_as_folder)
  {
    // Assuming target is a folder: use only folder part of filename if target
    // is really a file
    if (!wxDirExists(target_document))
      target_document = target.GetPath();
  }

  // Get verbs from the OS
  verb_list.InitFromDocument (target_document);

  // An explicit verb was chose that is not available
  if ((m_verb_id >= 0) && ((size_t)m_verb_id > verb_list.GetCount()))
  {
    // TODO: Trace this?
    return false;
  }

  if (m_treat_as_folder)
  {
    if (verb_list.GetCount () && !((m_verb_id == -1) 
      && prefs.explorerAlways))
    {
      verb_list.Launch ((m_verb_id == -1 ? 0 : m_verb_id));      
    }
    else
    {
      wxString argv = prefs.explorer + " \"" + target_document + "\"";
    
      wxExecute (argv);
    }
  }
  else
  {
    if (verb_list.GetCount () && !((m_verb_id == -1) 
      && prefs.editorAlways))
    {
      verb_list.Launch ((m_verb_id == -1 ? 0 : m_verb_id));      
    }
    else
    {
      wxString argv = prefs.editor + " \"" + target_document + "\"";
      
      wxExecute (argv);
    }
  }

  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
