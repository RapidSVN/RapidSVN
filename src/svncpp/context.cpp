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

    Data ()
      : listener (0), logIsSet (false), 
        promptCounter (0)
    {
      // intialize authentication providers
      // * simple 
      // * simple prompt 
      // * ssl server file 
      // * ssl client file 
      // * ssl pw file 
      // * ssl server prompt 
      // * ssl client prompt
      // * ssl pw prompt
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
        prompt,
        this,
        100000000, // not very nice. should be infinite...
        pool);
      *(svn_auth_provider_object_t **)apr_array_push (providers) = 
        provider;

      // add ssl providers
      svn_client_get_ssl_server_file_provider (&provider, pool);
      *(svn_auth_provider_object_t **)apr_array_push (providers) = 
        provider;

      svn_client_get_ssl_client_file_provider (&provider, pool);
      *(svn_auth_provider_object_t **)apr_array_push (providers) = 
        provider;

      svn_client_get_ssl_pw_file_provider (&provider, pool);
      *(svn_auth_provider_object_t **)apr_array_push (providers) = 
        provider;

      svn_client_get_ssl_server_prompt_provider (
        &provider, 
        ssl_server_prompt,
        this,
        pool);
      *(svn_auth_provider_object_t **)apr_array_push (providers) = 
        provider;

      svn_client_get_ssl_client_prompt_provider (
        &provider,
        ssl_client_prompt,
        this, 
        pool);
      *(svn_auth_provider_object_t **)apr_array_push (providers) = 
        provider;

      svn_client_get_ssl_pw_prompt_provider (
        &provider,
        ssl_pw_prompt,
        this, 
        pool);
      *(svn_auth_provider_object_t **)apr_array_push (providers) = 
        provider;

      svn_auth_baton_t *ab;
      svn_auth_open (&ab, providers, pool);

      // initialize ctx structure
      memset (&ctx, 0, sizeof (ctx));
      svn_config_get_config (&ctx.config, 0, pool);
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
    log_msg (const char **log_msg, 
             const char **tmp_file,
             apr_array_header_t *commit_items,
             void *baton,
             apr_pool_t *pool)
    {
      if (baton == NULL)
      {
        return svn_error_create (SVN_ERR_CANCELLED, NULL, 
                                 "Context::Data::log_msg: baton==NULL");
      }

      Data * data = static_cast <Data *> (baton);

      // if there is a listener, ask it
      if (!data->logIsSet)
      {
        data->retrieveLogMessage ();
      }

      // translate log message in native encoding to utf8
      SVN_ERR (translateString (data->getLogMessage (),
                                log_msg, pool));

      *tmp_file = NULL;
    
      return SVN_NO_ERROR;
    }

    /**
     * this is the callback function for the subversion
     * api functions to signal the progress of an action
     */
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
      if (baton == 0)
      {
        return;
      }

      Data * data = static_cast <Data *> (baton);

      data->notify (path, action, kind, mime_type, content_state,
                    prop_state, revision);
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
      if (baton == NULL)
      {
        return svn_error_create (SVN_ERR_CANCELLED, NULL, 
                                 "Context::prompt: baton==NULL");
      }

      Data * data = static_cast <Data *> (baton);

      if (data->promptCounter > 6)
      {
        data->promptCounter = 0;
      }
      data->promptCounter++;

      if (data->promptCounter == 3)
      {
        if (!data->retrieveLogin ())
        {
          return svn_error_create (SVN_ERR_CANCELLED, NULL, "");
        }
      }

      if (hide==TRUE)
      {
        SVN_ERR (translateString (data->getPassword (), result, pool));
      }
      else
      {
        SVN_ERR (translateString (data->getUsername (), result, pool));
      }

      return SVN_NO_ERROR;
    }

    /**
     * used by the ssl server prompt provider 
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
    ssl_server_prompt (const char **result, 
                       const char *prompt,
                       svn_boolean_t hide, 
                       void *baton, 
                       apr_pool_t *pool)
    {
      if (baton == NULL)
      {
        return svn_error_create (SVN_ERR_CANCELLED, NULL, 
                                 "Context::prompt: baton==NULL");
      }

      Data * data = static_cast <Data *> (baton);
      return data->askQuestion (prompt, result, hide  != 0, pool);
    }

    /**
     * used by the ssl client prompt provider 
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
    ssl_client_prompt (const char **result, 
                       const char *prompt,
                       svn_boolean_t hide, 
                       void *baton, 
                       apr_pool_t *pool)
    {
      if (baton == NULL)
      {
        return svn_error_create (SVN_ERR_CANCELLED, NULL, 
                                 "Context::prompt: baton==NULL");
      }

      Data * data = static_cast <Data *> (baton);
      return data->askQuestion (prompt, result, hide != 0, pool);
    }

    /**
     * used by the ssl pw prompt provider 
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
    ssl_pw_prompt (const char **result, 
                   const char *prompt,
                   svn_boolean_t hide, 
                   void *baton, 
                   apr_pool_t *pool)
    {
      if (baton == NULL)
      {
        return svn_error_create (SVN_ERR_CANCELLED, NULL, 
                                 "Context::prompt: baton==NULL");
      }

      Data * data = static_cast <Data *> (baton);
      return data->askQuestion (prompt, result, hide != 0, pool);
    }

    /**
     * retrieves an answer for a question that the
     * subversion api asks
     *
     * @param question question string
     * @param answer answer string
     * @param hide true if the answer be hidden (passwords)
     * @param pool
     * @return error code
     */
    svn_error_t *
    askQuestion (const char *question,
                 const char **result,
                 bool hide,
                 apr_pool_t *pool)
    {
      std::string newResult ("");
      bool ok = listener->contextAskQuestion (
        question, newResult, hide);

      if (!ok)
      {
        return svn_error_create (SVN_ERR_CANCELLED, NULL, 
                                 "Context::question: cancelled");
      }

      SVN_ERR (translateString (newResult.c_str (), result, pool));

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
     */
    bool 
    retrieveLogMessage ()
    {
      bool ok;

      if (listener == 0)
        return false;

      ok = listener->contextGetLogMessage (logMessage);
      if (!ok)
      {
        logIsSet = false;
      }

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
    retrieveLogin ()
    {
      bool ok;

      if (listener == 0)
        return false;

      ok = listener->contextGetLogin (username, password);

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
