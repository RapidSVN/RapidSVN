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
#include "modify.hpp"
#include "notify.hpp"
#include "pool.hpp"
#include "targets.hpp"

struct log_msg_baton
{
  const char *message;
  const char *base_dir;
};

// forward declarations
static svn_error_t * 
get_log_message (const char **log_msg,
                 const char **tmp_file,
                 apr_array_header_t * commit_items,
                 void *baton, apr_pool_t * pool);

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
  Modify::checkout (const char * moduleName, const char *destPath, 
                    const Revision & revision, bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    m_lastPath = destPath;

    if(m_notify == NULL)
      return;

    m_Err = svn_client_checkout (Notify::notify,
                                 (void *) m_notify,
                                 authenticate (),
                                 moduleName,
                                 m_lastPath.c_str (),
                                 revision.revision (),
                                 recurse,
                                 apr_pool);

    if(m_Err != NULL)
      throw ClientException (m_Err);
  }

  void
  Modify::notification (Notify * notify)
  {
    m_notify = notify;
  }

  void
  Modify::remove (const char * path, bool force)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    svn_client_commit_info_t *commit_info = NULL;
    m_lastPath = path;

    m_Err = svn_client_delete (&commit_info, m_lastPath.c_str (), NULL, force,
                               authenticate (),
                               &get_log_message,
                               logMessage (NULL),
                               Notify::notify, (void *) m_notify, apr_pool);
    if(m_Err != NULL)
      throw ClientException (m_Err);
  }

  void
  Modify::revert (const char * path, bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    m_lastPath = path;
    m_Err = svn_client_revert (m_lastPath.c_str (), recurse, Notify::notify, 
                               (void *) m_notify, apr_pool);

    if(m_Err != NULL)
      throw ClientException (m_Err);
  }

  void
  Modify::add (const char * path, bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();

    m_lastPath = path;
    m_Err = svn_client_add (m_lastPath.c_str (), recurse, Notify::notify, 
                            (void *) m_notify, apr_pool);

    if(m_Err != NULL)
      throw ClientException (m_Err);
  }

  void
  Modify::update (const char * path, const Revision & revision, 
                  bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    m_lastPath = path;
    m_Err = svn_client_update (authenticate (),
                               m_lastPath.c_str (),
                               revision.revision (),
                               recurse,
                               Notify::notify,
                               (void *) m_notify,
                               apr_pool);
    if(m_Err != NULL)
      throw ClientException (m_Err);
  }

  svn_revnum_t
  Modify::commit (const Targets & targets, const char * message, 
                  bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    svn_client_commit_info_t *commit_info = NULL;
    //m_lastPath = path;

    m_Err = svn_client_commit (&commit_info, Notify::notify, 
                               (void *) m_notify, 
                               authenticate (), targets.array (subPool), 
                               &get_log_message, 
                               logMessage (message), !recurse, apr_pool);
    if(m_Err != NULL)
      throw ClientException (m_Err);

    if(commit_info && SVN_IS_VALID_REVNUM (commit_info->revision))
      return commit_info->revision;

    return -1;
  }

  void
  Modify::copy (const char * srcPath, const char * destPath)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    Path sourcePath = srcPath;
    m_lastPath = destPath;

    svn_client_commit_info_t *commit_info = NULL;

    m_Err = svn_client_copy (&commit_info,
                             sourcePath.c_str (),
                             Revision(Revision::START).revision (),
                             m_lastPath.c_str (),
                             NULL,
                             authenticate (),
                             &get_log_message,
                             logMessage (NULL),
                             Notify::notify, 
                             (void *) m_notify,
                             apr_pool);
  
    if(m_Err != NULL)
      throw ClientException (m_Err);
  }

  void
  Modify::move (const char * srcPath, const char * destPath, 
                const Revision & revision, bool force)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    svn_client_commit_info_t *commit_info = NULL;
    Path sourcePath = srcPath;
    m_lastPath = destPath;


    m_Err = svn_client_move (&commit_info,
                             sourcePath.c_str (),
                             revision.revision (),
                             m_lastPath.c_str (),
                             force,
                             authenticate (),
                             &get_log_message,
                             logMessage (NULL),
                             Notify::notify,
                             (void *) m_notify,
                             apr_pool);
    if(m_Err != NULL)
      throw ClientException (m_Err);
  }

  void
  Modify::mkdir (const char * path, const char * message)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    svn_client_commit_info_t *commit_info = NULL;
    m_lastPath = path;

    m_Err = svn_client_mkdir (&commit_info,
                              path,
                              authenticate (),
                              &get_log_message,
                              logMessage (message),
                              Notify::notify,
                              (void *) m_notify,
                              apr_pool);

    if(m_Err != NULL)
      throw ClientException (m_Err);
  }

  void
  Modify::cleanup (const char * path)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    m_lastPath = path;
    m_Err = svn_client_cleanup (m_lastPath.c_str (), apr_pool);

    if(m_Err != NULL)
      throw ClientException (m_Err);
  }

  void
  Modify::resolve (const char * path, bool recurse)
  {
     Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
   m_lastPath = path;
    m_Err = svn_client_resolve (m_lastPath.c_str (),
                                Notify::notify,
                                (void *) m_notify,
                                recurse,
                                apr_pool);

    if(m_Err != NULL)
      throw ClientException (m_Err);
  }

  void
  Modify::doExport (const char * srcPath, const char * destPath, 
                    const Revision & revision)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    Path sourcePath = srcPath;
    m_lastPath = destPath;
    m_Err = svn_client_export (sourcePath.c_str (),
                               m_lastPath.c_str (),
                               const_cast<svn_opt_revision_t*>(
                                 revision.revision ()),
                               authenticate (),
                               Notify::notify,
                               (void *) m_notify,
                               apr_pool);

    if(m_Err != NULL)
      throw ClientException (m_Err);
  }

  void
  Modify::doSwitch (const char * path, const char * url, 
                    const Revision & revision, bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    m_lastPath = path;

    m_Err = svn_client_switch (authenticate (),
                               m_lastPath.c_str (),
                               url,
                               revision.revision (),
                               recurse,
                               Notify::notify,
                               (void *) m_notify,
                               apr_pool);

    if(m_Err != NULL)
      throw ClientException (m_Err);
  }

  void
  Modify::import (const char * path, const char * url, const char * newEntry,
                  const char * message, bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    m_lastPath = path;
    svn_client_commit_info_t *commit_info = NULL;

    m_Err = svn_client_import (&commit_info,
                               Notify::notify,
                               (void *) m_notify,
                               authenticate (),   
                               m_lastPath.c_str (),
                               url,
                               newEntry,
                               &get_log_message,
                               logMessage (message),
                               !recurse,
                               apr_pool);

    if(m_Err != NULL)
      throw ClientException (m_Err);
  }

  void
  Modify::merge (const char * path1, const Revision & revision1, 
                 const char * path2, const Revision & revision2,
                 const char * localPath, bool force, bool recurse)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();
    m_lastPath = localPath;
    Path srcPath1 = path1;
    Path srcPath2 = path2;

    m_Err = svn_client_merge (Notify::notify,
                              (void *) m_notify,
                              authenticate (),
                              srcPath1.c_str (),
                              revision1.revision (),
                              srcPath2.c_str (),
                              revision2.revision (),
                              localPath,
                              recurse,
                              force,
                              FALSE,
                              apr_pool);

    if(m_Err != NULL)
      throw ClientException (m_Err);
  }

  void *
  Modify::logMessage (const char * message, char * baseDirectory)
  {
    Pool subPool;
    apr_pool_t *apr_pool = subPool.pool ();
    log_msg_baton *baton = (log_msg_baton *) 
      apr_palloc (apr_pool, sizeof (*baton));

    baton->message = message;
    baton->base_dir = baseDirectory ? baseDirectory : ".";

    return baton;
  }

}


static svn_error_t *
get_log_message (const char **log_msg,
  const char **tmp_file,
  apr_array_header_t * commit_items,
  void *baton, apr_pool_t * pool)
{
  log_msg_baton *lmb = (log_msg_baton *) baton;

  if (lmb->message)
  {
    *log_msg = apr_pstrdup (pool, lmb->message);
    return SVN_NO_ERROR;
  }

  return SVN_NO_ERROR;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
