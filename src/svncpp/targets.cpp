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

// stl
#include <vector>

// subversion api
#include "svn_types.h"

// apr api
#include "apr_pools.h"
#include "apr_strings.h"

// svncpp
#include "svncpp/targets.hpp"
#include "svncpp/path.hpp"
#include "svncpp/pool.hpp"


namespace svn
{
  Targets::Targets (const std::vector<Path> & targets)
  {
    m_targets = targets;
  }
  
  Targets::Targets (const apr_array_header_t * apr_targets)
  {
    int i;
    
    m_targets.clear ();
    m_targets.reserve (apr_targets->nelts);
    
    for (i = 0; i < apr_targets->nelts; i++)
    {
      const char ** target = 
        &APR_ARRAY_IDX (apr_targets, i, const char *);
      
      m_targets.push_back (Path (*target));
    }
  }
  
  Targets::Targets (const Targets & targets)
  {
    m_targets = targets.targets ();
  }
  
  Targets::Targets (const char * target)
  {
    if (target != 0)
    {
      m_targets.push_back (target);
    }
  }

  Targets::~Targets ()
  {
  }

  const apr_array_header_t *
  Targets::array (const Pool & pool) const
  {
    std::vector<Path>::const_iterator it;
    
    apr_pool_t *apr_pool = pool.pool ();
    apr_array_header_t *apr_targets = 
      apr_array_make (apr_pool,
                      m_targets.size(),
                      sizeof (const char *));
    
    for (it = m_targets.begin (); it != m_targets.end (); it++)
    {
      const Path &path = *it;
      const char * target =
        apr_pstrdup (apr_pool, path.c_str());
      
      (*((const char **) apr_array_push (apr_targets))) = target;
    }
    
    return apr_targets;
  }
  
  const std::vector<Path> &
  Targets::targets () const
  {
    return m_targets;
  }

  size_t 
  Targets::size () const
  {
    return m_targets.size ();
  }

  const Path
  Targets::target () const
  {
    if (m_targets.size () > 0)
    {
      return m_targets[0];
    }
    else
    {
      return "";
    }
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
