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

// Subversion api
#include "svn_client.h"
#include "svn_sorts.h"
#include "svn_utf.h"

// svncpp
#include "client.hpp"
#include "exception.hpp"
#include "pool.hpp"
#include "status.hpp"
#include "targets.hpp"

#define DEFAULT_ARRAY_SIZE 5

namespace svn
{
  static svn_error_t *
  logReceiver (void *baton,
                   apr_hash_t * changed_paths,
                   svn_revnum_t rev,
                   const char *author,
                   const char *date, 
                   const char *msg, 
                   apr_pool_t * pool)
  {
    LogEntries * entries = 
      (LogEntries *) baton;
    entries->push_back ( LogEntry(rev, author, date, msg));

    return NULL;
  }

  std::vector<Status>
  Client::status (const char * path, const bool descend)
  {
    svn_error_t *error;
    std::vector<Status>statusHash;
    apr_hash_t *status_hash;
    Pool pool;

    error = svn_client_status (
      &status_hash, // pointer to hash
      NULL,         // revnum
      path,         // path
      descend, 
      true,         // get all
      false,        //update
      false,        //no_ignore,
      *m_context,    //client ctx
      pool);

    if (error!=NULL)
    {
      throw ClientException (error);
    }

    apr_array_header_t *statusarray = 
      apr_hash_sorted_keys (status_hash, svn_sort_compare_items_as_paths,
                            pool);
    int i;

    /* Loop over array, printing each name/status-structure */
    for (i = 0; i < statusarray->nelts; i++)
    {
      const svn_item_t *item;
      const char *filePath;
      svn_error_t *err;
      svn_wc_status_t *status = NULL;

      item = &APR_ARRAY_IDX (statusarray, i, const svn_item_t);
      status = (svn_wc_status_t *) item->value;

      err =
        svn_utf_cstring_from_utf8 (&filePath, (const char *) item->key, pool);
      /* no error handling here yet
         if (err)
         svn_handle_error (err, stderr, FALSE);
      */

      statusHash.push_back (Status (filePath, status));

    }
    return statusHash;
  }

  Status 
  Client::singleStatus (const char * path)
  {
    svn_error_t *error;
    apr_hash_t *status_hash;
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();

    error = svn_client_status (
      &status_hash, // pointer to hash
      NULL,         // revnum
      path,         // path
      false,        // descend
      true,         // get all
      false,        //update
      false,        //no_ignore,
      *m_context,   //client ctx
      apr_pool);

    if (error != NULL)
    {
      throw ClientException (error);
    }
    
    apr_array_header_t *statusarray = 
      apr_hash_sorted_keys (status_hash, svn_sort_compare_items_as_paths,
                            apr_pool);
    const svn_item_t *item;
    const char *filePath;
    svn_wc_status_t *status = NULL;

    item = &APR_ARRAY_IDX (statusarray, 0, const svn_item_t);
    status = (svn_wc_status_t *) item->value;

    //TODO svn_error_t *err =
    svn_utf_cstring_from_utf8 (&filePath, (const char *) item->key, apr_pool);
    // no error handling here yet
    //   if (err)
    //   svn_handle_error (err, stderr, FALSE);
    
    return Status (filePath, status);
  };

  const LogEntries *
  Client::log (const char * path, const Revision & revisionStart, 
               const Revision & revisionEnd)
  {
    Targets target (path);
    Pool pool;
    LogEntries * entries = new LogEntries ();
    svn_error_t *error;
    error = svn_client_log (
      target.array (pool), 
      revisionStart.revision (), 
      revisionEnd.revision (), 
      0, // not reverse by default
      1, // strict by default (not showing cp info)
      logReceiver,
      entries, 
      *m_context, // client ctx
      pool);

    if (error != NULL)
    {
      delete entries;
      throw ClientException (error);
    }

    return entries;
  }

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
