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

#ifndef _SVNCPP_UTILS_HPP_
#define _SVNCPP_UTILS_HPP_

namespace svn
{
  // forward declarations
  class Path;

  /**
   * Encapsulation for Subversion Utility functions
   */
  class Utils
  {
  public:
    /**
     * perform cleanup on working copy
     *
     * @param path path to clean.
     */
    static void 
    cleanup (const Path & path);

  private:
    /**
     * hide default constructor
     */
    Utils ();
  };
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
