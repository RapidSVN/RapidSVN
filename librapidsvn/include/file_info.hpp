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
 * along with this program (in the file GPL.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _FILE_INFO_H_INCLUDED_
#define _FILE_INFO_H_INCLUDED_

// forward declarations
namespace svn
{
  class Context;
}

class FileInfo
{
public:
  /**
   * constructor.
   *
   * @param context context to use
   */
  FileInfo(svn::Context * context, bool withUpdate = false);

  /**
   * destructor
   */
  virtual ~FileInfo();

  /**
   * adds a path to the list of paths that are to be examined
   *
   * @param path absolute path to add
   */
  void
  addPath(const char * path);

  /**
   * get an info string that contains informations about
   * the selection of files
   *
   * @return informational string
   */
  const wxString &
  info() const;

private:
  // hide implementation details
  struct Data;
  Data * m;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
