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

#ifndef _SVNCPP_TARGETS_HPP_
#define _SVNCPP_TARGETS_HPP_

// stl
#include <vector>

// forward declarations
typedef struct apr_array_header_t;
typedef struct apr_hash_t;

namespace svn
{
  // forward declarations
  class Path;
  class Pool;

  /**
   * Encapsulation for Subversion target arrays handling
   */
  class Targets
  {
  private:
    std::vector<Path> m_targets;

  public:
    /** 
     * Constructor
     *
     * @param targets vector of paths
     */
    Targets (const std::vector<Path> & targets);
    
    /**
     * Constructor from an APR array containing
     * char *.
     *
     * @param targets APR array header
     */
    Targets (const apr_array_header_t * targets);

    /**
     * Constructor. Initializes list with just
     * one entry
     *
     * @param target
     */
    Targets (const char * target );

    /**
     * Constructor from an APR hash containing
     * char *.
     *
     * @param hash APR hash
     * @param pool Pool needed for conversions
     */
    Targets (const apr_hash_t * hash, const Pool pool);

    /**
     * Copy Constructor
     *
     * @param targets Source
     */
    Targets (const Targets & targets);

    /**
     * Returns an apr array containing
     * char *.
     *
     * @param pool Pool used for conversion
     */
    const apr_array_header_t *
    array (const Pool & pool) const;

    /**
     * Returns a vector of paths
     *
     * @return vector of paths
     */
    const std::vector<Path> &
    targets() const;
  };
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
