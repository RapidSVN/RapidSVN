
#ifndef _SVNCPP_CLIENT_H_
#define _SVNCPP_CLIENT_H_

#include "apr_general.h"
#include "svn_pools.h"
#include "svn_client.h"
#include "svn_opt.h"
#include <string>

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
  svn_opt_revision_t rev;
  std::string targetPath;

  /**
   * Global error object struct.
   */
  svn_error_t * Err;

  /**
   * Returns the last path submitted.
   */
  const char * getLastPath ();

  /**
   * Convert the path to the Subversion format.
   */
  void internalPath (std::string & path);

  /**
   * Create a revision template.
   */
  svn_opt_revision_t * getRevision (long revNumber);

  /**
   * Creates a target out of a string.
   */
  apr_array_header_t * target (const char * path);

public:
  /**
   * Initializes the primary memory pool.
   */
  Client ();

  ~Client ();
};

}

#endif
