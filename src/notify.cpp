#include "include.h"
#include "svn_pools.h"
#include "tracer.h"
#include "notify.h"

/* Baton for notify	and	friends. */
struct NotifyBaton
{
	svn_boolean_t received_some_change;
	svn_boolean_t is_checkout;
	svn_boolean_t suppress_final_line;
	svn_boolean_t sent_first_txdelta;
	Tracer *tracer;
	apr_pool_t *pool;
};


/* This	implements `svn_wc_notify_func_t'. */
static void notify( void *baton,
					const char *path,
					svn_wc_notify_action_t action,
					svn_node_kind_t	kind,
					const char *mime_type,
					svn_wc_notify_state_t content_state,
					svn_wc_notify_state_t prop_state,
					svn_revnum_t revision )
{
	NotifyBaton	*nb	= (NotifyBaton*)baton;
	char statchar_buf[3] = "_ ";
	
	switch( action )
	{
	case svn_wc_notify_delete:
	case svn_wc_notify_update_delete:
		{
			nb->received_some_change = TRUE;
			wxString str = wxString::Format( "D  %s", path );
			nb->tracer->Trace( str );
		}
		break;
		
	case svn_wc_notify_update_add:
		{
			nb->received_some_change = TRUE;
			wxString str = wxString::Format( "A  %s", path );
			nb->tracer->Trace( str );
		}
		break;
		
	case svn_wc_notify_restore:
		{
			wxString str = wxString::Format( "Restored %s", path );
			nb->tracer->Trace( str );
		}
		break;
		
	case svn_wc_notify_revert:
		{
			wxString str = wxString::Format( "Reverted %s", path );
			nb->tracer->Trace( str );
		}
		break;
		
	case svn_wc_notify_resolve:
		{
			wxString str = wxString::Format( "Resolved conflicted state of %s", path );
			nb->tracer->Trace( str );
		}
		break;
		
	case svn_wc_notify_add:
	/* We *should* only	get	the	MIME_TYPE if PATH is a file.  If we
	do get it, and the mime-type is	not	in the "text/" grouping,
		note that this is a	binary addition.  */
		{
			wxString		str;
			wxString		mimeType( mime_type );
			if( mime_type
				&& ( mimeType.Length() > 5 )
				&& ( mimeType.StartsWith( "text/" ) != 0 ) )
			{
				str = wxString::Format( "A  (bin)  %s", path );
			}
			else
			{
				str = wxString::Format( "A         %s", path );
			}
			nb->tracer->Trace( str );
		}
		break;
		
	case svn_wc_notify_update_update:
		if( kind ==	svn_node_file )
		{
		/* Do this only	for	files, not for dirs, since we don't	want
			to treat directory closings	as "change"	events.	*/
			nb->received_some_change = TRUE;
			
			if( content_state == svn_wc_notify_state_conflicted )
				statchar_buf[0]	= 'C';
			else if( content_state == svn_wc_notify_state_merged )
				statchar_buf[0]	= 'G';
			else if( content_state == svn_wc_notify_state_modified )
				statchar_buf[0]	= 'U';
		}
		
		if( prop_state == svn_wc_notify_state_conflicted )
			statchar_buf[1]	= 'C';
		else if( prop_state	== svn_wc_notify_state_merged )
			statchar_buf[1]	= 'G';
		else if( prop_state	== svn_wc_notify_state_modified )
			statchar_buf[1]	= 'U';
		
		if( !((kind == svn_node_dir)
			&& ((prop_state == svn_wc_notify_state_unknown)
			|| (prop_state == svn_wc_notify_state_unchanged))))
		{
			wxString str = wxString::Format( "%s %s", statchar_buf, path );
			nb->tracer->Trace( str );
		}

		break;
		
	case svn_wc_notify_update_external:
	/* Currently this is used for checkouts	and	switches too.  If we
		want different output, we'll have to add new actions. */
		{
			wxString str = wxString::Format( "Fetching	external item into %s", path );
			nb->tracer->Trace( str );
		}
		break;
		
	case svn_wc_notify_update_completed:
		{
			if (! nb->suppress_final_line)
			{
				if (SVN_IS_VALID_REVNUM	(revision))
				{
					if (nb->is_checkout)
					{
						wxString str = wxString::Format( "Checked out revision %" SVN_REVNUM_T_FMT ".",
														revision );
						nb->tracer->Trace( str );
					}
					else
					{
						if (nb->received_some_change)
						{
							wxString str = wxString::Format( "Updated to revision %" SVN_REVNUM_T_FMT ".",
															revision );
							nb->tracer->Trace( str );
						}
						else
						{
							wxString str = wxString::Format( "At revision %" SVN_REVNUM_T_FMT ".",
															revision );
							nb->tracer->Trace( str );
						}
					}
				}
				else  /* no	revision */
				{
					if (nb->is_checkout)
						nb->tracer->Trace( "Checkout complete." );
					else
						nb->tracer->Trace( "Update complete." );
				}
			}
		}
		
		break;
		
	case svn_wc_notify_commit_modified:
		{
			wxString str = wxString::Format( "Sending        %s", path );
			nb->tracer->Trace( str );
		}
		break;
		
	case svn_wc_notify_commit_added:
		{
			wxString		str;
			wxString		mimeType( mime_type );

			if( mime_type
				&& ( mimeType.Length() > 5 )
				&& ( mimeType.StartsWith( "text/" ) != 0 ) )
			{
				str = wxString::Format("Adding  (bin)  %s", path);
			}
			else
			{
				str = wxString::Format("Adding         %s", path);
			}
			nb->tracer->Trace( str );
		}
		break;
		
	case svn_wc_notify_commit_deleted:
		{
			wxString str = wxString::Format( "Deleting       %s", path );
			nb->tracer->Trace( str );
		}
		break;
		
	case svn_wc_notify_commit_replaced:
		{
			wxString str = wxString::Format( "Replacing      %s", path );
			nb->tracer->Trace( str );
		}
		break;
		
	case svn_wc_notify_commit_postfix_txdelta:
		if( !nb->sent_first_txdelta )
		{
			nb->tracer->Trace( "Transmitting file data " );
			nb->sent_first_txdelta = TRUE;
		}
		
		//nb->tracer->Trace( "." );
		break;
		
	default:
		break;
	}
}


void svn_cl__get_notifier( svn_wc_notify_func_t *notify_func_p,
							void **notify_baton_p,
							svn_boolean_t	is_checkout,
							svn_boolean_t	suppress_final_line,
							Tracer *tr,
							apr_pool_t *pool )
{
	NotifyBaton	*nb	= (NotifyBaton*)apr_palloc (pool,	sizeof (*nb));
	
	nb->received_some_change = FALSE;
	nb->sent_first_txdelta = FALSE;
	nb->is_checkout	= is_checkout;
	nb->suppress_final_line	= suppress_final_line;
	nb->pool = pool;
	nb->tracer = tr;
	
	*notify_func_p = notify;
	*notify_baton_p	= nb;
}
