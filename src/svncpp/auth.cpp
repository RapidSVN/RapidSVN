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

// subversion api
#include "svn_client.h"
#include "svn_utf.h"

// svncpp
#include "auth.hpp"
#include "pool.hpp"

namespace svn
{

  Auth::Auth ()
  {
  }

  Auth::~Auth ()
  {
  }

  void
  Auth::username (const char *username)
  {
    m_username = username;
  }

  void
  Auth::password (const char *password)
  {
    m_password = password;
  }

  svn_client_ctx_t * 
  Auth::context (const Pool & pool)
  {
    apr_pool_t * p = pool.pool ();
    svn_client_auth_baton_t * baton =
      (svn_client_auth_baton_t *)
      apr_pcalloc (p, sizeof (svn_client_auth_baton_t));
    baton->username = svn_string_create (m_username.c_str (), p)->data;
    baton->password = svn_string_create (m_password.c_str (), p)->data;
    baton->prompt_callback = NULL;
    baton->prompt_baton = NULL;
    baton->store_auth_info = 1;
    baton->got_new_auth_info = 1;

    svn_client_ctx_t * ctx =
      svn_client_ctx_create (p);
    svn_client_ctx_set_auth_baton (ctx, baton);

    return ctx;
  }

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
