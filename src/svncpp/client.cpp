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
    svn_error_t *Err;
    std::vector<Status>statusHash;
    apr_hash_t *status_hash;
    Pool subPool;

    Err = svn_client_status (&status_hash, NULL, path, NULL, descend, TRUE,
                             FALSE,     //update
                             FALSE,     //no_ignore,
                             NULL, NULL,  //Notify baton
                             subPool.pool());
    if (Err == NULL)
    {
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
    }
    return statusHash;
  }

  Status 
  Client::singleStatus (const char * path)
  {
    svn_error_t *Err;
    apr_hash_t *status_hash;
    Pool subPool;

    Err = svn_client_status (&status_hash, 
                             NULL, // youngest
                             path, 
                             NULL, // auth_baton
                             false, // dont descend
                             true, // get all
                             false,     // dont update
                             false,     // ignore,
                             NULL, // no notify func
                             NULL, // no notify baton
                             subPool.pool());

    if(Err == NULL)
    {
      apr_array_header_t *statusarray = 
         apr_hash_sorted_keys (status_hash, svn_sort_compare_items_as_paths,
                               subPool.pool());
      const svn_item_t *item;
      const char *filePath;
      svn_error_t *err;
      svn_wc_status_t *status = NULL;

      item = &APR_ARRAY_IDX (statusarray, 0, const svn_item_t);
      status = (svn_wc_status_t *) item->value;

      err =
        svn_utf_cstring_from_utf8 (&filePath, (const char *) item->key, subPool.pool());
      /* no error handling here yet
         if (err)
         svn_handle_error (err, stderr, FALSE);
       */
      return Status (filePath, status);
    }
    else
    {
      return Status ();
    }
  };

}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
