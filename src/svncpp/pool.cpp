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

// svncpp
#include "pool.hpp"

/**
 * SvnCpp namespace
 */
namespace svn
{
  static bool m_initialized = false;

  inline static apr_pool_t * 
  pool_create (apr_pool_t * parent)
  {
    // CAUTION: this is not thread-safe!!!
    if (!m_initialized)
    {
      m_initialized = true;
      apr_pool_initialize ();
    }

    return svn_pool_create (parent);
  }

  Pool::Pool (apr_pool_t * parent)
    : m_parent (parent), m_pool (pool_create (parent))
  {
  }

  Pool::~Pool ()
  {
    if(m_pool)
    {
      svn_pool_destroy (m_pool);
    }
  }

  apr_pool_t *
  Pool::pool () const
  {
    return m_pool;
  }

  void
  Pool::renew ()
  {
    if (m_pool)
    {
      svn_pool_destroy (m_pool);
    }
    m_pool = pool_create (m_parent);
  }

//TODO
//   apr_pool_t *
//   Pool::operator=(const Pool & pool)
//   {
//     return 
//     if (this == &path)
//       return *this;
//     m_path = path.c_str();
//     return *this;
//   }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
