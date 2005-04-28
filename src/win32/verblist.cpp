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
/*
 * VerbList - Windows implementation
 */
// app
#include "../verblist.hpp"

// Windows API
#include <windows.h>

// stl
#include <vector>
#include <string>
#include <exception>

using namespace std;

// wx
#include "wx/filename.h"

/**
 * Registry read access wrapper
 *
 * @todo Replace with wxRegKey (if possible)
 */
class RegKey
{
public:
  class RegistryException : public std::exception {};
  class FormatException : public std::exception {};
  class NotFoundException : public std::exception {};

  /** 
   * Construct closed regkey instance
   */
  RegKey();

  /** 
   * Construct opened regkey instance
   *
   * @throw NotFoundException if key is not found
   */
  RegKey(HKEY base, LPCSTR subkey);

  /** 
   * Construct opened regkey instance
   *
   * @throw NotFoundException if key is not found
   */
  RegKey(const RegKey &base, LPCSTR subkey);
  ~RegKey();

  /**
   * Open the given subkey of the given base key
   * NOTE: This function must be called to fully initialize a RegKey instance
   *
   * @param base    Base key
   * @param subkey  Name of subkey to open, or NULL to open the base key
   *                directly
   * @throw RegistryException if an OS exception occurs
   * @return false if the key does not exist
   */
  bool Open(HKEY base, LPCSTR subkey);

  /**
   * Open the given subkey of the given base key
   * NOTE: This function must be called to fully initialize a RegKey instance
   *
   * @param base    Base key
   * @param subkey  Name of subkey to open, or NULL to open the base key
   *                directly
   * @throw RegistryException if an OS exception occurs
   * @return false if the key does not exist
   */
  bool Open(const RegKey &base, LPCSTR subkey);

  /**
   * Get the value of the value with the given name and store it in the
   * given std::string variable.
   *
   * @param name Name of the value, or NULL for the standard value
   * @param dst  String to receive value contents
   *
   * @return false if no such value exists. In this case, dst will be the
   *               empty string.
   *
   * @throw RegKey::RegistryException a registry error occurs
   * @throw RegKey::FormatException   the value cannot be converted to a
   *                                  string.
   */
  bool GetStringValue(LPCSTR name, std::string &dst) const;
  
  /**
   * Get the next subkey name
   *
   * An implementation using RegQueryInfoKey to determine subkey count and
   * exposing a GetSubKeyName(size_t index) method is also possible.
   *
   * @return false if there are no more subkeys
   */
  bool GetNextSubKeyName(std::string &dst);

protected:
  HKEY m_base, m_key;
  DWORD m_subkey_index;
};

/*
 * Stores one verb
 */
struct Verb
{
  typedef std::vector<Verb> LIST;

  bool m_uses_dde;
  std::string m_name, m_command;
  std::string m_dde_command, m_dde_app, m_dde_topic;

  Verb();

  /**
   * Read verb information from the given verb of the given key in the
   * registry
   *
   * If the registry entry is bad, this may result in a verb with only a
   * name (no commands), but that'll still be usable in some contexts.
   *
   * @throw RegKey::RegistryException
   * @throw RegKey::FormatException
   */
  void ReadFromRegistry(const RegKey &base, const char *verb_name);

  /**
   * Lauch the verb on the given document
   */
  void Launch(const char *document_path) const;
};

/**
 * Private data members of VerbList
 */
struct VerbList::Data
{
  wxFileName document_path;
  std::string document_type_name;
  Verb::LIST verb_list;
};

/****************************************************************************/

RegKey::RegKey () : m_base (NULL), m_key (NULL)
{
}

RegKey::RegKey (HKEY base, LPCSTR subkey)
{
  if (!Open (base, subkey))
    throw NotFoundException ();
}

RegKey::RegKey (const RegKey &base, LPCSTR subkey)
{
  if (!Open (base, subkey))
    throw NotFoundException ();
}

RegKey::~RegKey ()
{
  if (m_key != NULL)
    RegCloseKey (m_key);
}

bool RegKey::Open (HKEY base, LPCSTR subkey)
{
  LONG result;

  // Close open key before opening a new one
  if (m_key != NULL)
  {
    RegCloseKey (m_key);
    m_key = NULL;
  }

  m_base = base;

  // Reset subkey index as we're on a new key
  m_subkey_index = 0;

  result = RegOpenKeyEx (base, subkey, 0, KEY_READ, &m_key);
  if (result == ERROR_SUCCESS)
  {
    return true;
  }
  else
  {
    m_key = NULL;
    if (result == ERROR_FILE_NOT_FOUND)
      return false;
    else
      throw RegistryException ();
  }
}

bool RegKey::Open (const RegKey &base, LPCSTR subkey)
{
  return Open (base.m_key, subkey);
}

bool RegKey::GetStringValue (LPCSTR name, string &dst) const
{
  DWORD data_size, data_type;
  BYTE *data;
  LONG result;
  
  result = RegQueryValueEx (m_key, name, 0, &data_type, NULL, &data_size);
  if (result != ERROR_SUCCESS)
  {
    if (result == ERROR_FILE_NOT_FOUND)
    {
      dst.resize (0);
      return false;
    }
    else
    {
      throw RegistryException ();
    }
  }

  switch (data_type)
  {
    //TODO: Support unicode values
    //case REG_MULTI_SZ:
    case REG_SZ:
    case REG_EXPAND_SZ:
      // Ideally, I'd like to use boost::scoped_array here: It makes for a
      // cleaner syntax.
      data = new BYTE[data_size];
      try
      {
        if (RegQueryValueEx (m_key, name, 0, &data_type, data, &data_size) != ERROR_SUCCESS)
          throw RegistryException ();

        dst.assign ((char *)data);

        delete[] data;
      }
      catch (...)
      {
        delete[] data;
      }
      break;

    default:
      throw FormatException ();
      break;
  }

  return true;
}

bool RegKey::GetNextSubKeyName (string &dst)
{
  // Platform SDK docs (march 2003) say that key names are max 255
  // characters long
  TCHAR key_name[256];
  DWORD key_name_length;
  FILETIME last_write_time;
  LONG result;

  // TODO: Handle ERROR_MORE_DATA. Not currently needed, as registry keys
  // are currently never longer than the buffer.
  key_name_length = sizeof (key_name);
  result = RegEnumKeyEx (m_key, m_subkey_index, key_name, &key_name_length, 0, NULL, 
    NULL, &last_write_time);

  if ((result == ERROR_SUCCESS) || (result == ERROR_NO_MORE_ITEMS))
  {
    m_subkey_index++;
    dst.assign (key_name);
    return (result != ERROR_NO_MORE_ITEMS);
  }
  else
  {
    throw RegistryException ();
  }
}

/****************************************************************************/

Verb::Verb () : m_uses_dde (false)
{
}

void Verb::ReadFromRegistry (const RegKey &base, LPCSTR verb_name)
{
  RegKey dde_command_key, command_key;

  // Store descriptive verb name 
  m_name = verb_name;

  // Read command key
  string command_key_name (verb_name);
  command_key_name.append ("\\command");
  if (command_key.Open (base, command_key_name.c_str ()))
  {
    command_key.GetStringValue (NULL, m_command);

    // Attempt to read ddeexec key
    string dde_command_key_name (verb_name);
    dde_command_key_name.append ("\\ddeexec");
    if (dde_command_key.Open (base, dde_command_key_name.c_str ()))
    {
      RegKey dde_topic_key, dde_app_key;

      dde_command_key.GetStringValue (NULL, m_dde_command);

      if (dde_topic_key.Open (dde_command_key, "Topic"))
        dde_topic_key.GetStringValue (NULL, m_dde_topic);

      if (dde_app_key.Open (dde_command_key, "Application"))
        dde_app_key.GetStringValue (NULL, m_dde_app);

      m_uses_dde = true;
    }
  }
}

void Verb::Launch (const char *document_path) const
{
  // Execute verb on document
  // TODO: Supply window handle of RapidSVN window
  LONG result = (LONG)ShellExecute (NULL, m_name.c_str (), document_path, NULL, NULL, SW_SHOWNORMAL);
  if (result <= 32)
  {
    /*
    TODO: Handle errors. Always one of the following (according to M$ Platform
    SDK):

    0                       The operating system is out of memory or
                            resources.
    ERROR_FILE_NOT_FOUND    The specified file was not found.
    ERROR_PATH_NOT_FOUND    The specified path was not found. 
    ERROR_BAD_FORMAT        The .exe file is invalid (non-Win32® .exe or error
                            in .exe image).
    SE_ERR_ACCESSDENIED     The operating system denied access to the
                            specified file. 
    SE_ERR_ASSOCINCOMPLETE  The file name association is incomplete or
                            invalid.
    SE_ERR_DDEBUSY          The DDE transaction could not be completed because
                            other DDE transactions were being processed.
    SE_ERR_DDEFAIL          The DDE transaction failed. 
    SE_ERR_DDETIMEOUT       The DDE transaction could not be completed because
                            the request timed out.
    SE_ERR_DLLNOTFOUND      The specified dynamic-link library was not found.  
    SE_ERR_FNF              The specified file was not found.  
    SE_ERR_NOASSOC          There is no application associated with the given
                            file name extension.
    SE_ERR_OOM              There was not enough memory to complete the
                            operation.
    SE_ERR_PNF              The specified path was not found. 
    SE_ERR_SHARE            A sharing violation occurred. 
    */
  }
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

void VerbList::InitFromDocument (const wxString &document_path, bool isAFolder)
{
  RegKey ext_key;
  wxString extension_key_name;
  string progid_key_name;
  RegKey progid_key;

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

  if (isAFolder)
  {
    progid_key_name = "Folder";
  }
  else
  {
    // Find document extension
    if (!m->document_path.HasExt ())
      // Nothing to go on; we cannot provide an editor
      return;

    // Get progid of extension
    extension_key_name = "." + m->document_path.GetExt ();
    if (!ext_key.Open (HKEY_CLASSES_ROOT, extension_key_name.c_str()))
      return;
    if (!ext_key.GetStringValue (NULL, progid_key_name))
      return;
  }

  // Get information on progid (name, verbs)
  if (progid_key.Open (HKEY_CLASSES_ROOT, progid_key_name.c_str ()))
  {
    RegKey verb_base_key;
    string verb_key_name, default_verb_name;

    // Get long name of progid (ignored if not found)
    progid_key.GetStringValue (NULL, m->document_type_name);

    // Get verbs of progid
    if (verb_base_key.Open (progid_key, "shell"))
    {
      // Get name of default verb (ignored if not found)
      verb_base_key.GetStringValue (NULL, default_verb_name);
      while (verb_base_key.GetNextSubKeyName (verb_key_name))
      {
        try
        {
          Verb verb;
          verb.ReadFromRegistry (verb_base_key, verb_key_name.c_str ());

          // Place first if default
          if (verb_key_name == default_verb_name)
            m->verb_list.insert (m->verb_list.begin (), verb);
          else
            m->verb_list.push_back (verb);
        }
        catch (RegKey::NotFoundException)
        {
          // Do nothing: this is a symptom of some program registering its
          // file associations incorrectly. The rest of the verbs may still be
          // readable.
        }
        catch (RegKey::FormatException)
        {
          // Some part of the verb was stored in an unknown format. Same as
          // above.
        }
      }
    }
  }
}

size_t VerbList::GetCount () const
{
  return m->verb_list.size ();
}

const char *VerbList::GetName (size_t index) const
{
  return m->verb_list[index].m_name.c_str ();
}

void VerbList::Launch (size_t index) const
{
  m->verb_list[index].Launch (m->document_path.GetFullPath ().c_str ());
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
