
#ifndef _SVNCPP_MODIFY_H_
#define _SVNCPP_MODIFY_H_

#include "auth.h"
#include "notify.h"

#ifndef _SVNCPP_EXCEPTION_H_
#include "exception.h"
#endif

#define DEFAULT_ARRAY_SIZE 5

/**
 * Required function for Subversion C API.
 */
svn_error_t * svn_cl__get_log_message (const char **log_msg,
                         apr_array_header_t * commit_items,
                         void *baton, apr_pool_t * pool);

namespace svn
{

/**
 * Repository modification class.
 */
class Modify : public svn::Auth
{
private:
  Notify * _notify;
  svn_client_revision_t rev;

  /**
   * Create a revision template.
   */
  svn_client_revision_t * getRevision (long revNumber);

  /**
   * Creates a log message baton.
   */
  void * logMessage (const char * message, char * baseDirectory = NULL);

  /**
   * Creates a target out of a string.
   */
  apr_array_header_t * target (const char * path);

  svn_error_t * getLogMessage (const char **log_msg,
                         apr_array_header_t * commit_items,
                         void *baton, apr_pool_t * pool);

public:
  Modify ();
  ~Modify ();

  /**
   * Executes a revision checkout.
   * @param moduleName name of the module to checkout.
   * @param destPath destination directory for checkout.
   * @param revision the revision number to checkout. If the number is -1
   *                 then it will checkout the latest revision.
   * @param recurse whether you want it to checkout files recursively.
   * @exception ClientException
   */
  void checkout (const char * moduleName, const char *destPath, 
                 long revision, bool recurse);
  
  /**
   * Sets the notification function and baton that the C library 
   * uses to send processing information back to the calling program.
   * This must be called before calling the other methods in this class.
   * @param notify function that the SVN library should call when 
   *               checking out each file.
   */
  void notification (Notify * notify);

  /**
   * Sets a file for deletion.
   * @exception ClientException
   */
  void remove (const char * path, bool force);

  /**
   * Reverts a file to a pristine state.
   * @exception ClientException
   */
  void revert (const char * path, bool recurse);

  /**
   * Adds a file to the repository.
   * @exception ClientException
   */
  void add (const char * path, bool recurse);

  /**
   * Updates the directory.
   * @param path target file.
   * @param revision the revision number to checkout. If the number is -1
   *                 then it will checkout the latest revision.
   * @param recurse recursively update.
   * @exception ClientException
   */
  void update (const char * path, long revision, bool recurse);

  /**
   * Commits changes to the repository.
   * @param path file to commit.
   * @param logMessage log message that accompanies check in.
   * @param recurse whether the operation should be done recursively.
   * @exception ClientException
   */
  void commit (const char * path, const char * logMessage, bool recurse);

  /**
   * Copies a versioned file with the history preserved.
   * @exception ClientException
   */
  void copy (const char * srcPath, const char * destPath);

  /**
   * Moves or renames a file.
   * @exception ClientException
   */
  void move (const char * srcPath, const char * destPath, 
             long revision, bool force);
};

}

#endif
