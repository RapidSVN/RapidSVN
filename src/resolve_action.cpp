#include "include.h"
#include "utils.h"
#include "tracer.h"
#include "notify.h"
#include "rapidsvn_app.h"
#include "resolve_action.h"

ResolveAction::ResolveAction( wxFrame* frame, apr_pool_t *__pool, 
					 Tracer* tr, apr_array_header_t *trgts )
:	ActionThread( frame, __pool )
,	targets( trgts )
{
	SetTracer( tr, FALSE );		// do not own the tracer
}

void ResolveAction::Perform()
{
	// No dialog for Resolve. Just start the thread.
	// Note: recursion is not enabled by default.
	
	// #### TODO: check errors and throw an exception
	// create the thread
	Create();

	// here we start the action thread
	Run();
}

void* ResolveAction::Entry()
{
	int				i;	
	svn_error_t		*err = NULL;
	apr_pool_t		*subpool;
	svn_wc_notify_func_t notify_func = NULL;
	void *notify_baton = NULL;
    svn_boolean_t recursive  = NULL; // brm
	
	subpool = svn_pool_create( pool );

	svn_cl__get_notifier( &notify_func, &notify_baton,
						  FALSE, FALSE, GetTracer(), pool );

	for( i = 0; i < targets->nelts; i++ )
	{

		const char *target = ((const char **) (targets->elts))[i];

		err =  svn_client_resolve( target,
								notify_func,
								notify_baton,
                                recursive,
								subpool );

		if( err )
		{
			GetTracer()->Trace( err->message );
			svn_error_clear_all( err );
		}

		svn_pool_clear( subpool );
	}

	PostDataEvent( TOKEN_ACTION_END, NULL, ACTION_EVENT );	

	svn_pool_destroy( subpool );

	return NULL;
}