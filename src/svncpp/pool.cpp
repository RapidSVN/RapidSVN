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

#include "pool.h"
#include "svn_pools.h"

/**
 * SvnCpp namespace
 */
namespace svn
{
  Pool::Pool ()
  {
    m_pool = svn_pool_create (NULL);
  }

  Pool::Pool (apr_pool_t * parent)
  {
    m_pool = svn_pool_create (parent);
  }

    /*
  Pool::Pool (Pool * pool)
  {
    if (pool != this)
    {
      m_pool = pool->pool ();
    }
  }
    */

  Pool::~Pool ()
  {
    if(m_pool)
    {
      svn_pool_destroy (m_pool);
    }
  }

  apr_pool_t *Pool::pool ()
  {
    return m_pool;
  }
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
