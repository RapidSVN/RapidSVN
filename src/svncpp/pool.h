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

#ifndef _SVNCPP_POOL_H_
#define _SVNCPP_POOL_H_

/**
 * Forward declarations
 */
struct apr_pool_t;

/**
 * SvnCpp namespace
 */
namespace svn
{
  /**
   * Class for encapsulation of apr/subversion pools
   */
  class Pool
  {
  public:
    /**
     * creates a new pool
     */
    Pool ();

    /**
     * creates a new pool to an existing pool
     */
    Pool (apr_pool_t * parent);

    virtual ~ Pool ();

    /**
     * returns the handle to the pool
     */
    apr_pool_t *pool ();

  private:
     apr_pool_t * m_pool;

     Pool& operator=(const Pool&);
     Pool (const Pool&);
  };
}

#endif

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
