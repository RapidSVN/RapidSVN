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
   * Assemble list of verbs based on the given document which can be a folder
   * or a file.
   *
   * @throw std::exception on unexpected errors
   */
  void InitFromDocument (const wxString &document_path, bool isAFolder);

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
