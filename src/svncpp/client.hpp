/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#ifndef _SVNCPP_CLIENT_H_
#define _SVNCPP_CLIENT_H_

// stl
#include <vector>

// svncpp
#include "path.hpp"
#include "revision.hpp"

/**
 * SvnCpp namespace.
 */
namespace svn
{
  // forward declarations
  class Status;

  /**
   * Subversion client API.
   */
  class Client
  {
  protected:
    Revision m_revision;
    Path m_lastPath;

    /**
     * Global error object struct.
     */
    svn_error_t *m_Err;

  public:
    /**
     * Initializes the primary memory pool.
     */
    Client ();

    ~Client ();

    /**
     * Returns the last destination path submitted. 
     *
     * @return path in Subversion format.
     */
    const char *
    getLastPath ();

    /**
     * Enumerates all files/dirs at a given path.
     *
     * @param path Path to explore.
     * @param descend Recurse into subdirectories if existant.
     * @return Hash map with Status entries.
     */
    std::vector<Status>
    status (const char * path,  const bool descend = false);

    /**
     * Returns the status of a single file in the path.
     *
     * @param path File to gather status.
     * @return a Status with Statis.isVersioned = FALSE
     */
    Status singleStatus (const char * path);
  };

}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
