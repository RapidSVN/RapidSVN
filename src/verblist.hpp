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
#ifndef _VERBLIST_HPP_INCLUDED_
#define _VERBLIST_HPP_INCLUDED_

#include "wx/string.h"

/*
#ifdef _WIN32
#endif //_WIN32
*/

/**
 * VerbList abstracts a list of editors/handlers for a given file, with the
 * default one listed first.
 */
class VerbList
{
public:
#ifdef _WIN32
  /**
   * Construct an empty verb list
   */
  VerbList();  
  ~VerbList();  
#endif //_WIN32

  /**
   * Assemble list of verbs based on the given document
   *
   * @throw std::exception on unexpected errors
   */
  void InitFromDocument (const wxString &document_path);

  /**
   * @return Number of verbs in list
   */
  size_t GetCount() const;

  /**
   * @return Name of verb with the given index
   */
  const char *GetName(size_t index) const;

  /**
   * Launches the verb with the given index on the document on which the verb
   * list is based
   */
  void Launch(size_t index) const;

// No data yet used for non-windows platforms
#ifdef _WIN32
private:
  struct Data;
  Data *m;
#endif //_WIN32
};

#endif // _VERBLIST_HPP_INCLUDED_
