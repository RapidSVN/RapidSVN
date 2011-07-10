/*
 * ====================================================================
 * Copyright (c) 2002-2011 The RapidSVN Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.
 * If not, see <http://www.gnu.org/licenses/>.
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
#include "svncpp/vector_wrapper.hpp"

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

VerbList::VerbList()
{
}


VerbList::~VerbList()
{
}


void
VerbList::InitFromDocument(const wxString & /*documentPath*/, bool /*isAFolder*/)
{}


size_t
VerbList::GetCount() const
{
  return 0;
}


const wxString &
VerbList::GetName(size_t /*index*/) const
{
  return wxT("");
}


void
VerbList::Launch(size_t /*index*/) const
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
  bool m_usesDde;
  wxString m_name;
  wxString m_command;
  wxString m_ddeCommand;
  wxString m_ddeApplication;
  wxString m_ddeTopic;

  Verb();

  virtual ~ Verb() {};

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
  ReadFromRegistry(const wxRegKey & base, const wxString & verbName);


  /**
   * Lauch the verb on the given document
   */
  void Launch(const wxString & documentPath) const;
};


/**
 * Private data members of VerbList
 */
struct VerbList::Data
{
  wxFileName documentPath;
  wxString document_type_name;
  Verb::LIST verb_list;
};


//*********************************************************************

Verb::Verb() : m_usesDde(false)
{
}

bool
Verb::ReadFromRegistry(const wxRegKey & base, const wxString & verbName)
{
  // Store descriptive verb name
  m_name = verbName;

  if (!base.HasSubKey(verbName))
    return false;

  wxRegKey verbKey(base, verbName);

  // Read command key
  const wxString commandKeyName(wxT("command"));
  if (!verbKey.HasSubKey(commandKeyName))
    return false;

  wxRegKey commandKey(base, commandKeyName);

  if (commandKey.HasValue(NULL))
    commandKey.QueryValue(NULL, m_command);

  // Attempt to read ddeexec key
  m_ddeCommand.Clear();
  m_ddeTopic.Clear();
  m_ddeApplication.Clear();

  const wxString ddeCommandKeyName(wxT("ddeexec"));
  m_usesDde = verbKey.HasSubKey(ddeCommandKeyName);
  if (m_usesDde)
  {
    wxRegKey ddeCommandKey(verbKey, ddeCommandKeyName);;
    if (ddeCommandKey.HasValue(NULL))
      ddeCommandKey.QueryValue(NULL, m_ddeCommand);

    const wxString ddeTopicName(wxT("Topic"));
    if (ddeCommandKey.HasSubKey(ddeTopicName))
    {
      wxRegKey ddeTopicKey(ddeCommandKey, ddeTopicName);
      if (ddeTopicKey.HasValue(NULL))
        ddeTopicKey.QueryValue(NULL, m_ddeTopic);
    }

    const wxString ddeApplicationName(wxT("Application"));
    if (ddeCommandKey.HasSubKey(ddeApplicationName))
    {
      wxRegKey ddeApplicationKey(ddeCommandKey, ddeApplicationName);
      if (ddeApplicationKey.HasValue(NULL))
        ddeApplicationKey.QueryValue(NULL, m_ddeApplication);
    }
  }

  return true;
}

void
Verb::Launch(const wxString  & documentPath) const
{
  ShellExecute(NULL, m_name.c_str(),
               documentPath.c_str(), NULL, NULL, SW_SHOWNORMAL);
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

void
VerbList::InitFromDocument(const wxString & documentPath, bool isAFolder)
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

  m->documentPath = documentPath;

  // Make sure list is empty
  m->verb_list.clear();


  wxString progid_key_name;

  // Get progid of extension
  wxRegKey regKeyHKCR;

  if (isAFolder)
    progid_key_name = wxT("Folder");
  else
  {
    // Find document extension
    if (!m->documentPath.HasExt())
      // Nothing to go on; we cannot provide an editor
      return;

    wxString extension_key_name(wxT(".") + m->documentPath.GetExt());
    if (!regKeyHKCR.HasSubKey(extension_key_name))
      return;

    wxRegKey ext_key(regKeyHKCR, extension_key_name);
    if (!ext_key.HasValue(NULL))
      return;

    ext_key.QueryValue(NULL, progid_key_name);
  }

  // Get information on progid (name, verbs)
  if (!regKeyHKCR.HasSubKey(progid_key_name))
    return;

  wxRegKey progid_key(regKeyHKCR, progid_key_name);

  // Get long name of progid (ignored if not found)
  if (progid_key.HasValue(NULL))
    progid_key.QueryValue(NULL, m->document_type_name);

  // Get verbs of progid
  const wxString progid_shell(wxT("shell"));
  if (!progid_key.HasSubKey(progid_shell))
    return;

  wxRegKey verb_base_key(progid_key, progid_shell);

  // Get name of default verb (ignored if not found)

  wxString default_verbName;
  if (verb_base_key.HasValue(NULL))
    verb_base_key.QueryValue(NULL, default_verbName);

  wxString verb_key_name;
  long index;
  bool ok = verb_base_key.GetFirstKey(verb_key_name, index);

  while (ok)
  {
    Verb verb;
    if (verb.ReadFromRegistry(verb_base_key, verb_key_name))
    {
      // Place first if default
      if (verb_key_name == default_verbName)
        m->verb_list.insert(m->verb_list.begin(), verb);
      else
        m->verb_list.push_back(verb);
    }

    // select next key
    ok = verb_base_key.GetNextKey(verb_key_name, index);
  }
}

size_t
VerbList::GetCount() const
{
  return m->verb_list.size();
}

const wxString &
VerbList::GetName(size_t index) const
{
  return m->verb_list[index].m_name;
}

void
VerbList::Launch(size_t index) const
{
  m->verb_list[index].Launch(m->documentPath.GetFullPath());
}


#endif

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
