
#ifndef _SVNCPP_STATUS_H_
#define _SVNCPP_STATUS_H_

#include "auth.h"
#include "svn_sorts.h"

namespace Svn
{

/**
 * Subversion status API.
 */
class Status : public Svn::Auth
{
private:
  apr_hash_t *statushash;
  svn_revnum_t youngest;
  apr_array_header_t * statusarray;
  svn_wc_status_t * status;
  bool bVersioned;
  char * sPath;
  
  /**
   * Reset to all of the default properties.
   */
  void Reset ();
  
public:
  Status ();
  ~Status ();

  /**
   * Initiaties the status on a path. Returns false if the file is 
   * not under version control.
   */
  bool LoadPath (char * sFilePath);
  
  /**
   * Returns the revision.  Returns -1 on failure.
   */
  long Revision ();

  /**
   * Returns the last time the file was changed revision number.
   * Returns -1 on failure.
   */
  long LastChanged ();

  /**
   * Returns the file status in text. Returns NULL on failure.
   */
  char * StatusText ();

  /**
   * Returns the file status. Returns NULL on failure.
   */
  char * StatusProp ();

  /**
   * Returns whether the file is under version control.
   */
  bool IsVersioned ();
};

}

#endif
