/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#ifndef _SVNCPP_CLIENT_H_
#define _SVNCPP_CLIENT_H_

// stl
#include <vector>

// svncpp
#include "context.hpp"
#include "path.hpp"
#include "revision.hpp"
#include "log_entry.hpp"

/**
 * SvnCpp namespace.
 */
namespace svn
{
  // forward declarations
  class Context;
  class Notify;
  class Status;
  class Targets;

  typedef std::vector<LogEntry> LogEntries;

  /**
   * Subversion client API.
   */
  class Client
  {
  public:
    /**
     * Initializes the primary memory pool.
     */
    Client (Context * context = 0);

    virtual ~Client ();


    /**
     * @return returns the Client context
     */
    const Context * getContext () const;

    /**
     * sets the client context
     * you have to make sure the old context
     * is de-allocated
     *
     * @param context new context to use
     */
    void setContext (Context * context = NULL);


    /**
     * Returns the last destination path submitted. 
     *
     * @return path in Subversion format.
     */
    const char *
    getLastPath ();

    /**
     * Enumerates all files/dirs at a given path.
     *
     * Throws an exception if an error occurs
     *
     * @param path Path to explore.
     * @param descend Recurse into subdirectories if existant.
     * @return Hash map with Status entries.
     */
    std::vector<Status>
    status (const char * path,  const bool descend = false);

    /**
     * Returns the status of a single file in the path.
     *
     * Throws an exception if an error occurs
     *
     * @param path File to gather status.
     * @return a Status with Statis.isVersioned = FALSE
     */
    Status singleStatus (const char * path);

  /**
     * Executes a revision checkout.
     * @param moduleName name of the module to checkout.
     * @param destPath destination directory for checkout.
     * @param revision the revision number to checkout. If the number is -1
     *                 then it will checkout the latest revision.
     * @param recurse whether you want it to checkout files recursively.
     * @exception ClientException
     */
    void checkout (const char * moduleName, const Path & destPath, 
                   const Revision & revision, bool recurse);
  
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
    void remove (const Path & path, bool force);

    /**
     * Reverts a file to a pristine state.
     * @exception ClientException
     */
    void revert (const Path & path, bool recurse);

    /**
     * Adds a file to the repository.
     * @exception ClientException
     */
    void add (const Path & path, bool recurse);

    /**
     * Updates the directory.
     * @param path target file.
     * @param revision the revision number to checkout. 
     *                 Revision::HEAD will checkout the 
     *                 latest revision.
     * @param recurse recursively update.
     * @exception ClientException
     */
    void update (const Path & path, const Revision & revision, 
                 bool recurse);

    /**
     * Commits changes to the repository. This usually requires 
     * authentication, see Auth.
     * @return Returns a long representing the revision. It returns a 
     *         -1 if the revision number is invalid.
     * @param targets files to commit.
     * @param message log message.
     * @param recurse whether the operation should be done recursively.
     * @exception ClientException
     */
    svn_revnum_t
    commit (const Targets & targets, const char * message, bool recurse);

    /**
     * Copies a versioned file with the history preserved.
     * @exception ClientException
     */
    void copy (const Path & srcPath, 
               const Revision & srcRevision,
               const Path & destPath);

    /**
     * Moves or renames a file.
     * @exception ClientException
     */
    void move (const Path & srcPath, 
               const Revision & srcRevision, 
               const Path & destPath, 
               bool force);

    /**
     * Creates a directory directly in a repository or creates a 
     * directory on disk and schedules it for addition. If <i>path</i>
     * is a URL then authentication is usually required, see Auth.
     * @param message log message.
     * @exception ClientException
     */
    void mkdir (const Path & path, const char * message);

    /**
     * Recursively cleans up a local directory, finishing any
     * incomplete operations, removing lockfiles, etc.
     * @param path a local directory.
     * @exception ClientException
     */
    void cleanup (const Path & path);

    /**
     * Removes the 'conflicted' state on a file.
     * @exception ClientException
     */
    void resolve (const Path & path, bool recurse);

    /**
     * Exports the contents of either a subversion repository into a 
     * 'clean' directory (meaning a directory with no administrative 
     * directories).
     * @exception ClientException
     * @param destPath a destination path that must not already exist.
     */
    void doExport (const Path & srcPath, const Path & destPath, 
                   const Revision & revision);

    /**
     * Update local copy to mirror a new url. This excapsulates the 
     * svn_client_switch() client method.
     * @exception ClientException
     */
    void doSwitch (const Path & path, const char * url, 
                   const Revision & revision, bool recurse);

    /**
     * Import file or directory PATH into repository directory URL at
     * head.  This usually requires authentication, see Auth.
     * @param message log message.
     * @param newEntry new directory in which the contents of <i>path</i> are 
     *        imported.
     * @exception ClientException
     */
    void import (const Path & path, const char * url, const Path & newEntry,
                 const char * message, bool recurse);

    /**
     * Merge changes from two paths into a new local path.
     * @exception ClientException
     */
    void merge (const Path & path1, const Revision & revision1, 
                const Path & path2, const Revision & revision2,
                const Path & localPath, bool force, bool recurse);

    /**
     * Retrieve log information for the given path
     * Loads the log messages result set. 
     *
     * You can use the constants Revision::START and
     * Revision::HEAD
     *
     * @param path
     * @param revisionStart
     * @param revisionEnd        
     * @return a vector with log entries
     */
    const LogEntries *
    log (const char * path, const Revision & revisionStart, 
         const Revision & revisionEnd);

  protected:
    Revision m_revision;
    Path m_lastPath;
    Context * m_context;

    /**
     * Global error object struct.
     */
    svn_error_t *m_Err;

  private:
    Notify * m_notify;

    /**
     * disallow assignment operator
     */
    Client & operator= (const Client &);

    /**
     * disallow copy constructor
     */
    Client (const Client &);

  };

}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
