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
    svn_client_auth_baton_t * baton = & m_auth_baton;
    baton->username = m_username.c_str ();
    baton->password = m_password.c_str ();
    baton->prompt_callback = NULL;
    baton->prompt_baton = NULL;
    baton->store_auth_info = 1;

    svn_client_ctx_t * ctx = & m_ctx;
    ctx->old_auth_baton = baton;
    ctx->auth_baton = NULL;
    ctx->prompt_func = NULL;
    ctx->prompt_baton = NULL;
    ctx->notify_func = NULL;
    ctx->notify_baton = NULL;
    ctx->log_msg_func = NULL;
    ctx->log_msg_baton = NULL;

    return ctx;
  }

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
