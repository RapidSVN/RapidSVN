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

#ifndef _SVNCPP_WC_HPP_
#define _SVNCPP_WC_HPP_ 

namespace svn
{
  /**
   * Class that deals with a working copy
   */
  class Wc
  {
  public:
    /**
     * check if Path is a valid working directory
     *
     * @param dir path to a directory
     * @return true=valid working copy
     */
    static bool 
    checkWc (const char * dir);

    static const char * ADM_DIR_NAME;

  private:
  };
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
