#include "include.h"
#include "utils.h"
#include "tracer.h"
#include "notify.h"
#include "rapidsvn_app.h"
#include "add_action.h"

AddAction::AddAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr, apr_array_header_t * trgts):ActionThread (frame, __pool),
  targets
  (trgts)
{
  SetTracer (tr, FALSE);        // do not own the tracer
}

void
AddAction::Perform ()
{
  // No dialog for Add. Just start the thread.

  // #### TODO: check errors and throw an exception
  // create the thread
  Create ();

  // here we start the action thread
  Run ();
}

void *
AddAction::Entry ()
{
  int i;
  svn_error_t *err = NULL;
  apr_pool_t *subpool;

  svn_wc_notify_func_t notify_func = NULL;
  void *notify_baton = NULL;

  subpool = svn_pool_create (pool);

  svn_cl__get_notifier (&notify_func, &notify_baton,
                        TRUE, FALSE, GetTracer (), pool);

  for (i = 0; i < targets->nelts; i++)
  {

    const char *target = ((const char **) (targets->elts))[i];

    err = svn_client_add (target, 1,    // recursive (should get it from conf)
                          notify_func, notify_baton, subpool);

    if (err)
    {
      if (err->apr_err == SVN_ERR_ENTRY_EXISTS)
      {
        GetTracer ()->Trace (err->message);
        svn_error_clear_all (err);
      }
      else
      {
        svn_pool_clear (subpool);
        break;
      }
    }
  }

  if (err)
  {
    PostDataEvent (TOKEN_SVN_INTERNAL_ERROR, err, ACTION_EVENT);
  }

  PostDataEvent (TOKEN_ACTION_END, NULL, ACTION_EVENT);

  svn_pool_destroy (subpool);

  return NULL;
}
