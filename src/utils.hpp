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
#ifndef _RAPIDSVN_UTILS_H_INCLUDED_
#define _RAPIDSVN_UTILS_H_INCLUDED_

// svn
#include "svn_wc.h"

// forward declarations
class wxString;
class Tracer;
class wxMenu;

#define DEFAULT_ARRAY_SIZE 5

/**
 * Create pseudo-unix paths on windows for use with svn (reverse backslashes,
 * but leave the rest as it is)
 *
 * NOTE: Does nothing on non-windows platforms
 *
 * @param path Native path to convert
 * @return string with all backslashes converted to forward slashes
 */
wxString & UnixPath (wxString & path);

/**
 * Get a status string describing the given svn status
 */
void GetStatusText (wxString & str, svn_wc_status_kind st);

/**
 * Trim whitespace at start and end of string
 * (Convenience function for trimming first left, then right)
 */
void TrimString (wxString & str);

void *svn_cl__make_log_msg_baton (const char *message,
                                  const char *base_dir, apr_pool_t * pool);

/**
 * Recursive function to send the error strings to a Tracer
 */
void handle_svn_error (svn_error_t * err, Tracer * tracer);

svn_error_t *svn_cl__may_need_force (svn_error_t * err);

/**
 * Post a menu command event with the given ID. 
 *
 * Used for converting non-command events to command events so they'll move up
 * the GUI hierarchy.
 */
bool PostMenuEvent (wxEvtHandler *source, long id);

/**
 * Create a correctly sized button with an ellipsis (three dots)
 */
wxButton *CreateEllipsisButton(wxWindow *parent, long id);

/**
 * Append entries for the "Modify" menu
 *
 * @param parentMenu menu that will receive the items
 */
void AppendModifyMenu (wxMenu * parentMenu);

/**
 * Append entries for the "Query" menu
 *
 * @param parentMenu menu that will receive the items
 */
void AppendQueryMenu (wxMenu * parentMenu);

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
