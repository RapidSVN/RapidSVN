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
#if defined( _MSC_VER) && _MSC_VER <= 1200
#pragma warning( disable: 4786 )// debug symbol truncated
#endif

// stl
#include <string>

// subversion api
#include "svn_client.h"

// svncpp
#include "client.hpp"
#include "exception.hpp"
#include "pool.hpp"
#include "targets.hpp"

namespace svn
{
  void
  Client::checkout (const char * url, 
                    const Path & destPath, 
                    const Revision & revision, 
                    bool recurse) throw (ClientException)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();

    svn_error_t * error =
      svn_client_checkout (url, destPath.c_str (),
                           revision.revision (),
                           recurse,
                           *m_context,
                           apr_pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Client::remove (const Path & path, 
                  bool force) throw (ClientException)
  {
    Pool pool;
    Targets targets (path.c_str ());
    svn_client_commit_info_t *commit_info = NULL;

    svn_error_t * error =
      svn_client_delete (&commit_info, 
                         const_cast<apr_array_header_t*> (targets.array (pool)), 
                         force,
                         *m_context,
                         pool);
    if(error != NULL)
      throw ClientException (error);
  }

  void
  Client::remove (const Targets & targets, 
                  bool force) throw (ClientException)
  {
    Pool pool;
    svn_client_commit_info_t *commit_info = NULL;

    svn_error_t * error =
      svn_client_delete (&commit_info, 
                         const_cast<apr_array_header_t*> (targets.array (pool)), 
                         force,
                         *m_context,
                         pool);
    if(error != NULL)
      throw ClientException (error);
  }

  void
  Client::revert (const Path & path, 
                  bool recurse) throw (ClientException)
  {
    Pool pool;

    svn_error_t * error =
      svn_client_revert (path.c_str (), 
                         recurse, 
                         *m_context,
                         pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Client::add (const Path & path, 
               bool recurse) throw (ClientException)
  {
    Pool pool;

    svn_error_t * error =
      svn_client_add (path.c_str (), 
                      recurse, 
                      *m_context,
                      pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Client::update (const Path & path, 
                  const Revision & revision, 
                  bool recurse) throw (ClientException)
  {
    Pool pool;
    svn_error_t * error =
      svn_client_update (path.c_str (),
                         revision.revision (),
                         recurse,
                         *m_context,
                         pool);
    if(error != NULL)
      throw ClientException (error);
  }

  svn_revnum_t
  Client::commit (const Targets & targets, const char * message, 
                  bool recurse) throw (ClientException)
  {
    Pool pool;

    m_context->setLogMessage (message);

    svn_client_commit_info_t *commit_info = NULL;
    svn_error_t * error =
      svn_client_commit (&commit_info, 
                         targets.array (pool), 
                         !recurse, 
                         *m_context,
                         pool);
    if (error != NULL)
      throw ClientException (error);

    if (commit_info && SVN_IS_VALID_REVNUM (commit_info->revision))
      return commit_info->revision;

    return -1;
  }

  void
  Client::copy (const Path & srcPath, 
                const Revision & srcRevision, 
                const Path & destPath) throw (ClientException)
  {
    Pool pool;
    svn_client_commit_info_t *commit_info = NULL;
    svn_error_t * error = 
      svn_client_copy (&commit_info,
                       srcPath.c_str (),
                       srcRevision.revision (),
                       destPath.c_str (),
                       *m_context,
                       pool);
  
    if(error != NULL)
      throw ClientException (error);
  }

  void
  Client::move (const Path & srcPath, 
                const Revision & srcRevision, 
                const Path & destPath, 
                bool force) throw (ClientException)
  {
    Pool pool;
    svn_client_commit_info_t *commit_info = NULL;
    svn_error_t * error =  
      svn_client_move (&commit_info,
                       srcPath.c_str (),
                       srcRevision.revision (),
                       destPath.c_str (),
                       force,
                       *m_context,
                       pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Client::mkdir (const Path & path, 
                 const char * message) throw (ClientException)
  {
    Pool pool;
    m_context->setLogMessage (message);

    Targets targets (path.c_str ());

    svn_client_commit_info_t *commit_info = NULL;
    svn_error_t * error =  
      svn_client_mkdir (&commit_info, 
                        const_cast<apr_array_header_t*> 
                        (targets.array (pool)),
                        *m_context, pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Client::mkdir (const Targets & targets, 
                 const char * message) throw (ClientException)
  {
    Pool pool;
    m_context->setLogMessage (message);

    svn_client_commit_info_t *commit_info = NULL;
    svn_error_t * error =  
      svn_client_mkdir (&commit_info, 
                        const_cast<apr_array_header_t*> 
                        (targets.array (pool)),
                        *m_context, pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Client::cleanup (const Path & path) throw (ClientException)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();

    svn_error_t * error =  
      svn_client_cleanup (path.c_str (), *m_context, apr_pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Client::resolved (const Path & path, 
                    bool recurse) throw (ClientException)
  {
    Pool pool;
    svn_error_t * error =  
      svn_client_resolved (path.c_str (),
                           recurse,
                           *m_context,
                           pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Client::doExport (const Path & srcPath, 
                    const Path & destPath, 
                    const Revision & revision, 
                    bool force) throw (ClientException)
  {
    Pool pool;
    svn_error_t * error =  
      svn_client_export (srcPath.c_str (),
                         destPath.c_str (),
                         const_cast<svn_opt_revision_t*>
                         (revision.revision ()),
                         force,
                         *m_context,
                         pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Client::doSwitch (const Path & path, 
                    const char * url, 
                    const Revision & revision, 
                    bool recurse) throw (ClientException)
  {
    Pool pool;
    svn_error_t * error =  
      svn_client_switch (path.c_str (),
                         url,
                         revision.revision (),
                         recurse,
                         *m_context,
                         pool);
    
    if(error != NULL)
      throw ClientException (error);
  }

  void
  Client::import (const Path & path, 
                  const char * url, 
                  const char * message, 
                  bool recurse) throw (ClientException)
  {
    Pool pool;
    svn_client_commit_info_t *commit_info = NULL;

    m_context->setLogMessage (message);

    svn_error_t * error =  
      svn_client_import (&commit_info,
                         path.c_str (),
                         url,
                         !recurse,
                         *m_context,
                         pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Client::merge (const Path & path1, const Revision & revision1, 
                 const Path & path2, const Revision & revision2,
                 const Path & localPath, bool force, 
                 bool recurse,
                 bool notice_ancestry,
                 bool dry_run) throw (ClientException)
  {
    Pool pool;
    svn_error_t * error =  
      svn_client_merge (path1.c_str (),
                        revision1.revision (),
                        path2.c_str (),
                        revision2.revision (),
                        localPath.c_str (),
                        recurse,
                        !notice_ancestry,
                        force,
                        dry_run,
                        *m_context,
                        pool);

    if(error != NULL)
      throw ClientException (error);
  }

  void
  Client::relocate (const char * from_url, 
                    const char * to_url, 
                    const Path & path,
                    bool recurse) throw (ClientException)
  {
    Pool pool;
    svn_error_t * error =  
      svn_client_relocate (from_url,
                         to_url,
                         path.c_str (),
                         recurse,
                         *m_context,
                         pool);
    
    if(error != NULL)
      throw ClientException (error);
  }

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
