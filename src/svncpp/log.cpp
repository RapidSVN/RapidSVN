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

// subversion api
#include "svn_client.h"

// svncpp
#include "exception.hpp"
#include "log.hpp"
#include "path.hpp"
#include "pool.hpp"
#include "revision.hpp"
#include "targets.hpp"

namespace svn
{
  static svn_error_t *
  messageReceiver (void *baton,
                   apr_hash_t * changed_paths,
                   svn_revnum_t rev,
                   const char *author,
                   const char *date, 
                   const char *msg, 
                   apr_pool_t * pool)
  {
    std::vector<LogEntry> * entries = 
      (std::vector<LogEntry> *) baton;
    entries->push_back ( LogEntry(rev, author, date, msg));

    return NULL;
  }

  Log::Log (const char * path, const Revision & revisionStart, 
            const Revision & revisionEnd)
  {
    Targets target (path);
    Pool pool;
    apr_pool_t *apr_pool = pool.pool ();

    svn_error_t *error;
    error = svn_client_log (
      target.array (apr_pool), 
      revisionStart.revision (), 
      revisionEnd.revision (), 
      0, // not reverse by default
      1, // strict by default (not showing cp info)
      messageReceiver,
      &m_entries, 
      NULL, // client ctx
      apr_pool);

    if (error != NULL)
    {
      throw ClientException (error);
    }
  }

  Log::~Log ()
  {
  }

  const std::vector<LogEntry> &
  Log::entries () const
  {
    return m_entries;
  }
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
