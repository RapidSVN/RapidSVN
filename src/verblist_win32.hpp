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
#ifndef _VERBLIST_WIN32_HPP_INCLUDED_
#define _VERBLIST_WIN32_HPP_INCLUDED_

#include <vector>
#include <string>
#include <exception>

class RegKey;

/**
 * Windows-specific IVerbList implementation
 *
 * @todo Organize more portably (which structure is the prefererred one?)
 * @todo Support creation based on mime-type
 * TODO: Implement icon retreival using:
 * HRESULT IShellIcon::GetIconOf(LPCITEMIDLIST pidl, UINT flags, LPINT lpIconIndex);
 * or: 
 * HKEY_CLASSES_ROOT\progid\DefaultIcon
 */
class VerbList : public IVerbList
{
public:
  /**
   * Construct an empty verb list
   */
  VerbList ();

  /** 
   * @see IVerbList
   */ 
  void InitFromDocument (const wxString &document_path);

  /** 
   * @see IVerbList
   */ 
  size_t GetCount () const;

  /** 
   * @see IVerbList
   */ 
  const char *GetName (size_t index) const;

  /** 
   * @see IVerbList
   */ 
  void Launch (size_t index) const;

protected:
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

  wxFileName m_document_path;
  std::string m_document_type_name;
  Verb::LIST m_verb_list;
};

#endif 
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
