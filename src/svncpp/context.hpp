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

#ifndef _SVNCPP_CONTEXT_HPP_
#define _SVNCPP_CONTEXT_HPP_ 

// stl
#include <string>

// Subversion api
#include "svn_client.h"

// svncpp
#include "pool.hpp"

// forward declarations

namespace svn
{
  /**
   * This class will hold the client context
   * and replace the old notification and baton
   * stuff
   */
  class Context
  {
  public:
    /**
     * default constructor
     */
    Context ();

    /**
     * copy constructor
     *
     * @param src 
     */
    Context (const Context &src);

    /**
     * destructor
     */
    virtual ~Context ()
    {
    }

    /**
     * constructor with username/password
     */
    Context (const char * username, const char * password);

    /**
     * set username/password for authentication
     */
    void setLogin (const char * username, const char * password);

    /**
     * operator to get svn_client_ctx object
     */
    operator svn_client_ctx_t * () 
    {
      return &m_ctx;
    }

    svn_client_ctx_t * ctx ()
    {
      return &m_ctx;
    }

    /**
     * set log message
     *
     * @param msg
     */
    void setLogMessage (const char * msg)
    {
      m_logMessage = msg;
    }

  private:
    Pool m_pool;
    svn_client_ctx_t m_ctx;
    std::string m_username;
    std::string m_password;
    std::string m_logMessage;

    /**
     * disable assignment operator
     */
    Context & operator = (const Context &);

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
    prompt (const char ** result, const char * prompt,
            svn_boolean_t hide, void *baton,
            apr_pool_t *pool);
    
    static svn_error_t *
    log_msg (const char **log_msg, 
             const char **tmp_file,
             apr_array_header_t *commit_items,
             void *baton,
             apr_pool_t *pool);

    static void 
    notify (void * baton,
            const char *path,
            svn_wc_notify_action_t action,
            svn_node_kind_t kind,
            const char *mime_type,
            svn_wc_notify_state_t content_state,
            svn_wc_notify_state_t prop_state,
            svn_revnum_t revision);

  };
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
