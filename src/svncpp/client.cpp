
#include "client.h"
#include "pool.h"
#include "svn_path.h"
#include "svn_client.h"
#include "svn_sorts.h"
#include "svn_utf.h"

#define DEFAULT_ARRAY_SIZE 5

namespace svn
{

  Client::Client ()
  {
    pool = svn_pool_create (NULL);
    memset (&rev, 0, sizeof (rev));
  }

  Client::~Client ()
  {
    svn_pool_destroy (pool);
  }

  svn_opt_revision_t *
  Client::getRevision (long revNumber)
  {
    rev.value.number = 0;
    rev.value.date = 0;
    rev.kind = svn_opt_revision_unspecified;

    switch (revNumber)
    {
    case -1:
      rev.kind = svn_opt_revision_unspecified;
      break;
    case -2:
      rev.kind = svn_opt_revision_head;
      break;
    default:
      rev.kind = svn_opt_revision_number;
      rev.value.number = revNumber;
      break;
    }

    return &rev;
  }

  apr_array_header_t *
  Client::target (const char *path)
  {
    apr_array_header_t *targets = apr_array_make (pool,
                                                  DEFAULT_ARRAY_SIZE,
                                                  sizeof (const char *));

    const char *target = apr_pstrdup (pool, path);
    (*((const char **) apr_array_push (targets))) = target;

    return targets;
  }

  const char *
  Client::getLastPath ()
  {
    return lastPath.c_str ();
  }

  void 
  Client::internalPath (std::string & path)
  {
    svn_stringbuf_t *buf = svn_stringbuf_create (path.c_str (), pool);
    svn_path_internal_style (buf);
    path = buf->data;
  }

  std::vector < Status * >
  Client::status (const char * path, const bool descend)
  {
    svn_error_t *Err;
    std::vector < Status * >statusHash;
    apr_hash_t *status_hash;
    Pool subPool(pool);

    Err = svn_client_status (&status_hash, NULL, path, NULL, descend, TRUE,
                             FALSE,     //update
                             FALSE,     //no_ignore,
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

        statusHash.push_back (new Status (filePath, status));

      }
    }
    return statusHash;
  }

  Status *
  Client::singleStatus (const char * path)
  {
    svn_error_t *Err;
    Status * result;
    apr_hash_t *status_hash;
    Pool subPool(pool);

    Err = svn_client_status (&status_hash, NULL, path, NULL, false, true,
                             false,     //update
                             false,     //no_ignore,
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
      result = new Status (filePath, status);
    }
    else
      result = NULL;

    return result;
  };

}
