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
 * @file entry.cpp
 */

// svncpp
#include "entry.hpp"

namespace svn
{
  Entry::Entry (const svn_wc_entry_t * src)
    : m_pool (0), m_entry (0)
  {
    // copy the contents of src
    if (src)
    {
      m_entry = svn_wc_entry_dup (src, m_pool);
    }
    else
    {
      m_entry = (svn_wc_entry_t*)
        apr_pcalloc (m_pool, sizeof (svn_wc_entry_t));
    }
  }

  Entry::~Entry ()
  {
    // no need to explicitely de-allocate m_entry
    // since this will be handle by m_pool
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
