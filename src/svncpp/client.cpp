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

// Subversion api
#include "svn_client.h"
#include "svn_sorts.h"
#include "svn_utf.h"

// APR api
//#include "apr_tables.h"

// svncpp
#include "client.hpp"
#include "exception.hpp"
#include "pool.hpp"
#include "status.hpp"

#define DEFAULT_ARRAY_SIZE 5

namespace svn
{

  Client::Client ()
  {
  }

  Client::~Client ()
  {
  }

  const char *
  Client::getLastPath ()
  {
    return m_lastPath.c_str ();
  }


  std::vector<Status>
  Client::status (const char * path, const bool descend)
  {
    svn_error_t *error;
    std::vector<Status>statusHash;
    apr_hash_t *status_hash;
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();

    error = svn_client_status (
      &status_hash, // pointer to hash
      NULL,         // revnum
      path,         // path
      descend, 
      true,         // get all
      false,        //update
      false,        //no_ignore,
      NULL,         //client ctx
      apr_pool);

    if (error!=NULL)
    {
      throw ClientException (error);
    }

    apr_array_header_t *statusarray = 
      apr_hash_sorted_keys (status_hash, svn_sort_compare_items_as_paths,
                            subPool.pool());
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
        svn_utf_cstring_from_utf8 (&filePath, (const char *) item->key, subPool.pool());
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
      NULL,         //client ctx
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

}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
