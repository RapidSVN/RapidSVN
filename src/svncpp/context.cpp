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

// Apache Portable Runtime
#include "apr_xlate.h"

// Subversion api
#include "svn_auth.h"
#include "svn_config.h"
#include "svn_subst.h"
#include "svn_utf.h"

// svncpp
#include "context.hpp"
#include "context_listener.hpp"

namespace svn
{
  struct Context::Data
  {
  public:
    ContextListener * listener;
    bool logIsSet;
    int promptCounter;
    Pool pool;
    svn_client_ctx_t ctx;
    std::string username;
    std::string password;
    std::string logMessage;

    /**
     * translate native c-string to utf8 
     */
    static svn_error_t * 
    translateString (const char * str, const char ** newStr,
                     apr_pool_t * pool)
    {
      // due to problems with apr_xlate we dont perform
      // any conversion at this place. YOU will have to make
      // sure any strings passed are UTF 8 strings
      // svn_string_t *string = svn_string_create ("", pool);
      // 
      // string->data = str;
      // string->len = strlen (str);
      // 
      // const char * encoding = APR_LOCALE_CHARSET;
      // 
      // SVN_ERR (svn_subst_translate_string (&string, string,
      //                                      encoding, pool));
      // 
      // *newStr = string->data;
      *newStr = str;
      return SVN_NO_ERROR;
    }

    /**
     * the @a baton is interpreted as Data *
     * Several checks are performed on the baton:
     * - baton == 0?
     * - baton->Data
     * - listener set?
     *
     * @param baton
     * @param data returned data if everything is OK
     * @retval SVN_NO_ERROR if everything is fine
     * @retval SVN_ERR_CANCELLED on invalid values
     */
    static svn_error_t *
    getData (void * baton, Data ** data)
    {
      if (baton == NULL)
        return svn_error_create (SVN_ERR_CANCELLED, NULL, 
                                 "invalid baton");

      Data * data_ = static_cast <Data *>(baton);

      if (data_->listener == 0)
        return svn_error_create (SVN_ERR_CANCELLED, NULL,
                                 "invalid listener");

      *data = data_;
      return SVN_NO_ERROR;
    }

    Data ()
      : listener (0), logIsSet (false), 
        promptCounter (0)
    {
      // intialize authentication providers
      // * simple 
      // * username
      // * simple prompt
      // * ssl server trust file 
      // * ssl server trust prompt
      // * ssl client cert pw file
      // * ssl client cert pw prompt
      // * ssl cliebt cert file
      // ===================
      // 8 providers
      apr_array_header_t *providers = 
        apr_array_make (pool, 8, 
                        sizeof (svn_auth_provider_object_t *));
      svn_auth_provider_object_t *provider;

      svn_client_get_simple_provider (
        &provider,
        pool);
      *(svn_auth_provider_object_t **)apr_array_push (providers) =
        provider;

      svn_client_get_username_provider (
        &provider,
        pool);
      *(svn_auth_provider_object_t **)apr_array_push (providers) =
        provider;

      svn_client_get_simple_prompt_provider (
        &provider,
        onSimplePrompt,
        this,
        100000000, // not very nice. should be infinite...
        pool);
      *(svn_auth_provider_object_t **)apr_array_push (providers) = 
        provider;

      // add ssl providers
      svn_client_get_ssl_server_trust_file_provider (&provider, pool);
      *(svn_auth_provider_object_t **)apr_array_push (providers) = 
        provider;

      svn_client_get_ssl_server_trust_prompt_provider (
        &provider, onSslServerTrustPrompt, this, pool);
      *(svn_auth_provider_object_t **)apr_array_push (providers) = 
        provider;

      svn_client_get_ssl_client_cert_pw_file_provider (&provider, pool);
      *(svn_auth_provider_object_t **)apr_array_push (providers) = 
        provider;

      svn_client_get_ssl_client_cert_pw_prompt_provider (
        &provider, onSslClientCertPwPrompt, this, pool);
      *(svn_auth_provider_object_t **)apr_array_push (providers) = 
        provider;

      svn_client_get_ssl_client_cert_file_provider (&provider, pool);
      *(svn_auth_provider_object_t **)apr_array_push (providers) = 
        provider;

      svn_auth_baton_t *ab;
      svn_auth_open (&ab, providers, pool);

      // initialize ctx structure
      memset (&ctx, 0, sizeof (ctx));
      svn_config_get_config (&ctx.config, 0, pool);
      ctx.auth_baton = ab;
      ctx.log_msg_func = onLogMsg;
      ctx.log_msg_baton = this;
      ctx.notify_func = onNotify;
      ctx.notify_baton = this;
    }

    void setAuthCache(bool value)
    {
      void *param = 0;
      if (!value)
        param = (void *)"1";
 
      svn_auth_set_parameter (ctx.auth_baton,
                              SVN_AUTH_PARAM_NO_AUTH_CACHE,
                              param);
    }

    /** @see Context::setLogin */
    void setLogin (const char * usr, const char *pwd)
    {
      username = usr;
      password = pwd;

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
      logIsSet = true;
    }

    /**
     * this function gets called by the subversion api function
     * when a log message is needed. This is the case on a commit
     * for example
     */
    static svn_error_t *
    onLogMsg (const char **log_msg, 
              const char **tmp_file,
              apr_array_header_t *commit_items,
              void *baton,
              apr_pool_t *pool)
    {
      Data * data;
      SVN_ERR (getData (baton, &data));

      std::string msg ("");
      if (data->logIsSet)
        msg = data->getLogMessage ();
      else
      {
        if (!data->retrieveLogMessage (msg))
          return svn_error_create (SVN_ERR_CANCELLED, NULL, "");
      }

      SVN_ERR (svn_utf_cstring_to_utf8 (
                 log_msg, msg.c_str (), pool));

      *tmp_file = NULL;
    
      return SVN_NO_ERROR;
    }

    /**
     * this is the callback function for the subversion
     * api functions to signal the progress of an action
     */
    static void 
    onNotify (void * baton,
              const char *path,
              svn_wc_notify_action_t action,
              svn_node_kind_t kind,
              const char *mime_type,
              svn_wc_notify_state_t content_state,
              svn_wc_notify_state_t prop_state,
              svn_revnum_t revision)
    {
      if (baton == 0)
        return;

      Data * data = static_cast <Data *> (baton);

      data->notify (path, action, kind, mime_type, content_state,
                    prop_state, revision);
    }

    /**
     * @see svn_auth_simple_prompt_func_t
     */
    static svn_error_t *
    onSimplePrompt (svn_auth_cred_simple_t **cred,
                    void *baton,
                    const char *realm,
                    const char *username, 
                    apr_pool_t *pool)
    {
      Data * data;
      SVN_ERR (getData (baton, &data));

      if (!data->retrieveLogin (username, realm))
        return svn_error_create (SVN_ERR_CANCELLED, NULL, "");

      svn_auth_cred_simple_t* lcred = (svn_auth_cred_simple_t*)
        apr_palloc (pool, sizeof (svn_auth_cred_simple_t));
      SVN_ERR (svn_utf_cstring_to_utf8 ( 
                 &lcred->password,
                 data->getPassword (), pool));
      SVN_ERR (svn_utf_cstring_to_utf8 ( 
                 &lcred->username,
                 data->getUsername (), pool));

      *cred = lcred;

      return SVN_NO_ERROR;
    }

    /**
     * @see svn_auth_ssl_server_trust_prompt_func_t
     */
    static svn_error_t *
    onSslServerTrustPrompt (svn_auth_cred_ssl_server_trust_t **cred, 
                            void *baton,
                            const char *realm,
                            int failures,
                            const svn_auth_ssl_server_cert_info_t *info,
                            apr_pool_t *pool)
    {
      Data * data;
      SVN_ERR (getData (baton, &data));
      
      ContextListener::SslServerTrustData trustData (failures);
      trustData.hostname = info->hostname;
      trustData.fingerprint = info->fingerprint;
      trustData.validFrom = info->valid_from;
      trustData.validUntil = info->valid_until;
      trustData.issuerDName = info->issuer_dname;

      if (!data->listener->contextSslServerTrustPrompt (trustData))
        return svn_error_create (SVN_ERR_CANCELLED, NULL, "");

      svn_auth_cred_ssl_server_trust_t *cred_ = 
        (svn_auth_cred_ssl_server_trust_t*)
        apr_palloc (pool, sizeof (svn_auth_cred_ssl_server_trust_t));
      
      cred_->trust_permanently = trustData.trustPermanently;
      cred_->accepted_failures = trustData.acceptedFailures;

      *cred = cred_;
      
      return SVN_NO_ERROR;
    }

    /**
     * @see svn_auth_ssl_client_cert_prompt_func_t
     */
    static svn_error_t *
    onSslClientCertPrompt (svn_auth_cred_ssl_client_cert_t **cred, 
                           void *baton, 
                           apr_pool_t *pool)
    {
      Data * data;
      SVN_ERR (getData (baton, &data));

      std::string certFile ("");
      if (!data->listener->contextSslClientCertPrompt (certFile))
        return svn_error_create (SVN_ERR_CANCELLED, NULL, "");

      svn_auth_cred_ssl_client_cert_t *cred_ = 
        (svn_auth_cred_ssl_client_cert_t*)
        apr_palloc (pool, sizeof (svn_auth_cred_ssl_client_cert_t));
        
      SVN_ERR (svn_utf_cstring_to_utf8 (
                 &cred_->cert_file,
                 certFile.c_str (),
                 pool));

      *cred = cred_;

      return SVN_NO_ERROR;
    }

    /**
     * @see svn_auth_ssl_client_cert_pw_prompt_func_t  
     */
    static svn_error_t *
    onSslClientCertPwPrompt (
      svn_auth_cred_ssl_client_cert_pw_t **cred, 
      void *baton, 
      apr_pool_t *pool)
    {
      Data * data;
      SVN_ERR (getData (baton, &data));

      std::string password ("");
      if (!data->listener->contextSslClientCertPwPrompt (password))
        return svn_error_create (SVN_ERR_CANCELLED, NULL, "");

      svn_auth_cred_ssl_client_cert_pw_t *cred_ = 
        (svn_auth_cred_ssl_client_cert_pw_t *)
        apr_palloc (pool, sizeof (svn_auth_cred_ssl_client_cert_pw_t));

      SVN_ERR (svn_utf_cstring_to_utf8 (
                 &cred_->password,
                 password.c_str (),
                 pool));

      *cred = cred_;

      return SVN_NO_ERROR;
    }

    const char * 
    getUsername () const
    {
      return username.c_str ();
    }

    const char *
    getPassword () const
    {
      return password.c_str ();
    }

    const char *
    getLogMessage () const
    {
      return logMessage.c_str ();
    }

    /**
     * if the @a listener is set, use it to retrieve the log
     * message using ContextListener::contextGetLogMessage. 
     * This return values is given back, then.
     *
     * if the @a listener is not set the its checked whether
     * the log message has been set using @a setLogMessage
     * yet. If not, return false otherwise true
     *
     * @param msg log message
     * @retval false cancel
     */
    bool 
    retrieveLogMessage (std::string & msg)
    {
      bool ok;

      if (listener == 0)
        return false;

      ok = listener->contextGetLogMessage (logMessage);
      if (ok)
        msg = logMessage;
      else
        logIsSet = false;

      return ok;
    }

    /**
     * if the @a listener is set and no password has been
     * set yet, use it to retrieve login and password using 
     * ContextListener::contextGetLogin.
     * 
     * if the @a listener is not set, check if setLogin
     * has been called yet. 
     *
     * @return continue?
     * @retval false cancel
     */
    bool
    retrieveLogin (const char * username_,
                   const char * realm)
    {
      bool ok;

      if (listener == 0)
        return false;

      if (username_ == NULL)
        username = "";
      else
        username = username_;

      ok = listener->contextGetLogin (realm, username, password);

      return ok;
    }

    /**
     * if the @a listener is set call the method
     * @a contextNotify
     */
    void 
    notify (const char *path,
            svn_wc_notify_action_t action,
            svn_node_kind_t kind,
            const char *mime_type,
            svn_wc_notify_state_t content_state,
            svn_wc_notify_state_t prop_state,
            svn_revnum_t revision)
    {
      if (listener != 0)
      {
        listener->contextNotify (path, action, kind, mime_type,
                                 content_state, prop_state, revision);
      }
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
  Context::setAuthCache (bool value)
  {
    m->setAuthCache (value);
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
    return m->getUsername ();
  }

  const char *
  Context::getPassword () const
  {
    return m->getPassword ();
  }

  const char *
  Context::getLogMessage () const
  {
    return m->getLogMessage ();
  }

  void 
  Context::setListener (ContextListener * listener)
  {
    m->listener = listener;
  }

  ContextListener * 
  Context::getListener () const
  {
    return m->listener;
  }

  void
  Context::reset ()
  {
    m->promptCounter = 0;
    m->logIsSet = false;
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
