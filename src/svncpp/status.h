
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

  /**
   * Returns the description of the status.
   */
  const char * statusDescription (svn_wc_status_kind kind);


  
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
   * Returns the last last author to commit to a file.
   * @exception EntryNotVersioned
   */
  const char * lastCommitAuthor ();


  /**
   * Returns the file status of the "textual" component. 
   */
  const char * textDescription ();

  /**
   * Returns the file status property enum of the "textual" component. 
   */
  svn_wc_status_kind textType ();

  /**
   * Returns the file status of the "property" component. 
   */
  const char * propDescription ();

  /**
   * Returns the file status property enum of the "property" component. 
   */
  svn_wc_status_kind propType ();

  /**
   * Returns whether the file is under version control.
   */
  bool isVersioned ();

  /**
   * Returns true if the file is locked.
   * @exception EntryNotVersioned
   */
  bool isLocked ();

  /**
   * Returns true if the file is copied.
   * @exception EntryNotVersioned
   */
  bool isCopied ();
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
