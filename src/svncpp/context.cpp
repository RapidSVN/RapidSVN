/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// Subversion api
#include "svn_auth.h"
#include "svn_config.h"
#include "svn_utf.h"

// svncpp
#include "context.hpp"

namespace svn
{
  struct Context::Data
  {
  public:
    bool loginSet;
    bool logSet;
    Pool pool;
    svn_client_ctx_t ctx;
    std::string username;
    std::string password;
    std::string logMessage;

    Data ()
      : loginSet (false), logSet (false)
    {
      // intialize authentication providers
      apr_array_header_t *providers = 
        apr_array_make (pool, 1, 
                        sizeof (svn_auth_provider_object_t *));
      svn_auth_provider_object_t *client_provider = 
        (svn_auth_provider_object_t *)apr_pcalloc (
          pool, sizeof (*client_provider));

      svn_client_get_simple_provider (
        &(client_provider->vtable),
        &(client_provider->provider_baton), 
        pool);

      *(svn_auth_provider_object_t **)apr_array_push (providers) = 
        client_provider;

      svn_auth_baton_t *ab;
      svn_auth_open (&ab, providers, pool);
      svn_auth_set_parameter (ab, SVN_AUTH_PARAM_DEFAULT_USERNAME, "");
      svn_auth_set_parameter (ab, SVN_AUTH_PARAM_DEFAULT_PASSWORD, "");

      // initialize ctx structure
      memset (&ctx, 0, sizeof (ctx));
      svn_config_get_config (&ctx.config, pool);
      ctx.auth_baton = ab;
      ctx.log_msg_func = log_msg;
      ctx.log_msg_baton = this;
      ctx.notify_func = notify;
      ctx.notify_baton = this;
    }

    /** @see Context::setLogin */
    void setLogin (const char * usr, const char *pwd)
    {
      username = usr;
      password = pwd;
      loginSet = true;

      svn_auth_baton_t * ab = ctx.auth_baton;
      svn_auth_set_parameter (ab, SVN_AUTH_PARAM_DEFAULT_USERNAME, 
                              username.c_str ());
      svn_auth_set_parameter (ab, SVN_AUTH_PARAM_DEFAULT_PASSWORD, 
                              password.c_str ());

    }

    /** @see Context::setLogMessage */
    void setLogMessage (const char * msg)
    {
      logMessage = msg;
      logSet = true;
    }
      
    static svn_error_t *
    log_msg (const char **log_msg, 
             const char **tmp_file,
             apr_array_header_t *commit_items,
             void *baton,
             apr_pool_t *pool)
    {
      Context * context = (Context *)baton;
      //TODO maybe change encoding
      //take a look at *subversion*client/cmdline/util.c
      *log_msg = context->getLogMessage ();
      *tmp_file = NULL;
    
      return SVN_NO_ERROR;
    }

    static void 
    notify (void * baton,
            const char *path,
            svn_wc_notify_action_t action,
            svn_node_kind_t kind,
            const char *mime_type,
            svn_wc_notify_state_t content_state,
            svn_wc_notify_state_t prop_state,
            svn_revnum_t revision)
    {
      //TODO forward this
    }

    /**
     * used by the simple prompt provider to retrieve
     * username and password
     *
     * @param result returned username/password, depending on
     *               on hide
     * @param prompt string (unused here)
     * @param hide 0=username/1=password
     * @param baton Context
     * @param pool pool to use
     * @return error
     */
    static svn_error_t *
    prompt (const char **result, 
            const char *prompt,
            svn_boolean_t hide, 
            void *baton, 
            apr_pool_t *pool)
    {
      apr_status_t status=0;

      if (baton == NULL)
      {
        return svn_error_create (status, NULL, 
                                 "Context::prompt: baton==NULL");
      }

      Context * context = (Context *)baton;

      if (hide==TRUE)
      {
        SVN_ERR (svn_utf_cstring_to_utf8( 
                   (const char **)result, 
                   context->getPassword (), NULL, pool));
      }
      else
      {
        SVN_ERR (svn_utf_cstring_to_utf8( 
                   (const char **)result,
                   context->getUsername (), NULL, pool));
      }

      return SVN_NO_ERROR;
    }
    
  };

  Context::Context ()
  {
    m = new Data ();
  }

  Context::Context (const Context & src)
  {
    m = new Data ();
    setLogin (src.getUsername (), src.getPassword ());
  }

  Context::~Context ()
  {
    delete m;
  }
  
  void
  Context::setLogin (const char * username, const char * password)
  {
    m->setLogin (username, password);
  }

  Context::operator svn_client_ctx_t * () 
  {
    return &(m->ctx);
  }

  svn_client_ctx_t * 
  Context::ctx ()
  {
    return &(m->ctx);
  }
  
  void 
  Context::setLogMessage (const char * msg)
  {
    m->setLogMessage (msg);
  }

  const char *
  Context::getUsername () const
  {
    return m->username.c_str ();
  }

  const char *
  Context::getPassword () const
  {
    return m->password.c_str ();
  }

  const char *
  Context::getLogMessage () const
  {
    return m->logMessage.c_str ();
  }

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
