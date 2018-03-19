/*
 * ====================================================================
 * Copyright (c) 2002-2018 The RapidSVN Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program (in the file LGPL.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#ifndef _SVNCPP_REPOSITORY_PATH_HPP_
#define _SVNCPP_REPOSITORY_PATH_HPP_

#include "svncpp/path.hpp"

namespace svn
{
  /**
   * Encapsulation for Subversion Path handling with repository root.
   */
  class RepositoryPath : public Path
  {
  private:
    std::string m_repositoryRoot;

    RepositoryPath(const Path &) {}
    Path& operator= (const Path & path) {}

  public:
    /**
     * Constructor that takes a path as parameter
     * and also repository root path.
     * Both strings are converted to subversion internal
     * representation. The string are copied.
     *
     * @param path RepositoryPath string
     * @param repositoryRoot RepositoryPath string
     */
    RepositoryPath(const std::string & path = "", const std::string & repositoryRoot = "")
      : Path(path), m_repositoryRoot(repositoryRoot) {}

    /**
     * Constructor
     *
     * @see RepositoryPath::RepositoryPath (const std::string &, const std::string &)
     * @param path RepositoryPath string
     * @param repositoryRoot RepositoryPath string
     */
    RepositoryPath(const char * path, const char * repositoryRoot)
      : Path(path), m_repositoryRoot(repositoryRoot != NULL ? repositoryRoot : "") {}

    /**
     * Copy constructor
     *
     * @param path RepositoryPath to be copied
     */
    RepositoryPath(const RepositoryPath & path)
      : Path(path), m_repositoryRoot(path.getRepositoryRoot()) {}

    /**
     * Assignment operator
     */
    RepositoryPath& operator= (const RepositoryPath & path)
    {
      if (this == &path)
        return *this;

      Path(*this) = path;
      m_repositoryRoot = path.getRepositoryRoot();

      return *this;
    }

    /**
     * Comparison operator
     */
    bool
    operator== (const RepositoryPath & path) const
    {
      return Path(*this) == path && m_repositoryRoot == path.getRepositoryRoot();
    }

    /** returns repository root of subversion, which contains this path */
    std::string
    getRepositoryRoot() const
    {
      return m_repositoryRoot;
    }
  };
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
