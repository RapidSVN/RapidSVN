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
// app
#include "external_program_action.hpp"
#include "preferences_dlg.hpp"
#include "verblist.hpp"

ExternalProgramAction::ExternalProgramAction (wxWindow * parent, long verb_id, 
  bool treat_as_folder) : Action (parent, actionWithSingleTarget), 
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
  wxFileName target = GetTarget ().c_str ();

  // The target we'll pass to the external program
  wxString target_document = target.GetFullPath ();

  if (m_treat_as_folder)
  {
    // Assuming target is a folder: use only folder part of filename if target
    // is really a file
    if (!wxDirExists(target_document))
      target_document = target.GetPath();
  }

  // Get verbs from the OS
  try
  {
    verb_list.InitFromDocument (target_document);
  }
  catch (std::exception)
  {
    // Failed assembling verbs.
    // TODO: Trace this?
  }

  // An explicit verb was chose that is not available
  if ((m_verb_id >= 0) && ((size_t)m_verb_id > verb_list.GetCount()))
  {
    // TODO: Trace this?
    return false;
  }

  if (m_treat_as_folder)
  {
    if (verb_list.GetCount () && !((m_verb_id == -1) 
      && PreferencesDlg::Data.m_standard_file_explorer_always))
    {
      verb_list.Launch ((m_verb_id == -1 ? 0 : m_verb_id));      
    }
    else
    {
      // HACK: Here we're assuming the child process doesn't write to the
      // strings
      wxChar *argv[3] = { 
        (wxChar *)PreferencesDlg::Data.m_standard_file_explorer.c_str (),
        (wxChar *)target_document.c_str (),
        NULL
      };
    
      wxExecute (argv);
    }
  }
  else
  {
    if (verb_list.GetCount () && !((m_verb_id == -1) 
      && PreferencesDlg::Data.m_standard_editor_always))
    {
      verb_list.Launch ((m_verb_id == -1 ? 0 : m_verb_id));      
    }
    else
    {
      // HACK: Here we're assuming the child process doesn't write to the
      // strings
      wxChar *argv[3] = { 
        (wxChar *)PreferencesDlg::Data.m_standard_editor.c_str (),
        (wxChar *)target_document.c_str (),
        NULL
      };
      
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
