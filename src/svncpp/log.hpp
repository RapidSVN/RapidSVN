/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#ifndef _SVNCPP_LOG_H_
#define _SVNCPP_LOG_H_

#ifdef WIN32
// Eliminate worthless win32 warnings
#pragma warning(disable: 4786)
#endif

// stl
#include <vector>
#include <string>

// svncpp
#include "log_entry.hpp"

namespace svn
{
  // forward declarations
  class Revision;

  /**
   * Class to retrieve log information
   */
  class Log 
  {
  public:
    /**
     * Loads the log messages result set. 
     *TODO??? This usually requires authentication. 
     * You can use the constants Revision::START and
     * Revision::HEAD
     *
     * @see Auth
     *
     * @param path
     * @param revisionStart
     * @param revisionEnd                    
     */
    Log (const char * path, const Revision & revisionStart, 
         const Revision & revisionEnd);

    virtual ~Log();

    /**
     * @return vector with log entries
     */
    const std::vector<LogEntry> &
    entries () const;

  private:
    std::vector<LogEntry> m_entries;

    /**
     * hide default constructor
     */
    Log ();
    
    /**
     * hide copy constructor
     */
    Log (const Log &);
  };

}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
