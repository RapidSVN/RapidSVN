
#ifndef _SVNCPP_LOG_H_
#define _SVNCPP_LOG_H_

#ifdef WIN32
// Eliminate worthless win32 warnings
#pragma warning(disable: 4786)
#endif

#include "auth.h"
#include <vector>
#include <string>

namespace svn
{

/**
 * Iterator class for log entries on a file.
 */
class Log : public svn::Auth
{
private:
  std::vector<long> _revision;
  std::vector<std::string> _author;
  std::vector<std::string> _date;
  std::vector<std::string> _message;
  bool versioned;

  /**
   * Resets all of the global variables.
   */
  void reset ();

  /**
   * Records where the iterator is currently set to.
   */
  int cursor;

  /**
   * Number of elements in the result set.
   */
  int size;

public:
  Log();
  ~Log();

  /**
   * Loads the log messages result set, clearing old result sets. 
   * This usually requires authentication, see Auth. 
   * @param revisionStart if set to -2 then the revision will start 
   *                      at the revision head.
   * @param revisionEnd if revisionStart is set to -2 and this is set 
   *                    to 1 then it will retrieve all log messages.
   *                        
   */
  void loadPath (const char * path, long revisionStart, 
                 long revisionEnd);

  /**
   * Moves to the next row in the log result set.
   * @returns true if the cursor is in the result set.
   */
  bool next ();

  /**
   * Moves to the previous row in the log result set.
   * @returns true if the cursor is in the result set.
   */
  bool previous ();

  /**
   * Moves to the last row in the log result set.
   * @returns true if the cursor is in the result set.
   */
  bool last ();

  /**
   * Moves to the first row in the log result set.
   * @returns true if the cursor is in the result set.
   */
  bool first ();

  /**
   * Moves to the cursor before first row in the log result set.
   */
  void beforeFirst ();

  /**
   * Returns the log message of the current result set.
   */
  const char * message ();

  /**
   * Returns the log date of the current result set.
   */
  const char * date ();

  /**
   * Formats the output of a Subversion date.
   * @param dateText Subversion date text. Takes the result of the 
   *                 date() method.
   * @param format template for formatting the returned date.
   */
  const char * formatDate (const char * dateText, const char * format);

  /**
   * Returns the log author of the current result set.
   */
  const char * author ();

  /**
   * Returns the revision of the current result set.
   */
  unsigned long revision ();

  /**
   * Returns the number of results recieved.
   */
  int count ();

  /**
   * Returns true if the file is versioned.
   */
  bool isVersioned ();
};

static svn_error_t * messageReceiver (void *baton,
                                      apr_hash_t * changed_paths,
                                      svn_revnum_t rev,
                                      const char *author,
                                      const char *date, 
                                      const char *msg, 
                                      apr_pool_t * pool);

}

#endif
