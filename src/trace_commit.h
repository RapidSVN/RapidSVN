#ifndef _TRACE_COMMIT_EDITOR_H_INCLUDED_
#define _TRACE_COMMIT_EDITOR_H_INCLUDED_

/**
* Constructs an editor that prints out events in a commit. 
*/
svn_error_t* get_trace_commit_editor( const svn_delta_editor_t **editor,
									void **edit_baton,
									const char *initial_path,
									Tracer *tr,
									apr_pool_t *pool );


#endif