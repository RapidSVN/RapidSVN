
#ifndef _SVNCPP_AUTH_H_
#define _SVNCPP_AUTH_H_

#include "client.h"
#include "error.h"

namespace Svn
{

/**
 * Subversion authentication API.
 */
class Auth : public Svn::Client
{
private:
  char * userName;
  char * passWord;
  svn_client_auth_baton_t * auth_obj;

public:
  Auth ();
  ~Auth ();

  /**
   * Sets the username.
   */
  void Username (char * username);
  
  /**
   * Sets the password.
   */
  void Password (char * password);
  
  /**
   * Returns an authentication object.
   */
  svn_client_auth_baton_t * Authenticate ();
};

}

#endif
