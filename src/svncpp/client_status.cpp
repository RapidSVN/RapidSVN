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
#include "dirent.hpp"
#include "exception.hpp"
#include "pool.hpp"
#include "status.hpp"
#include "targets.hpp"
#include "url.hpp"

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
    entries->insert (entries->begin (), LogEntry(rev, author, date, msg));

    return NULL;
  }

  struct StatusEntriesBaton {
    apr_pool_t* pool;
    apr_hash_t* hash;
  };

  static void StatusEntriesFunc (void *baton,
                                 const char *path,
                                 svn_wc_status_t *status)
  {
      svn_wc_status_t* stat;
      StatusEntriesBaton* seb = (StatusEntriesBaton*)baton;

      path = apr_pstrdup (seb->pool, path);
      stat = svn_wc_dup_status (status, seb->pool);
      apr_hash_set (seb->hash, path, APR_HASH_KEY_STRING, stat);
  }

  static StatusEntries
  localStatus (const char * path,
               const bool descend,
               const bool get_all,
               const bool update,
               const bool no_ignore,
               Context * context)
  {
    svn_error_t *error;
    StatusEntries entries;
    apr_hash_t *status_hash;
    svn_revnum_t revnum;
    Pool pool;
    StatusEntriesBaton baton;

    status_hash = apr_hash_make (pool);
    baton.hash = status_hash;
    baton.pool = pool;

    error = svn_client_status (
      &revnum,      // revnum
      path,         // path
      0,
      StatusEntriesFunc, // status func
      &baton,        // status baton
      descend,
      get_all,
      update,
      no_ignore,
      *context,    //client ctx
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
    for (i = statusarray->nelts-1; i >= 0; i--)
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

      entries.push_back (Status (filePath, status));

    }
    return entries;
  }

  static Status 
  dirEntryToStatus (const DirEntry & dirEntry)
  {
    Pool pool;

    svn_wc_entry_t * e =
      static_cast<svn_wc_entry_t *> (
        apr_pcalloc (pool, sizeof (svn_wc_entry_t)));
    e->name = dirEntry.name ();
    e->revision = dirEntry.createdRev ();
    e->url = dirEntry.name ();
    e->kind = dirEntry.kind ();
    e->schedule = svn_wc_schedule_normal;
    e->text_time = dirEntry.time ();
    e->prop_time = dirEntry.time ();
    e->cmt_rev = dirEntry.createdRev ();
    e->cmt_date = dirEntry.time ();
    e->cmt_author = dirEntry.lastAuthor ();

    svn_wc_status_t * s =
      static_cast<svn_wc_status_t *> (
        apr_pcalloc (pool, sizeof (svn_wc_status_t)));
    s->entry = e;
    s->text_status = svn_wc_status_normal;
    s->prop_status = svn_wc_status_normal;
    s->locked = 0;
    s->switched = 0;
    s->repos_text_status = svn_wc_status_normal;
    s->repos_prop_status = svn_wc_status_normal;

    return Status (dirEntry.name (), s);
  }

  static StatusEntries
  remoteStatus (Client * client,
                const char * path,
                const bool descend,
                const bool get_all,
                const bool update,
                const bool no_ignore,
                Context * context)
  {
    Revision rev (Revision::HEAD);
    DirEntries dirEntries = client->ls (path, rev, descend);
    DirEntries::const_iterator it;
    
    StatusEntries entries;

    for (it = dirEntries.begin (); it != dirEntries.end (); it++)
    {
      const DirEntry & dirEntry = *it;

      entries.push_back (dirEntryToStatus (dirEntry));
    }

    return entries;
  }


  StatusEntries 
  Client::status (const char * path,
                  const bool descend,
                  const bool get_all,
                  const bool update,
                  const bool no_ignore)
  {
    if (Url::isValid (path))
      return remoteStatus (this, path, descend, get_all, update, 
                           no_ignore, m_context);
    else
      return localStatus (path, descend, get_all, update, 
                          no_ignore, m_context);
  }

  static Status
  localSingleStatus (const char * path, Context * context)
  {
    svn_error_t *error;
    apr_hash_t *status_hash;
    Pool pool;
    StatusEntriesBaton baton;

    status_hash = apr_hash_make( pool );
    baton.hash = status_hash;
    baton.pool = pool;

    error = svn_client_status (
      0,      // revnum
      path,         // path
      0,
      StatusEntriesFunc, // status func
      &baton,        // status baton
      false,
      true,
      false,
      false,
      *context,    //client ctx
      pool);

    if (error != NULL)
    {
      throw ClientException (error);
    }
    
    apr_array_header_t *statusarray = 
      apr_hash_sorted_keys (status_hash, svn_sort_compare_items_as_paths,
                            pool);
    const svn_item_t *item;
    const char *filePath;
    svn_wc_status_t *status = NULL;

    item = &APR_ARRAY_IDX (statusarray, 0, const svn_item_t);
    status = (svn_wc_status_t *) item->value;

    //TODO svn_error_t *err =
    svn_utf_cstring_from_utf8 (&filePath, (const char *) item->key, pool);
    
    return Status (filePath, status);
  };

  static Status
  remoteSingleStatus (const char * path, Context * context)
  {
    // TODO!!!
    return Status ();
  }

  Status 
  Client::singleStatus (const char * path)
  {
    if (Url::isValid (path))
      return remoteSingleStatus (path, m_context);
    else
      return localSingleStatus (path, m_context);
  }

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
