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

// svncpp includes
#include "path.hpp"
#include "pool.hpp"

//subversion includes
#include "svn_path.h"

namespace svn
{
  Path::Path (const char * path)
  {
    init (path);
  }

  Path::Path (const std::string & path) 
  {
    init (path.c_str ());
  }

  Path::Path (const Path & path) 
  {
    init (path.c_str ());
  }

  void
  Path::init (const char * path)
  {
    Pool pool;
    const char * int_path = 
      svn_path_internal_style (path, pool.pool () );
    
    m_path = int_path;
  }

  const std::string &
  Path::path () const
  {
    return m_path;
  }

  const char * 
  Path::c_str() const
  {
    return m_path.c_str ();
  }

  Path& 
  Path::operator=(const Path & path)
  {
    return *this;
  }

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
