#ifndef _SVN_FILE_INFO_H_INCLUDED_
#define _SVN_FILE_INFO_H_INCLUDED_

svn_error_t *svn_get_file_info (const wxString & path,
                                apr_pool_t * pool, wxString & info);

#endif
