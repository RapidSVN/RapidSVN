#include "include.h"
#include "auth_baton.h"

AuthBaton::AuthBaton (apr_pool_t * pool,
                      const wxString & username /* = NULL */ ,
                      const wxString & password /* = NULL */ ,
                      svn_client_prompt_t prompt_callback /* = NULL */ ,
                      void *prompt_baton /* = NULL */ ,
                      svn_boolean_t owr /* = FALSE  */ )
{
  subpool = svn_pool_create (pool);
  auth_obj =
    (svn_client_auth_baton_t *) apr_pcalloc (subpool, sizeof (*auth_obj));

  auth_obj->prompt_callback = prompt_callback;
  auth_obj->prompt_baton = prompt_baton;

  if (!username.IsEmpty ())
  {
    auth_obj->username = username.c_str ();
    auth_obj->overwrite = TRUE;
  }

  if (!password.IsEmpty ())
  {
    auth_obj->password = password.c_str ();
    auth_obj->overwrite = TRUE;
  }
}

AuthBaton::~AuthBaton ()
{
  svn_pool_destroy (subpool);
}
