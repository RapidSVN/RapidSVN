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

#ifndef _SVNCPP_PATH_HPP_
#define _SVNCPP_PATH_HPP_

#include <string>

namespace svn
{
  /**
   * Encapsulation for Subversion Path handling
   */
  class Path
  {
  private:
    std::string m_path;

    /**
     * initialize the class
     *
     * @param path Path string
     */
    void init (const char * path);

  public:
    /**
     * Constructor that takes a string as parameter.
     * The string is converted to subversion internal
     * representation. The string is copied.
     *
     * @param path Path string
     */
    Path (const std::string & path = "");
    
    /**
     * Constructor
     *
     * @see Path::Path (const std::string &)
     * @param path Path string
     */
    Path (const char * path);

    /**
     * Copy constructor
     *
     * @param path Path to be copied
     */
    Path (const Path & path);

    /**
     * Assignment operator
     */
    Path& operator=(const Path&);

    /**
     * @return Path string
     */
    const std::string &
    path () const;

    /**
     * @return Path string as c string
     */
    const char * 
    c_str() const;

  };
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
