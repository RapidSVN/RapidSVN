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
#include "status.hpp"

namespace svn
{


  Status::Status ()
  {
    init (NULL, NULL);
  }

  Status::Status (const Status & src)
  {
    init (src.m_path.c_str (), src.m_status);
  }


  Status::Status (const char *path, svn_wc_status_t * status)
  {
    init (path, status);
  }

  void Status::init (const char *path, const svn_wc_status_t * status)
  {
    m_path = path;
    m_status = status;
    m_versioned = FALSE;
    m_isDir = FALSE;
    m_isCopied = FALSE;
    m_isLocked = FALSE;
    m_revision = SVN_INVALID_REVNUM;
    m_lastChanged = 0;
    m_lastCommitAuthor = "";
    m_textType = svn_wc_status_none;
    m_propType = svn_wc_status_none;

    if (status != NULL)
    {
      svn_wc_entry_t *entry = status->entry;
      m_versioned = status->text_status > svn_wc_status_unversioned;
      m_textType = status->text_status;
      m_propType = status->prop_status;
      m_isCopied = status->copied == 1;
      m_isLocked = status->locked == 1;

      if (entry != NULL)
      {
        m_revision = entry->revision;
        m_lastChanged = entry->cmt_rev;
        if(entry->cmt_author) // if just added then no author
          m_lastCommitAuthor = entry->cmt_author;
        m_isDir = entry->kind == svn_node_dir;
      }
    }
  }

  Status::~Status ()
  {
  }

  const char *Status::statusDescription (svn_wc_status_kind kind)
  {
    switch (kind)
    {
    case svn_wc_status_none:
      return "non-svn";
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
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
