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
#ifndef _TRACE_COMMIT_EDITOR_H_INCLUDED_
#define _TRACE_COMMIT_EDITOR_H_INCLUDED_

/**
* Constructs an editor that prints out events in a commit. 
*/
svn_error_t *get_trace_commit_editor (const svn_delta_editor_t ** editor,
                                      void **edit_baton,
                                      const char *initial_path,
                                      Tracer * tr, apr_pool_t * pool);


#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
