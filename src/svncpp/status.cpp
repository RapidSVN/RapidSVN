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
 * @file status.cpp
 */

// svncpp
#include "status.hpp"

namespace svn
{
  Status::Status (const Status & src)
  {
    if( &src != this )
    {
      init (src.m_path, src.m_status);
    }
  }

  Status::Status (const char *path, svn_wc_status_t * status)
  {
    init (path, status);
  }

  Status::~Status ()
  {
  }

  void Status::init (const char *path, const svn_wc_status_t * status)
  {
    m_status = (svn_wc_status_t *)
      apr_pcalloc (m_pool, sizeof (svn_wc_status_t));
    if (!status)
    {
      m_isVersioned = false;
    }
    else
    {
      m_isVersioned = status->text_status > svn_wc_status_unversioned;
      // now duplicate the contents
      if (status->entry)
      {
        m_status->entry = svn_wc_entry_dup (status->entry, m_pool);
      }
      m_status->text_status = status->text_status;
      m_status->prop_status = status->prop_status;
      m_status->locked = status->locked;
      m_status->copied = status->copied;
      m_status->switched = status->switched;
      m_status->repos_text_status = status->repos_text_status;
      m_status->repos_prop_status = status->repos_prop_status;
    }
  }

  const char *
  Status::statusDescription (const svn_wc_status_kind kind) const
  {
    switch (kind)
    {
    case svn_wc_status_none:
      return "none";
      break;
    case svn_wc_status_normal:
      return "normal";
      break;
    case svn_wc_status_added:
      return "added";
      break;
    case svn_wc_status_absent:
      return "absent";
      break;
    case svn_wc_status_deleted:
      return "deleted";
      break;
    case svn_wc_status_replaced:
      return "replaced";
      break;
    case svn_wc_status_modified:
      return "modified";
      break;
    case svn_wc_status_merged:
      return "merged";
      break;
    case svn_wc_status_conflicted:
      return "conflicted";
      break;
    case svn_wc_status_unversioned:
    default:
      return "unversioned";
      break;
    }

  }

  Status &
  Status::operator=(const Status & status)
  {
    if (this == &status)
      return *this;

    init (status.m_path, status.m_status);
    return *this;
  }
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
