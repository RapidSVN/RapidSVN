
#ifndef _SVNCPP_CLIENT_H_
#define _SVNCPP_CLIENT_H_

#include "apr_general.h"
#include "svn_pools.h"
#include "svn_client.h"

/**
 * SvnCpp namespace.
 */
namespace svn
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
   * Initializes the primary memory pool.
   */
  Client ();

  ~Client ();
};

}

#endif
