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
#include "exception.hpp"
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

  void
  Wc::ensureAdm (const char * dir, const char * url, const Revision & revision)
  {
    Pool pool;
    Path dirPath (dir);
    Path urlPath (url);

    svn_error_t * error =
      svn_wc_ensure_adm (dirPath.c_str (), urlPath.c_str (),
                         revision.revnum (), pool);

    if(error != NULL)
      throw ClientException (error);
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
