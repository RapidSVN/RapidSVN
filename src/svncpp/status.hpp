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
#ifndef _SVNCPP_STATUS_HPP_
#define _SVNCPP_STATUS_HPP_

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
    bool m_isVersioned;
    std::string m_path;
    bool m_isDir;
    bool m_isCopied;
    bool m_isLocked;
    svn_revnum_t m_revision;
    unsigned long m_lastChanged;
    std::string m_lastCommitAuthor;
    svn_wc_status_kind m_textType;
    svn_wc_status_kind m_propType;
    svn_wc_status_t * m_status;

    /**
     * Returns the description of the status.
     */
    const char *
    statusDescription (const svn_wc_status_kind kind) const;

    /**
     * Initialize structures
     *
     * @param path
     * @param status if NULL isVersioned will be false
     */
    void 
    init (const char *path, const svn_wc_status_t * status);

  public:

    /**
     * copy constructor
     */
    Status (const Status & src);

    /**
     * Constructor
     *
     */
    Status (const char *path = NULL, svn_wc_status_t * status = NULL);

    /**
     * destructor
     */
    virtual ~Status ();

    /**
     * @return path of status entry
     */
    const char *
    path () const
    {
      return m_path.c_str ();
    }

    /**
     * @return TRUE if entry is a dir
     */
    const bool 
    isDir () const
    {
      return m_isDir;
    }

    /**
     * @return revision if versioned, otherwise SVN_INVALID_REVNUM
     */
    const svn_revnum_t 
    revision () const
    {
      return m_revision;
    }

    /**
     * Returns the last time the file was changed revision number.
     */
    const unsigned long 
    lastChanged () const
    {
      return m_lastChanged;
    }

    /**
     * @return name of author if versioned, NULL otherwise
     */
    const char *
    lastCommitAuthor () const
    {
      return m_lastCommitAuthor.c_str ();
    }

    /**
     * @return file status of the "textual" component. 
     */
    const char *
    textDescription () const
    {
      return statusDescription (m_textType);
    }

    /**
     * @return file status property enum of the "textual" component. 
     */
    const svn_wc_status_kind 
    textType () const
    {
      return m_textType;
    }

    /**
     * @return textual file status of the "property" component. 
     */
    const char *
    propDescription () const
    {
      return statusDescription (m_propType);
    }

    /**
     * @return file status property enum of the "property" component. 
     */
    const svn_wc_status_kind 
    propType () const
    {
      return m_propType;
    }

    /**
     * @return TRUE if under version control
     */
    const bool 
    isVersioned () const
    {
      return m_isVersioned;
    }

    /**
     * @return TRUE if locked
     */
    const bool 
    isLocked () const
    {
      return m_isLocked;
    }

    /**
     * @return TRUE if copied
     */
    const bool 
    isCopied () const
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
