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

// stl
#include <string>

// subversion api
#include "svn_client.h"

// svncpp
#include "exception.hpp"
#include "modify.hpp"
#include "notify.hpp"
#include "pool.hpp"
#include "targets.hpp"

struct log_msg_baton
{
  log_msg_baton (const std::string & message)
  {
    this->message = message;
  }

  std::string message;
};

// static functions
static svn_error_t *
get_log_message (const char **log_msg,
  const char **tmp_file,
  apr_array_header_t * commit_items,
  void *baton, apr_pool_t * pool)
{
  log_msg_baton *lmb = (log_msg_baton *) baton;

  *log_msg = apr_pstrdup (pool, lmb->message.c_str ());
  return SVN_NO_ERROR;
}

namespace svn
{
  // forward declarations
  class Targets;

  Modify::Modify ()
  {
  }

  Modify::~Modify ()
  {
  }

  void
  Modify::checkout (const char * url, const Path & destPath, 
                    const Revision & revision, bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();

    if(m_notify == NULL)
      return;

    svn_error_t * error =
      svn_client_checkout (Notify::notify,
                           (void *) m_notify,
                           url, destPath.c_str (),
                           revision.revision (),
                           recurse,
                           context (apr_pool),
                           apr_pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Modify::notification (Notify * notify)
  {
    m_notify = notify;
  }

  void
  Modify::remove (const Path & path, bool force)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    svn_client_commit_info_t *commit_info = NULL;

    svn_error_t * error =
      svn_client_delete (&commit_info, path.c_str (), 
                         NULL, // wx_adm_access
                         force,
                         NULL, NULL, //no log message function/baton
                         Notify::notify, (void *) m_notify, 
                         context (apr_pool),
                         apr_pool);
    if(error != NULL)
      throw ClientException (error);
  }

  void
  Modify::revert (const Path & path, bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();

    svn_error_t * error =
      svn_client_revert (path.c_str (), recurse, Notify::notify, 
                         (void *) m_notify, apr_pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Modify::add (const Path & path, bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();

    svn_error_t * error =
      svn_client_add (path.c_str (), 
                      recurse, 
                      Notify::notify, 
                      (void *) m_notify, 
                      apr_pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Modify::update (const Path & path, const Revision & revision, 
                  bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    svn_error_t * error =
      svn_client_update (path.c_str (),
                         revision.revision (),
                         recurse,
                         Notify::notify,
                         (void *) m_notify,
                         context (apr_pool),
                         apr_pool);
    if(error != NULL)
      throw ClientException (error);
  }

  svn_revnum_t
  Modify::commit (const Targets & targets, const char * message, 
                  bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    svn_client_commit_info_t *commit_info = NULL;
    log_msg_baton baton (message);

    svn_error_t * error =
      svn_client_commit (&commit_info, 
                         Notify::notify, 
                         (void *) m_notify, 
                         targets.array (subPool), 
                         &get_log_message, 
                         &baton, 
                         !recurse, 
                         context (apr_pool),
                         apr_pool);
    if (error != NULL)
      throw ClientException (error);

    if (commit_info && SVN_IS_VALID_REVNUM (commit_info->revision))
      return commit_info->revision;

    return -1;
  }

  void
  Modify::copy (const Path & srcPath, 
                const Revision & srcRevision, 
                const Path & destPath)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    svn_client_commit_info_t *commit_info = NULL;

    svn_error_t * error = 
      svn_client_copy (&commit_info,
                       srcPath.c_str (),
                       srcRevision.revision (),
                       destPath.c_str (),
                       NULL, // wc_adm_access
                       NULL, // log_msg_func
                       NULL, // log_msg_baton
                       Notify::notify, 
                       (void *) m_notify,
                       context (apr_pool),
                       apr_pool);
  
    if(error != NULL)
      throw ClientException (error);
  }

  void
  Modify::move (const Path & srcPath, 
                const Revision & srcRevision, 
                const Path & destPath, 
                bool force)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    svn_client_commit_info_t *commit_info = NULL;

    svn_error_t * error =  
      svn_client_move (&commit_info,
                       srcPath.c_str (),
                       srcRevision.revision (),
                       destPath.c_str (),
                       force,
                       NULL, // log_msg_func
                       NULL, // log_msg_baton
                       Notify::notify,
                       (void *) m_notify,
                       context (apr_pool),
                       apr_pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Modify::mkdir (const Path & path, const char * message)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    svn_client_commit_info_t *commit_info = NULL;
    log_msg_baton baton (message);

    svn_error_t * error =  
      svn_client_mkdir (&commit_info,
                        path.c_str (),
                        &get_log_message,
                        &baton,
                        Notify::notify,
                        (void *) m_notify,
                        context (apr_pool),
                        apr_pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Modify::cleanup (const Path & path)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();

    svn_error_t * error =  
      svn_client_cleanup (path.c_str (), apr_pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Modify::resolve (const Path & path, bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();

    svn_error_t * error =  
      svn_client_resolve (path.c_str (),
                          Notify::notify,
                          (void *) m_notify,
                          recurse,
                          apr_pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Modify::doExport (const Path & srcPath, const Path & destPath, 
                    const Revision & revision)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();

    svn_error_t * error =  
      svn_client_export (srcPath.c_str (),
                         destPath.c_str (),
                         const_cast<svn_opt_revision_t*>
                         (revision.revision ()),
                         Notify::notify,
                         (void *) m_notify,
                         context (apr_pool),
                         apr_pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Modify::doSwitch (const Path & path, 
                    const char * url, 
                    const Revision & revision, 
                    bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();

    svn_error_t * error =  
      svn_client_switch (path.c_str (),
                         url,
                         revision.revision (),
                         recurse,
                         Notify::notify,
                         (void *) m_notify,
                         context (apr_pool),
                         apr_pool);
    
    if(error != NULL)
      throw ClientException (error);
  }

  void
  Modify::import (const Path & path, 
                  const char * url, 
                  const Path & newEntry, 
                  const char * message, 
                  bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    svn_client_commit_info_t *commit_info = NULL;
    log_msg_baton baton (message);

    svn_error_t * error =  
      svn_client_import (&commit_info,
                         Notify::notify,
                         (void *) m_notify,
                         path.c_str (),
                         url,
                         newEntry.c_str (),
                         &get_log_message,
                         &baton,
                         !recurse,
                         context (apr_pool),
                         apr_pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Modify::merge (const Path & path1, const Revision & revision1, 
                 const Path & path2, const Revision & revision2,
                 const Path & localPath, bool force, bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();

    svn_error_t * error =  
      svn_client_merge (Notify::notify,
                        (void *) m_notify,
                        path1.c_str (),
                        revision1.revision (),
                        path2.c_str (),
                        revision2.revision (),
                        localPath.c_str (),
                        recurse,
                        force,
                        FALSE, // dry_run
                        context (apr_pool),
                        apr_pool);

    if(error != NULL)
      throw ClientException (error);
  }

//TODO this doesnt seem to do anything
//   void *
//   Modify::logMessage (const char * message, const Path & baseDirectory)
//   {
//     Pool subPool;
//     apr_pool_t *apr_pool = subPool.pool ();
//     log_msg_baton *baton = (log_msg_baton *) 
//       apr_palloc (apr_pool, sizeof (*baton));

//     baton->message = message;
//     baton->base_dir = baseDirectory ? baseDirectory : ".";

//     return baton;
//   }

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
