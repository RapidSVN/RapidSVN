#ifndef _VSVN_UTILS_H_INCLUDED_
#define _VSVN_UTILS_H_INCLUDED_

#include "tracer.h"
#include "svn_file_status.h"

#define DEFAULT_ARRAY_SIZE 5

#define APPLICATION_NAME	_T("RapidSVN")

// Ids used for communication events between threads and frame
#define TOKEN_BASE					100
#define TOKEN_INFO					TOKEN_BASE + 1
#define TOKEN_SVN_INTERNAL_ERROR	TOKEN_BASE + 2
#define TOKEN_VSVN_INTERNAL_ERROR	TOKEN_BASE + 3
#define TOKEN_ACTION_END			TOKEN_BASE + 4

wxString& UnixPath( wxString &path );

void GetStatusText( wxString& str, svn_wc_status_kind st );

void TrimString( wxString& str );

void* svn_cl__make_log_msg_baton( const char *message, 
								const char *base_dir,
								apr_pool_t *pool );

svn_error_t* svn_cl__get_log_message( const char **log_msg,
										apr_array_header_t *commit_items,
										void *baton,
										apr_pool_t *pool );

void revision_from_number( svn_client_revision_t *rev, unsigned long nr );


/**
* Recursive function to send the error strings to a Tracer
*/
void handle_svn_error( svn_error_t *err, Tracer *tracer );

void MakeStatusLine( const SvnFileStatus& st, const wxString& path, wxString& line );

svn_error_t * svn_cl__may_need_force( svn_error_t *err );

#endif