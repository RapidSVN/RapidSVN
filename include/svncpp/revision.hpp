/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#ifndef _SVNCPP_REVISION_HPP_
#define _SVNCPP_REVISION_HPP_ 

// subversion api
#include "svn_types.h"
#include "svn_opt.h"

// svncpp
#include "datetime.hpp"

namespace svn
{
  /**
   * Class that encapsulates svn_opt_revnum_t.
   *
   * @see svn_opt_revnum_t
   */
  class Revision
  {
  private:
    svn_opt_revision_t m_revision;

    void
    init (const svn_opt_revision_t * revision);

  public:
    static const svn_opt_revision_kind START;
    static const svn_opt_revision_kind HEAD;

    /**
     * Constructor
     *
     * @param revision revision information
     */
    Revision (const svn_opt_revision_t * revision);

    /**
     * Constructor
     *
     * @param revnum revision number
     */
    Revision (const svn_revnum_t revnum);

    /**
     * Constructor
     *
     * @param kind
     */
    Revision (const svn_opt_revision_kind kind = svn_opt_revision_unspecified);

    /**
     * Constructor
     *
     * @param datetime DateTime wrapper for apr_time_t
     */
    Revision (const DateTime dateTime);

    /**
     * Copy constructor
     *
     * @param revision Source
     */
    Revision (const Revision & revision);

    /**
     * @return revision information
     */
    const svn_opt_revision_t *
    revision () const;

    /**
     * @see revision (). Same function
     * but with operator overloading
     */
    operator svn_opt_revision_t * ()
    {
      return &m_revision;
    }

    /**
     * @return revision numver
     */
    const svn_revnum_t 
    revnum () const;

    /**
     * @return revision kind
     */
    const svn_opt_revision_kind 
    kind () const;

    /**
     * @return date
     */
    const apr_time_t
    date () const;
  };
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
