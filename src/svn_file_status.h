#ifndef _SVN_FILE_STATUS_H_INCLUDED_
#define _SVN_FILE_STATUS_H_INCLUDED_

#include "auth_baton.h"

class SvnFileStatus
{
private:
	svn_wc_status_t		*status;
	apr_pool_t			*subpool;
		
public:
						SvnFileStatus( apr_pool_t *thepool );
						~SvnFileStatus();

	void				retrieveStatus( const wxString &path, 
										AuthBaton& auth_baton );

	svn_wc_status_kind	getFileStatus() const;
	svn_wc_status_kind	getFilePropStatus() const;
	svn_revnum_t		getRevision() const;
	svn_revnum_t		getLastChange() const;

	operator			bool()
						{
							return status == NULL;
						}

	const svn_wc_status_t *getStatus() const
						{
							return status;
						}

};

#endif