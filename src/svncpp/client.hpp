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
  typedef std::vector<Status> StatusEntries;

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
     * @param get_all Return all entries, not just the interesting ones.
     * @param update Query the repository for updates.
     * @param no_ignore Disregard default and svn:ignore property ignores.
     * @return vector with Status entries.
     */
    StatusEntries 
    status (const char * path,
            const bool descend = false,
            const bool get_all = true,
            const bool update = false,
            const bool no_ignore = false);

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
     * Sets a single file for deletion.
     * @exception ClientException
     */
    void remove (const Path & path, bool force);

    /**
     * Sets files for deletion.
     *
     * @param targets targets to delete
     * @param force force if files are locally modified
     * @exception ClientException
     */
    void remove (const Targets & targets, bool force);

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
     * Updates the file or directory.
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
     * Retrieves the contents for a specific @a revision of
     * a @a path
     *
     * @param path path of file or directory
     * @param revision revision to retrieve
     * @return contents of the file
     */
    std::string
    cat (const Path & path, const Revision & revision);

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
     * Create multiple directories.
     *
     * @see mkdir (const Path&, const char*)
     * @param path new directory
     * @param message
     */
    void mkdir (const Targets & targets, const char * message);

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
     * @param srcPath source path
     * @param destPath a destination path that must not already exist.
     * @param revision revision to use for the export
     * @param force force export
     */
    void doExport (const Path & srcPath, const Path & destPath, 
                   const Revision & revision, bool force=false);

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
     * @param path path to import
     * @param url
     * @param message log message.
     * @param recurse
     * @exception ClientException
     */
    void import (const Path & path, const char * url,
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
     * Loads the log messages result set. The first
     * entry  is the youngest revision.
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

    /**
     * Produce diff output which describes the delta between
     * @a path/@a revision1 and @a path/@a revision2. @a path
     * can be either a working-copy path or a URL.
     *
     * A ClientException will be thrown if either @a revision1 or
     * @a revision2 has an `unspecified' or unrecognized `kind'.
     *
     * @param tmpPath prefix for a temporary directory needed by diff.
     * Filenames will have ".tmp" and similar added to this prefix in
     * order to ensure uniqueness.
     * @param path path of the file.
     * @param revision1 one of the revisions to check.
     * @param revision2 the other revision.
     * @param recurse whether the operation should be done recursively.
     * @param ignoreAncestry whether the files will be checked for
     * relatedness.
     * @param noDiffDeleted if true, no diff output will be generated
     * on deleted files.
     * @return delta between the files
     * @exception ClientException
     */
    std::string
    diff (const Path & tmpPath, const Path & path,
          const Revision & revision1, const Revision & revision2,
          const bool recurse, const bool ignoreAncestry,
          const bool noDiffDeleted);

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
