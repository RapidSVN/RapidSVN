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

// stl
#include <string>

// svncpp
#include "svncpp/log_entry.hpp"
#include "svncpp/pool.hpp"

// subversion api
#include "svn_time.h"


namespace svn
{
  LogChangePathEntry::LogChangePathEntry (
    const char *path_,
    char action_,
    const char *copyFromPath_,
    const svn_revnum_t copyFromRevision_)
   : path (path_), action(action_), 
     copyFromPath (copyFromPath_ != NULL ? copyFromPath_ : ""),
     copyFromRevision (copyFromRevision_)
  {
  }


  LogEntry::LogEntry ()
  {
  }


  LogEntry::LogEntry (
    const svn_revnum_t revision_,
    const char * author_,
    const char * date_,
    const char * message_)
  {
    date = 0;

    if (date_ != 0)
    {
      Pool pool;

      if (svn_time_from_cstring (&date, date_, pool) != 0)
        date = 0;
    }

    revision = revision_;
    author = author_ == 0 ? "" : author_;
    message = message_ == 0 ? "" : message_;
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
