#ifndef _AUTHENTICATION_BATON_H_INCLUDED_
#define _AUTHENTICATION_BATON_H_INCLUDED_

class AuthBaton
{
public:
  svn_client_auth_baton_t * auth_obj;
  apr_pool_t *subpool;

public:

   AuthBaton (apr_pool_t * pool,
              const wxString & user = _T (""),
              const wxString & pass = _T (""),
              svn_client_prompt_t prompt_callback = NULL,
              void *prompt_baton = NULL, svn_boolean_t owr = FALSE);

  ~AuthBaton ();
};

#endif
