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
    Targets(const std::vector<Path> & targets);
    
    Targets(const apr_array_header_t * targets);

    Targets(const Targets & targets);

    const apr_array_header_t *
    array(const Pool pool) const;

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
