
#ifndef _SVNCPP_STATUS_H_
#define _SVNCPP_STATUS_H_

#include "auth.h"
#include "svn_sorts.h"
#include <string>

#ifndef _SVNCPP_EXCEPTION_H_
#include "exception.h"
#endif

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
  std::string filePath;
  std::string _statusText;
  
  /**
   * Reset to all of the default properties.
   */
  void reset ();
  
public:
  Status ();
  ~Status ();

  /**
   * Initiaties the status on a path. 
   */
  void loadPath (const char * path);
  
  /**
   * Returns the revision.  
   * @exception EntryNotVersioned
   */
  unsigned long revision ();

  /**
   * Returns the last time the file was changed revision number.
   * @exception EntryNotVersioned
   */
  unsigned long lastChanged ();

  /**
   * Returns the file status in text. 
   * @exception EntryNotVersioned
   */
  const char * statusText ();

  /**
   * Returns the file status property enum. 
   * @exception EntryNotVersioned
   */
  svn_wc_status_kind statusProp ();

  /**
   * Returns whether the file is under version control.
   */
  bool isVersioned ();
};

/**
 * Represents an error triggered when the path set is not versioned.
 */
class EntryNotVersioned : public Exception
{
public:
  /**
   * Constructor. 
   */
  EntryNotVersioned (std::string message = "") :
      Exception(message) {};

  /* Destructor */
  virtual ~EntryNotVersioned () throw () {};
};

}

#endif
