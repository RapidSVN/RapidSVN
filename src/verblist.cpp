/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
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
/*
 * VerbList - Windows implementation
 */

// stl
#include <vector>

// wx
#include "wx/wx.h"
#include "wx/filename.h"

#ifdef __WXMSW__
#include "wx/msw/registry.h"
#include <windows.h>
#endif

// app
#include "verblist.hpp"


#ifndef __WXMSW__
/**********************************************************************
 * NON WINDOWS IMPLEMENTATION OF VerbList
 *********************************************************************/

VerbList::VerbList
{
}


VerbList::~VerbList 
{
}


void
VerbList::InitFromDocument(const wxString & documentPath, bool isAFolder)
{}


size_t
VerbList::GetCount () const
{
  return 0;
}


const wxString &
VerbList::GetName (size_t index) const
{
  return "";
}


void
VerbList::Launch (size_t index) const
{
}



#else
/**********************************************************************
 * WINDOWS IMPLEMENTATION OF VerbList
 *********************************************************************/


/**
 * This class stores one verb
 */
struct Verb
{
  typedef std::vector<Verb> LIST;
  bool m_uses_dde;
  wxString m_name;
  wxString m_command;
  wxString m_dde_command;
  wxString m_dde_app;
  wxString m_dde_topic;

  Verb ();

  virtual ~ Verb () {};

  /**
   * Read verb information from the given verb of the given key in the
   * registry
   *
   * If the registry entry is bad, this may result in a verb with only a
   * name (no commands), but that'll still be usable in some contexts.
   *
   * @retval false if error has happened
   */
  bool
  ReadFromRegistry(const wxRegKey & base, const wxString & verb_name);


  /**
   * Lauch the verb on the given document
   */
  void Launch(const wxString & document_path) const;
};


/**
 * Private data members of VerbList
 */
struct VerbList::Data
{
  wxFileName document_path;
  wxString document_type_name;
  Verb::LIST verb_list;
};


//*********************************************************************

Verb::Verb () : m_uses_dde (false)
{
}

bool
Verb::ReadFromRegistry (const wxRegKey & base, const wxString & verb_name)
{
  // Store descriptive verb name 
  m_name = verb_name;

  wxString command_key_name (verb_name + wxT("\\command"));

  // Read command key
  if (base.HasValue (command_key_name))
    return false;

  wxRegKey command_key (base, command_key_name);

  command_key.QueryValue (NULL, m_command);

  // Attempt to read ddeexec key
  const wxString dde_command_key_name (verb_name + wxT("\\ddeexec"));
  wxRegKey dde_command_key (base, dde_command_key_name);;

  if (dde_command_key.HasValue (NULL))
    dde_command_key.QueryValue (NULL, m_dde_command);

  wxRegKey dde_topic_key (dde_command_key, wxT("Topic"));
  if (dde_topic_key.HasValue (NULL))
    dde_topic_key.QueryValue (NULL, m_dde_topic);

  wxRegKey dde_app_key (dde_command_key, wxT("Application"));
  if (dde_app_key.HasValue (NULL))
  {
    dde_app_key.QueryValue (NULL, m_dde_app);

    m_uses_dde = true;
  }

  return true;
}

void 
Verb::Launch (const wxString  & document_path) const
{
  ShellExecute (NULL, m_name.c_str (), 
                document_path.c_str (), NULL, NULL, SW_SHOWNORMAL);
  // TODO: error handling
}

/****************************************************************************/

VerbList::VerbList()
{
  m = new Data;
}

VerbList::~VerbList()
{
  delete m;
}

void VerbList::InitFromDocument (const wxString & document_path, bool isAFolder)
{

  // Algorithm:
  // look up extension:          HKEY_CLASSES_ROOT\.ext
  // find ProgID of extension:   HKEY_CLASSES_ROOT\.ext\(Standard) = progid
  // find name of file type:     HKEY_CLASSES_ROOT\progid\(Standard) = file type name
  // find verbs:                 HKEY_CLASSES_ROOT\progid\shell\verb1
  // find command for each verb: HKEY_CLASSES_ROOT\progid\shell\verb1\command\(Standard) = command path
  //
  // Expand wildcards in command: "%1" and %1, and %SystemRoot% and other
  // environment variables.

  m->document_path = document_path;

  // Make sure list is empty
  m->verb_list.clear ();


  wxString progid_key_name;

  if (isAFolder)
    progid_key_name = wxT("Folder");
  else
  {
    // Find document extension
    if (!m->document_path.HasExt ())
      // Nothing to go on; we cannot provide an editor
      return;

    // Get progid of extension
    wxString extension_key_name (wxT(".") + m->document_path.GetExt ());
    wxRegKey ext_key (wxRegKey::HKCR, extension_key_name);
    if (!ext_key.Open ())
      return;
    if (!ext_key.QueryValue (NULL, progid_key_name))
      return;
  }

  // Get information on progid (name, verbs)
  wxRegKey progid_key (wxRegKey::HKCR, progid_key_name);
  if (progid_key.IsEmpty ())
    return;

  // Get long name of progid (ignored if not found)
  if (progid_key.HasValue (NULL))
    progid_key.QueryValue (NULL, m->document_type_name);

  // Get verbs of progid
  wxRegKey verb_base_key (progid_key, wxT("shell"));
  if (verb_base_key.IsEmpty ())
    return;

  // Get name of default verb (ignored if not found)

  wxString default_verb_name (wxT(""));
  if (verb_base_key.HasValue (NULL))
    verb_base_key.QueryValue (NULL, default_verb_name);

  wxString verb_key_name;
  long index;
  bool ok = verb_base_key.GetFirstKey (verb_key_name, index);

  while (ok)
  {
    Verb verb;
    if (verb.ReadFromRegistry (verb_base_key, verb_key_name))
    {
      // Place first if default
      if (verb_key_name == default_verb_name)
        m->verb_list.insert (m->verb_list.begin (), verb);
      else
        m->verb_list.push_back (verb);
    }

    // select next key
    ok = verb_base_key.GetNextKey (verb_key_name, index);
  }
}

size_t 
VerbList::GetCount () const
{
  return m->verb_list.size ();
}

const wxString &
VerbList::GetName (size_t index) const
{
  return m->verb_list[index].m_name;
}

void 
VerbList::Launch (size_t index) const
{
  m->verb_list[index].Launch (m->document_path.GetFullPath ());
}


#endif

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
