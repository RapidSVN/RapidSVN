#ifndef _MERGE_ACTION_H_INCLUDED_
#define _MERGE_ACTION_H_INCLUDED_

#include "action_thread.h"

class MergeAction
:	public ActionThread
{
private:
	void GetNotifier(svn_wc_notify_func_t *notify_func_p, void **notify_baton_p, svn_boolean_t is_checkout, svn_boolean_t suppress_final_line, apr_pool_t *pool);
	
	wxString		path1;
	wxString		path2;

	svn_client_revision_t	rev_start;
	svn_client_revision_t	rev_end;
	
	svn_client_revision_t	rev_second;

	wxString		user;
	wxString		pass;

	svn_boolean_t	recursive;	
	svn_boolean_t	force;

public:	
					MergeAction( wxFrame* frame, apr_pool_t *__pool, Tracer* tr );
					~MergeAction();

	void			Perform();
	void*			Entry();
};

#endif