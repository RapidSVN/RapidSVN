#ifndef _UPDATE_ACTION_H_INCLUDED_
#define _UPDATE_ACTION_H_INCLUDED_

#include "action_thread.h"

class UpdateAction
:	public ActionThread
{
private:
	wxString		user;
	wxString		pass;
	unsigned long	revnum;
	
					/**
					* This is true if the revision edit box
					* from the dialog was not empty. Otherwise,
					* it is false, too indicate that the rev passed to
					* svn_client_checkout must remain unspecified, so the
					* last revision in the repository should be extracted.
					*/
	bool			rev_specified;

	apr_array_header_t	*targets;

public:
					UpdateAction( wxFrame* frame, apr_pool_t *__pool, 
								Tracer* tr, apr_array_header_t *trgts );

	void			Perform();
	void*			Entry();
};

#endif