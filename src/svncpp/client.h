/*
 * ====================================================================
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
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

#include "apr_general.h"
#include "svn_pools.h"
#include "svn_client.h"
#include "svn_opt.h"
#include "status.h"
#include <string>
#include <vector>

/**
 * SvnCpp namespace.
 */
namespace svn
{
  /**
   * Subversion client API.
   */
  class Client
  {
  protected:
    apr_pool_t * m_pool;
    svn_opt_revision_t m_rev;
    std::string m_lastPath;

    /**
     * Global error object struct.
     */
    svn_error_t *m_Err;

    /**
     * Convert the path to the Subversion format.
     */
    void internalPath (std::string & path);

    /**
     * Create a revision template.
     */
    svn_opt_revision_t *getRevision (long revNumber);

    /**
     * Creates a target out of a string.
     */
    apr_array_header_t *target (const char *path);

  public:
    /**
     * Initializes the primary memory pool.
     */
     Client ();

    ~Client ();


    /**
     * Returns the last destination path submitted. The path 
     * is returned in Subversion path format.
     */
    const char *getLastPath ();

    /**
     * Enumerates all files/dirs at a given path.
     *
     * @param path Path to explore.
     * @param descend Recurse into subdirectories if existant.
     * @return Hash map with Status entries.
     */
     std::vector < Status * >status (const char * path,
                                     const bool descend = false);

    /**
     * Returns the status of a single file in the path.
     *
     * @param path File to gather status.
     * @return NULL if the path is not versioned.
     */
     Status * singleStatus (const char * path);
  };

}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
