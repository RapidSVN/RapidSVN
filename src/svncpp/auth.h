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

#include "client.h"
#include <string>

namespace svn
{

/**
 * Subversion authentication API.
 */
class Auth : public svn::Client
{
private:
  std::string userName;
  std::string passWord;
  svn_client_auth_baton_t * auth_obj;
  bool failed;

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
   * Returns an authentication object.
   */
  svn_client_auth_baton_t * authenticate ();

  /**
   * Returns true if the authentication succeeded.  This should be called 
   * immediately following an action in which authentication is required
   * to verify success.
   */
  bool isAuthenticated ();
};

/**
 * The auth info callback routine. This should be called if
 * the user is unable to authenticate.
 */
svn_error_t * prompt (char **info, const char *prompt, svn_boolean_t hide,
                      void *baton, apr_pool_t *pool);

}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
