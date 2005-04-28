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
#ifndef _TRACE_UPDATE_EDITOR_H_INCLUDED_
#define _TRACE_UPDATE_EDITOR_H_INCLUDED_

// subversion
#include "svn_delta.h"
#include "svn_pools.h"
#include "svn_types.h"

// app

// forward declarations
class Tracer;

svn_error_t *get_trace_update_editor (const svn_delta_editor_t ** editor,
                                      void **edit_baton,
                                      const char *initial_path,
                                      svn_boolean_t is_checkout,
                                      svn_boolean_t no_final_line,
                                      Tracer * tracer, apr_pool_t * pool);

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
