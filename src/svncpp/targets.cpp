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

// stl
#include <vector>

// subversion api

// svncpp
#include "targets.hpp"
#include "path.hpp"
#include "pool.hpp"

namespace svn
{
    Targets::Targets(const std::vector<Path> & targets)
    {
      m_targets = targets;
    }
    
    Targets::Targets(const apr_array_header_t * targets)
    {
      // TODO
    }

    Targets::Targets(const Targets & targets)
    {
      m_targets = targets.targets ();
    }

    const apr_array_header_t *
    Targets::array(const Pool pool) const
    {
      //TODO
    }

    const std::vector<Path> &
    Targets::targets() const
    {
      return m_targets;
    }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
