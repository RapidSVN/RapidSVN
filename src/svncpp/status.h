
#ifndef _SVNCPP_STATUS_H_
#define _SVNCPP_STATUS_H_

#include "auth.h"
#include "svn_sorts.h"

namespace svn
{

/**
 * Subversion status API.
 */
class Status : public svn::Auth
{
private:
  apr_hash_t *statushash;
  svn_revnum_t youngest;
  apr_array_header_t * statusarray;
  svn_wc_status_t * status;
  bool versioned;
  char * filePath;
  
  /**
   * Reset to all of the default properties.
   */
  void reset ();
  
public:
  Status ();
  ~Status ();

  /**
   * Initiaties the status on a path. Returns false if the file is 
   * not under version control.
   */
  bool loadPath (char * path);
  
  /**
   * Returns the revision.  Returns -1 on failure.
   */
  long revision ();

  /**
   * Returns the last time the file was changed revision number.
   * Returns -1 on failure.
   */
  long lastChanged ();

  /**
   * Returns the file status in text. Returns NULL on failure.
   */
  char * statusText ();

  /**
   * Returns the file status. Returns NULL on failure.
   */
  char * statusProp ();

  /**
   * Returns whether the file is under version control.
   */
  bool isVersioned ();
};

}

#endif
