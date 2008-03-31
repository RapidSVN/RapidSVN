/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.  
 * If not, see <http://www.gnu.org/licenses/>.
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
