
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
};

}

#endif
