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

#include "pool.h"
#include "svn_pools.h"

/**
 * SvnCpp namespace
 */
namespace svn
{
  Pool::Pool ()
    : m_pool(NULL)
  {
  }

  Pool::Pool (apr_pool_t * parent)
    : m_pool(NULL)
  {
    Create(parent);
  }

  Pool::Pool (Pool & parent)
    : m_pool(NULL)
  {
    Create(parent);
  }

  void
  Pool::Create (apr_pool_t * parent)
  {
    if( m_pool )
    {
      svn_pool_destroy (m_pool);
    }

    m_pool = svn_pool_create (parent);
  }

  void
  Pool::Create (Pool & parent)
  {
    Create( parent.pool() );
  }

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
