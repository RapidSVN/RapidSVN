#include "include.h"
#include "utils.h"
#include "tracer.h"
#include "notify.h"
#include "rapidsvn_app.h"
#include "revert_action.h"

RevertAction::RevertAction( wxFrame* frame, apr_pool_t *__pool, 
					 Tracer* tr, apr_array_header_t *trgts )
:	ActionThread( frame, __pool )
,	targets( trgts )
{
	SetTracer( tr, FALSE );		// do not own the tracer
}

void RevertAction::Perform()
{
	// No dialog for Revert. Just start the thread.
	// Note: recursion is not enabled by default.
	
	// #### TODO: check errors and throw an exception
	// create the thread
	Create();

	// here we start the action thread
	Run();
}

void* RevertAction::Entry()
{
	int				i;	
	svn_error_t		*err = NULL;
	apr_pool_t		*subpool;
	svn_boolean_t recursive = FALSE;	// no recursion by default
	svn_wc_notify_func_t notify_func = NULL;
	void *notify_baton = NULL;
	
	subpool = svn_pool_create( pool );

	svn_cl__get_notifier( &notify_func, &notify_baton,
						  FALSE, FALSE, GetTracer(), pool );

	for( i = 0; i < targets->nelts; i++ )
	{

		const char *target = ((const char **) (targets->elts))[i];

		err =  svn_client_revert( target,
								recursive,
								notify_func,
								notify_baton,
								subpool );

		if( err )
			break;

		svn_pool_clear( subpool );
	}

	if( err )
	{
		PostDataEvent( TOKEN_SVN_INTERNAL_ERROR, err, ACTION_EVENT );
	}

	PostDataEvent( TOKEN_ACTION_END, NULL, ACTION_EVENT );
	
	svn_pool_destroy( subpool );

	return NULL;
}