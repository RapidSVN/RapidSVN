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

#ifndef _SVNCPP_POOL_HPP_
#define _SVNCPP_POOL_HPP_

#include "pool.hpp"

/**
 * SvnCpp namespace
 */
namespace svn
{
  Pool::Pool (apr_pool_t * parent)
    : m_pool (svn_pool_create (parent))
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
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
