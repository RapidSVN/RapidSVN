/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#include "auth.h"
#include "svn_utf.h"

struct prompt_user_baton
{
  bool * failed;
};

namespace svn
{

Auth::Auth ()
{
  auth_obj = (svn_client_auth_baton_t *) apr_pcalloc 
             (m_pool, sizeof (*auth_obj));

  auth_obj->prompt_callback = prompt;
  auth_obj->prompt_baton = NULL;
  auth_obj->store_auth_info = true;
  auth_obj->got_new_auth_info = true;
}

Auth::~Auth ()
{
  failed = false;
}

void
Auth::username (const char *username)
{
  userName = username;
}

void
Auth::password (const char *password)
{
  passWord = password;
}

bool
Auth::isAuthenticated ()
{
  return !failed;
}

svn_client_auth_baton_t *
Auth::authenticate ()
{
  auth_obj->username = userName.c_str ();
  auth_obj->password = passWord.c_str ();
  prompt_user_baton pub;
  pub.failed = &failed;
  auth_obj->prompt_baton = &pub;

  return auth_obj;
}

svn_error_t *
prompt (char **info, const char *prompt, svn_boolean_t hide,
        void *baton, apr_pool_t *pool)
{
  prompt_user_baton *pub = (prompt_user_baton*) baton;
  svn_stringbuf_t *strbuf = svn_stringbuf_create ("", pool);

  *pub->failed = true;

  svn_utf_cstring_to_utf8 ((const char **)info, strbuf->data,
                            NULL, pool);

  return SVN_NO_ERROR;
}

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
