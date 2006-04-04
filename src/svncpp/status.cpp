/*
 * ====================================================================
 * Copyright (c) 2002-2006 The RapidSvn Group.  All rights reserved.
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

#include "m_svn_status.hpp"
#include "m_svn_data.hpp"
#include "m_converter.hpp"

namespace svn
{
  // Status::Data is defined in m_svn_data.hpp

  Status::Status ()
  {
    m = new Data;

    m->status = 0;
    m->path = "";
  }

  Status::Status (const Status & src)
  {
    m = new Data;

    m->status = 0;
    m->path = "";

    if (&src != this)
    {
      m->path = src.m->path;

      m->status = (SvnStatus *)
        apr_pcalloc (m->pool, sizeof (SvnStatus));
  
      if (!src.m->status)
      {
        m->isVersioned = false;
      }
      else
      {
        m->isVersioned = src.m->status->text_status > svn_wc_status_unversioned;
  
        // now duplicate the contents
        if (src.m->status->entry)
          m->status->entry = svn_wc_entry_dup (src.m->status->entry, m->pool);
  
        m->status->text_status = src.m->status->text_status;
        m->status->prop_status = src.m->status->prop_status;
        m->status->locked = src.m->status->locked;
        m->status->copied = src.m->status->copied;
        m->status->switched = src.m->status->switched;
        m->status->repos_text_status = src.m->status->repos_text_status;
        m->status->repos_prop_status = src.m->status->repos_prop_status;
  
        // duplicate the contents of repos_lock structure
#if CHECK_SVN_SUPPORTS_LOCK
        if (src.m->status->repos_lock)
          m->status->repos_lock = svn_lock_dup (src.m->status->repos_lock, m->pool);
#endif
      }
    }
  }

  Status::~Status ()
  {
    delete m;
  }

  const char *
  Status::path () const
  {
    return m->path.c_str ();
  }

  const Entry 
  Status::entry () const
  {
    return Entry (m->status->entry);
  }

  const svn_wc_status_kind 
  Status::textStatus () const
  {
    return m->status->text_status;
  }

  const svn_wc_status_kind 
  Status::propStatus () const
  {
    return m->status->prop_status;
  }

  const bool 
  Status::isVersioned () const
  {
    return m->isVersioned;
  }

  const bool 
  Status::isCopied () const
  {
    return m->status->copied != 0;
  }

  const bool
  Status::isSwitched () const
  {
    return m->status->switched != 0;
  }

  const svn_wc_status_kind
  Status::reposTextStatus () const
  {
    return m->status->repos_text_status;
  }

  const svn_wc_status_kind
  Status::reposPropStatus () const
  {
    return m->status->repos_prop_status;
  }

  const bool
  Status::isLocked () const
  {
#if CHECK_SVN_SUPPORTS_LOCK
    if (m->status->repos_lock && (m->status->repos_lock->token != 0))
      return true;
    else if (m->status->entry)
      return m->status->entry->lock_token != 0;
    else
      return false;
#else
    return false;
#endif
  }

  const bool
  Status::isRepLock () const
  {
#if CHECK_SVN_SUPPORTS_LOCK
    if (m->status->entry && (m->status->entry->lock_token != 0))
      return false;
    else if (m->status->repos_lock && (m->status->repos_lock->token != 0))
      return true;
    else
      return false;
#else
    return false;
#endif
  }

  const char *
  Status::lockToken () const
  {
#if CHECK_SVN_SUPPORTS_LOCK
    if (m->status->repos_lock && m->status->repos_lock->token != 0)
      return m->status->repos_lock->token;
    else if (m->status->entry)
      return m->status->entry->lock_token;
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
    if (m->status->repos_lock && m->status->repos_lock->token != 0)
      return m->status->repos_lock->owner;
    else if (m->status->entry)
      return m->status->entry->lock_owner;
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
    if (m->status->repos_lock && m->status->repos_lock->token != 0)
      return m->status->repos_lock->comment;
    else if (m->status->entry)
      return m->status->entry->lock_comment;
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
    if (m->status->repos_lock && m->status->repos_lock->token != 0)
      return m->status->repos_lock->creation_date;
    else if (m->status->entry)
      return m->status->entry->lock_creation_date;
    else
      return 0;
#else
    return 0;
#endif
  }

  Status &
  Status::operator=(const Status & src)
  {
    if (this == &src)
      return *this;

    delete m;
    m = new Data;

    m->path = src.m->path;
 
    m->status = (SvnStatus *)
      apr_pcalloc (m->pool, sizeof (SvnStatus));

    if (!src.m->status)
    {
      m->isVersioned = false;
    }
    else
    {
      m->isVersioned = src.m->status->text_status > svn_wc_status_unversioned;

      // now duplicate the contents
      if (src.m->status->entry)
        m->status->entry = svn_wc_entry_dup (src.m->status->entry, m->pool);

      m->status->text_status = src.m->status->text_status;
      m->status->prop_status = src.m->status->prop_status;
      m->status->locked = src.m->status->locked;
      m->status->copied = src.m->status->copied;
      m->status->switched = src.m->status->switched;
      m->status->repos_text_status = src.m->status->repos_text_status;
      m->status->repos_prop_status = src.m->status->repos_prop_status;

      // duplicate the contents of repos_lock structure
#if CHECK_SVN_SUPPORTS_LOCK
      if (src.m->status->repos_lock)
      m->status->repos_lock = svn_lock_dup (src.m->status->repos_lock, m->pool);
#endif
    }

    return *this;
  }
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
