/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library (in the file LGPL.txt); if not, 
 * write to the Free Software Foundation, Inc., 51 Franklin St, 
 * Fifth Floor, Boston, MA  02110-1301  USA
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
#include "svncpp/check.hpp"
#include "svncpp/client.hpp"
#include "svncpp/exception.hpp"
#include "svncpp/pool.hpp"
#include "svncpp/targets.hpp"
#include "m_check.hpp"

#ifndef CHECK_SVN_VERSION
#error "CHECK_SVN_VERSION not defined"
#endif

#ifndef CHECK_SVN_SUPPORTS_LOCK
#error "CHECK_SVN_SUPPORTS_LOCK not defined"
#endif


namespace svn
{
  svn_revnum_t
  Client::checkout (const char * url, 
                    const Path & destPath, 
                    const Revision & revision, 
                    bool recurse) throw (ClientException)
  {
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();

    svn_revnum_t revnum = 0;
    svn_error_t * error =
      svn_client_checkout (&revnum,
                           url, destPath.c_str (),
                           revision.revision (),
                           recurse,
                           *m_context,
                           apr_pool);

    if(error != NULL)
      throw ClientException (error);
    return revnum;
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

  /**
   * Using new LOCK feature in subversion 1.2 and up
   */
#if CHECK_SVN_SUPPORTS_LOCK
  void
  Client::lock (const Targets & targets, bool force,
                const char * comment) throw (ClientException)
  {
    Pool pool;

    svn_error_t * error =
      svn_client_lock (const_cast<apr_array_header_t*> (targets.array (pool)), 
                       comment,
                       force,
                       *m_context,
                       pool);
    if(error != NULL)
      throw ClientException (error);
  }
#else
  void
  Client::lock(const Targets &, bool, const char *) throw (ClientException)
  {
    throw ClientException ("Client::lock not supported");
  }
#endif

#if CHECK_SVN_SUPPORTS_LOCK
  void
  Client::unlock (const Targets & targets, bool force) throw (ClientException)
  {
    Pool pool;

    svn_error_t * error =
      svn_client_unlock (const_cast<apr_array_header_t*> (targets.array (pool)), 
                       force,
                       *m_context,
                       pool);
    if(error != NULL)
      throw ClientException (error);
  }
#else 
  void
  Client::unlock(const Targets &, bool force) throw (ClientException)
  {
    throw ClientException ("Client::unlock not supported");
  }
#endif

  void
  Client::revert (const Targets & targets,
                  bool recurse) throw (ClientException)
  {
    Pool pool;

    svn_error_t * error =
      svn_client_revert ((targets.array (pool)), 
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

  svn_revnum_t
  Client::update (const Path & path, 
                  const Revision & revision, 
                  bool recurse) throw (ClientException)
  {
    Pool pool;
    svn_revnum_t revnum = 0;
    svn_error_t * error =
      svn_client_update (&revnum,
                         path.c_str (),
                         revision.revision (),
                         recurse,
                         *m_context,
                         pool);
    if(error != NULL)
      throw ClientException (error);
    return revnum;
  }

  /**
   * Using new functions and features in subversion 1.2 and up
   */
#if CHECK_SVN_SUPPORTS_LOCK
  svn_revnum_t
  Client::commit (const Targets & targets, const char * message, 
                  bool recurse, bool keepLocks) throw (ClientException)
  {
    Pool pool;

    m_context->setLogMessage (message);

    svn_client_commit_info_t *commit_info = NULL;

    svn_error_t * error =
      svn_client_commit2 (&commit_info, 
                         targets.array (pool), 
                         recurse, 
                         keepLocks,
                         *m_context,
                         pool);
    if (error != NULL)
      throw ClientException (error);

    if (commit_info && SVN_IS_VALID_REVNUM (commit_info->revision))
      return commit_info->revision;

    return -1;
  }
  /**
   * Compatibility to older versions of subversion
   */
#else
  svn_revnum_t
  Client::commit (const Targets & targets, const char * message, 
                  bool recurse, bool keepLocks) throw (ClientException)
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
#endif

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

#if CHECK_SVN_VERSION(1,2)
  void
  Client::move (const Path & srcPath, 
                const Revision & srcRevision, 
                const Path & destPath, 
                bool force) throw (ClientException)
  {
    Pool pool;
    svn_client_commit_info_t *commit_info = NULL;
    svn_error_t * error =  
      svn_client_move2 (&commit_info,
                       srcPath.c_str (),
                       destPath.c_str (),
                        force,
                       *m_context,
                       pool);

    if(error != NULL)
      throw ClientException (error);
  }
#else
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
#endif

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

  svn_revnum_t
  Client::doExport (const Path & srcPath, 
                    const Path & destPath, 
                    const Revision & revision, 
                    bool force) throw (ClientException)
  {
    Pool pool;
    svn_revnum_t revnum = 0;
    svn_error_t * error =  
      svn_client_export (&revnum,
                         srcPath.c_str (),
                         destPath.c_str (),
                         const_cast<svn_opt_revision_t*>
                         (revision.revision ()),
                         force,
                         *m_context,
                         pool);

    if(error != NULL)
      throw ClientException (error);
    return revnum;
  }

  svn_revnum_t
  Client::doSwitch (const Path & path, 
                    const char * url, 
                    const Revision & revision, 
                    bool recurse) throw (ClientException)
  {
    Pool pool;
    svn_revnum_t revnum = 0;
    svn_error_t * error =  
      svn_client_switch (&revnum,
                         path.c_str (),
                         url,
                         revision.revision (),
                         recurse,
                         *m_context,
                         pool);
    
    if(error != NULL)
      throw ClientException (error);
    return revnum;
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
  Client::relocate (const Path & path,
                    const char * from_url, 
                    const char * to_url, 
                    bool recurse) throw (ClientException)
  {
    Pool pool;
    svn_error_t * error =  
      svn_client_relocate (path.c_str (),
                         from_url,
                         to_url,
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
