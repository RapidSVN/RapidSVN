#include "include.h"
#include "wx/resource.h"
#include "rapidsvn_app.h"
#include "notify.h"
#include "auth_baton.h"
#include "update_dlg.h"
#include "update_action.h"

UpdateAction::UpdateAction( wxFrame *frame, apr_pool_t *__pool, 
							Tracer* tr, apr_array_header_t *trgts )
:	ActionThread( frame, __pool )
,	targets( trgts )
{
	SetTracer( tr, FALSE );		// do not own the tracer
	revnum = 0;
	rev_specified = false;
}

void UpdateAction::Perform()
{
	////////////////////////////////////////////////////////////
	// Here we are in the main thread.

#if defined(__WXMSW__)
	// Load the .wxr 'file' from a .rc resource, under Windows.
	Update_Dialog = wxLoadUserResource("Update_Dialog", "WXRDATA");
	// All resources in the file (only one in this case) get parsed
	// by this call.
	wxResourceParseString(Update_Dialog);
#else
	// Simply parse the data pointed to by the variable Import_Dialog.
	wxResourceParseData(Update_Dialog);	
#endif
	
	UpdateDlg		*upDlg = new UpdateDlg;

	if( upDlg->LoadFromResource( mainFrame, "Update_Dialog" ) )
	{
		upDlg->InitializeData();

		if( upDlg->ShowModal() == ID_BUTTON_OK )
		{
			user = upDlg->user->GetValue();
			pass = upDlg->pass->GetValue();

			// get revision number from dialog
			// #### TODO: check errors
			wxString	tmpstr = upDlg->revision->GetValue();
			TrimString( tmpstr );
			if( tmpstr.IsEmpty() )
				rev_specified = false;
			else
			{
				rev_specified = true;
				tmpstr.ToULong( &revnum, 10);
			}


			// #### TODO: check errors and throw an exception
			// create the thread
			Create();

			// here we start the action thread
			Run();

	////////////////////////////////////////////////////////////
		}
	}

	// destroy the dialog
	upDlg->Close(TRUE);
}

void* UpdateAction::Entry()
{
	AuthBaton		auth_baton( pool, user, pass );
	svn_client_revision_t	rev;
	int				i;

	svn_wc_notify_func_t notify_func = NULL;
	void *notify_baton = NULL;
	svn_error_t		*err = NULL;

	memset( &rev, 0, sizeof(rev) );
	
	svn_cl__get_notifier( &notify_func, &notify_baton,
						  TRUE, FALSE, GetTracer(), pool );

	if( rev_specified )
		revision_from_number( &rev, revnum );

	for( i = 0; i < targets->nelts; i++ )
	{
		const char *target = ((const char **) (targets->elts))[i];
		const char *parent_dir, *entry;
		
		err =  svn_wc_get_actual_target( target,
										&parent_dir,
										&entry,
										pool );
		if( err )
			break;

		err = svn_client_update( auth_baton.auth_obj,
								target,
								NULL,
								&(rev),
								TRUE,
								notify_func, notify_baton,
								pool );
		
		if( err )
			break;
	}

	if( err )
	{
		PostDataEvent( TOKEN_SVN_INTERNAL_ERROR, err, ACTION_EVENT );
	}

	PostDataEvent( TOKEN_ACTION_END, NULL, ACTION_EVENT );
	
	return NULL;
}