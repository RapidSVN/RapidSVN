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

// svncpp
#include "svncpp/status.hpp"

//#include <assert.h>

namespace svn
{
  Status::Status (const Status & src)
    : m_status (0), m_path (0)
  {
    if( &src != this )
    {
      init (src.m_path->data, src.m_status);
    }
  }

  Status::Status (const char *path, svn::SvnStatus * status)
    : m_status (0), m_path (0)
  {
    init (path, status);
  }

  Status::~Status ()
  {
  }

  void Status::init (const char *path, const svn::SvnStatus * status)
  {
    if (path)
    {
      m_path = svn_string_create (path, m_pool);
    }
    else
    {
      m_path = svn_string_create ("", m_pool);
    }

    m_status = (svn::SvnStatus *)
      apr_pcalloc (m_pool, sizeof (svn::SvnStatus));

    if (!status)
    {
      m_isVersioned = false;
    }
    else
    {
      m_isVersioned = status->text_status > svn_wc_status_unversioned;

      // now duplicate the contents
      if (status->entry)
        m_status->entry = svn_wc_entry_dup (status->entry, m_pool);

      m_status->text_status = status->text_status;
      m_status->prop_status = status->prop_status;
      m_status->locked = status->locked;
      m_status->copied = status->copied;
      m_status->switched = status->switched;
      m_status->repos_text_status = status->repos_text_status;
      m_status->repos_prop_status = status->repos_prop_status;

      // duplicate the contents of repos_lock structure
#if CHECK_SVN_SUPPORTS_LOCK
      if (status->repos_lock)
        m_status->repos_lock = svn_lock_dup (status->repos_lock, m_pool);
#endif
    }
  }

  Status &
  Status::operator=(const Status & status)
  {
    if (this == &status)
      return *this;

    init (status.m_path->data, status.m_status);
    return *this;
  }

  const bool
  Status::isLocked () const
  {
#if CHECK_SVN_SUPPORTS_LOCK
    if (m_status->repos_lock)
      return m_status->repos_lock->token != 0;
    else
      return false;
#else
    return 0;
#endif
  }

  const char *
  Status::lockToken () const
  {
#if CHECK_SVN_SUPPORTS_LOCK
    if (m_status->repos_lock)
      return m_status->repos_lock->token;
    else
      return "";
#else
    return "";
#endif
  }

  const char *
  Status::lockOwner () const
  {
#if CHECK_SVN_SUPPORTS_LOCK
    if (m_status->repos_lock)
      return m_status->repos_lock->owner;
    else
      return "";
#else
    return "";
#endif
  }

  const char *
  Status::lockComment () const
  {
#if CHECK_SVN_SUPPORTS_LOCK
    if (m_status->repos_lock)
      return m_status->repos_lock->comment;
    else
      return "";
#else
    return "";
#endif
  }

  const apr_time_t
  Status::lockCreationDate () const
  {
#if CHECK_SVN_SUPPORTS_LOCK
    if (m_status->repos_lock)
      return m_status->repos_lock->creation_date;
    else
      return 0;
#else
    return 0;
#endif
  }
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
