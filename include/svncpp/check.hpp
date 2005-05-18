/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#ifndef _SVNCPP_CHECK_HPP_
#define _SVNCPP_CHECK_HPP_

// subversion api
#include "svn_version.h"

/**
 * Check if the current version of the subversion
 * API is at least major.minor
 */
#define CHECK_SVN_VERSION (major,minor) \
    (SVN_VER_MAJOR > (major) || \
    (SVN_VER_MAJOR == (major) && SVN_VER_MINOR > (minor))

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
