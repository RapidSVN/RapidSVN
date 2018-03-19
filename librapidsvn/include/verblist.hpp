/*
 * ====================================================================
 * Copyright (c) 2002-2018 The RapidSVN Group.  All rights reserved.
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
#ifndef _VERBLIST_HPP_INCLUDED_
#define _VERBLIST_HPP_INCLUDED_

// forward declarations
class wxString;

/**
 * VerbList abstracts a list of editors/handlers for a given file, with the
 * default one listed first.
 */
class VerbList
{
public:
  /** constructor */
  VerbList();

  /** destructor */
  virtual ~VerbList();

  /**
   * Assemble list of verbs based on the given document which can be a folder
   * or a file.
   *
   * @throw std::exception on unexpected errors
   */
  void
  InitFromDocument(const wxString & documentPath, bool isAFolder);

  /**
   * @return Number of verbs in list
   */
  size_t
  GetCount() const;

  /**
   * @return Name of verb with the given index
   */
  const wxString  &
  GetName(size_t index) const;

  /**
   * Launches the verb with the given index on the document on which the verb
   * list is based
   */
  void
  Launch(size_t index) const;

private:
  struct Data;
  Data *m;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
