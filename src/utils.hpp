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
#ifndef _VSVN_UTILS_H_INCLUDED_
#define _VSVN_UTILS_H_INCLUDED_

#include "tracer.hpp"

#define DEFAULT_ARRAY_SIZE 5

#define APPLICATION_NAME _T("RapidSVN")

// Ids used for communication events between threads and frame
#define TOKEN_BASE     100
#define TOKEN_INFO     TOKEN_BASE + 1
#define TOKEN_SVN_INTERNAL_ERROR TOKEN_BASE + 2
#define TOKEN_VSVN_INTERNAL_ERROR TOKEN_BASE + 3
#define TOKEN_ACTION_END   TOKEN_BASE + 4

wxString & UnixPath (wxString & path);

void GetStatusText (wxString & str, svn_wc_status_kind st);

void TrimString (wxString & str);

void *svn_cl__make_log_msg_baton (const char *message,
                                  const char *base_dir, apr_pool_t * pool);

/**
* Recursive function to send the error strings to a Tracer
*/
void handle_svn_error (svn_error_t * err, Tracer * tracer);

svn_error_t *svn_cl__may_need_force (svn_error_t * err);

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
