/*
 * ====================================================================
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _SVNCPP_STATUS_H_
#define _SVNCPP_STATUS_H_

#include "svn_types.h"
#include "svn_wc.h"
#include <string>

namespace svn
{
/**
 * Subversion status API.
 */
  class Status
  {
  private:
    const svn_wc_status_t *m_status;
    bool m_versioned;
     std::string m_path;
    bool m_isDir;
    bool m_isCopied;
    bool m_isLocked;
    unsigned long m_revision;
    unsigned long m_lastChanged;
     std::string m_lastCommitAuthor;
    svn_wc_status_kind m_textType;
    svn_wc_status_kind m_propType;

  /**
   * Returns the description of the status.
   */
    const char *statusDescription (svn_wc_status_kind kind);

  public:
  /**
   * Initialize structures
   */
    void init (const char *path, const svn_wc_status_t * status);

     Status ();

  /**
   * copy constructor
   */
     Status (const Status & src);

     Status (const char *path, svn_wc_status_t * status = NULL);

    ~Status ();

  /**
   * @return path of status entry
   */
    const char *path ()
    {
      return m_path.c_str ();
    }

  /**
   * @return TRUE if entry is a dir
   */
    bool isDir ()
    {
      return m_isDir;
    }

  /**
   * @return revision if versioned, otherwise SVN_INVALID_REVNUM
   */
    unsigned long revision ()
    {
      return m_revision;
    }

  /**
   * Returns the last time the file was changed revision number.
   */
    unsigned long lastChanged ()
    {
      return m_lastChanged;
    }

  /**
   * @return name of author if versioned, NULL otherwise
   */
    const char *lastCommitAuthor ()
    {
      return m_lastCommitAuthor.c_str ();
    }


  /**
   * @return file status of the "textual" component. 
   */
    const char *textDescription ()
    {
      return statusDescription (m_textType);
    }

  /**
   * @return file status property enum of the "textual" component. 
   */
    const svn_wc_status_kind textType ()
    {
      return m_textType;
    }

  /**
   * @return textual file status of the "property" component. 
   */
    const char *propDescription ()
    {
      return statusDescription (m_propType);
    }

  /**
   * @return file status property enum of the "property" component. 
   */
    const svn_wc_status_kind propType ()
    {
      return m_propType;
    }

  /**
   * @return TRUE if under version control
   */
    bool isVersioned ()
    {
      return m_versioned;
    }

  /**
   * @return TRUE if locked
   */
    bool isLocked ()
    {
      return m_isLocked;
    }

  /**
   * @return TRUE if copied
   */
    bool isCopied ()
    {
      return m_isCopied;
    }
  };

}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
