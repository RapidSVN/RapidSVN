#ifndef _NOTIFY_H_INCLUDED_
#define _NOTIFY_H_INCLUDED_

/* Set *NOTIFY_FUNC_P and *NOTIFY_BATON_P to a notifier/baton for all
 * operations, allocated in POOL.
 * 
 * If this is a checkout, set IS_CHECKOUT to true, so that the
 * notifier will print the appropriate summary line at the end of the
 * output.
 * 
 * If don't want a summary line at the end of notifications, set
 * SUPPRESS_FINAL_LINE.
 */
void svn_cl__get_notifier(  svn_wc_notify_func_t *notify_func_p,
							void **notify_baton_p,
							svn_boolean_t is_checkout,
							svn_boolean_t suppress_final_line,
							Tracer *tr,
							apr_pool_t *pool );


#endif