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

// subversion api
#include "svn_wc.h"

// svncpp
#include "path.hpp"
#include "pool.hpp"
#include "wc.hpp"

namespace svn
{
  const char * Wc::ADM_DIR_NAME = SVN_WC_ADM_DIR_NAME;

  bool 
  Wc::checkWc (const char * dir)
  {
    Pool pool;
    Path path (dir);
    int wc;

    svn_error_t * error =
      svn_wc_check_wc (path.c_str (), &wc, pool);

    if ((error != NULL) || (wc == 0))
    {
      return false;
    }

    return true;
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
