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

// svncpp
#include "exception.hpp"
#include "path.hpp"
#include "pool.hpp"
#include "utils.hpp"

namespace svn
{
  void
  Utils::cleanup (const Path & path)
  {
    Pool pool;
    svn_error_t * err = svn_wc_cleanup (path.c_str (), 0, 
                                        pool.pool ());
    if (err!=0)
    {
      throw ClientException (err);
    }
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
