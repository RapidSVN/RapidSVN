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

#ifndef _SVNCPP_AUTH_H_
#define _SVNCPP_AUTH_H_

// stl
#include <string>

// subversion api
#include "svn_client.h"

namespace svn
{
  // forward declarations
  class Pool;

  /**
   * Subversion authentication API.
   */
  class Auth
  {
  private:
    std::string m_username;
    std::string m_password;

  public:
    Auth ();
    ~Auth ();

    /**
     * Sets the username.
     */
    void username (const char * username);
  
    /**
     * Sets the password.
     */
    void password (const char * password);

    /**
     * returns a client context for authentication
     *
     * @param pool pool to use for allocation
     * @return client context
     */
    svn_client_ctx_t * context (const Pool & pool);
  };
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
