
#ifndef _SVNCPP_CLIENT_H_
#define _SVNCPP_CLIENT_H_

#include "apr_general.h"
#include "svn_pools.h"
#include "svn_client.h"

/**
 * SvnCpp namespace.
 */
namespace Svn
{

/**
 * Subversion client API.  This class is never directly used.
 */
class Client
{
protected:
  apr_pool_t * pool;

  /**
   * Global error object struct.
   */
  svn_error_t * Err;

public:
  /**
   * Initialize the primary memory pool.
   */
  Client ();

  ~Client ();

  /**
   * Returns the error object.
   */
  svn_error_t * GetError ();
};

}

#endif
