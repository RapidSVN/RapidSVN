#ifndef _IMPORT_ACTION_H_INCLUDED_
#define _IMPORT_ACTION_H_INCLUDED_

#include "action_thread.h"

class ImportAction
:	public ActionThread
{
private:
	
	wxString		reposURL;
	wxString		path;
	wxString		new_entry;
	wxString		logMsg;	
	wxString		user;
	wxString		pass;
	svn_boolean_t	recursive;

public:	
					ImportAction( wxFrame* frame, apr_pool_t *__pool, Tracer* tr );
					~ImportAction();

	void			Perform();
	void*			Entry();
};

#endif