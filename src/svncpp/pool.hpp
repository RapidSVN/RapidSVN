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
     * creates new instance, but no pool.
     * You will have to call "Create" next.
     */
    Pool ();

    /**
     * creates a subpool new pool to an existing pool
     */
    Pool (apr_pool_t * parent);

    virtual ~ Pool ();

    /**
     * copy constructor create a new subpool to
     * the existing pool
     */
    Pool (Pool&);

    /**
     * returns the handle to the pool
     */
    apr_pool_t *pool ();

    /**
     * create a new pool
     *
     * @param parent NULL: parent pool. If NULL this
     *               creates a global pool, otherwise
     *               a subpool
     */
    void Create (apr_pool_t * parent);

    /**
     * create a subpool to the existing pool
     */
    void Create (Pool & parent);

  private:
    apr_pool_t * m_pool;
    Pool& operator=(const Pool&);
  };
}

#endif

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
