
#ifndef _SVNCPP_ERROR_H_
#define _SVNCPP_ERROR_H_

#include "client.h"

namespace svn
{

/**
 * Error handling class.
 */
class Error
{
private:
  svn_error_t * err;

public:
  Error ();
  ~Error ();

  /**
   * Sets the error from an Subversion error struct.
   */
  void setError (svn_error_t * error);

  /**
   * Records whether an error exists.
   */
  bool exists ();

  /**
   * Returns the error message. Returns NULL if there is no error.
   */
  const char * message ();

  /**
   * Returns the error source.  Returns NULL if there is no error.
   */
  const char * source ();

  /**
   * Returns the APR error id.  Returns -1 if there is no error.
   */
  int aprError ();

};

}

#endif
