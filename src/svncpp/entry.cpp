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
    : m_entry (0), m_pool (0), m_valid (false)
  {
    init (src);
  }

  Entry::Entry (const Entry & src)
    : m_entry (0), m_pool (0), m_valid (false)
  {
    init (src);
  }

  Entry::~Entry ()
  {
    // no need to explicitely de-allocate m_entry
    // since this will be handled by m_pool
  }

  void
  Entry::init (const svn_wc_entry_t * src)
  {
    if (src)
    {
      // copy the contents of src
      m_entry = svn_wc_entry_dup (src, m_pool);
      m_valid = true;
    }
    else
    {
      // create an empty entry
      m_entry = (svn_wc_entry_t*)
        apr_pcalloc (m_pool, sizeof (svn_wc_entry_t));
    }
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
