/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#ifndef _SVNCPP_LOG_ENTRY_H_
#define _SVNCPP_LOG_ENTRY_H_

// stl
#include <string>
#include <list>

// apr
#include "apr_time.h"

// subversion api
#include "svn_types.h"

namespace svn
{

  struct LogChangePathEntry
  {
    LogChangePathEntry (const char *path_,
                        char action_,
                        const char *copyFromPath_,
                        const svn_revnum_t copyFromRevision_);

    std::string path;
    char action;
    std::string copyFromPath;
    svn_revnum_t copyFromRevision;
  };


  struct LogEntry
  {
  public:
    LogEntry ();

    LogEntry (const svn_revnum_t revision,
              const char * author,
              const char * date,
              const char * message);

    svn_revnum_t revision;
    std::string author;
    std::string message;
    std::list<LogChangePathEntry> changedPaths;
    apr_time_t date;
  };
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */

