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

#ifndef _SVNCPP_PATH_HPP_
#define _SVNCPP_PATH_HPP_

// stl
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

    /**
     * check whether a path is set. Right now
     * this checks only if the string is non-
     * empty.
     *
     * @return true if there is a path set
     */
    bool 
    isset() const;


    /**
     * adds a new URL component to the path
     *
     * @param component new component to add
     */
    void
    addComponent (const char * component);


    /**
     * adds a new URL component to the path
     *
     * @param component new component to add
     */
    void
    addComponent (const std::string & component);


    /**
     * split path in its components
     *
     * @param dirpath directory/path component
     * @param basename filename
     */
    void
    split (std::string & dirpath, std::string & basename) const;


    /**
     * split path in its components including
     * file extension
     *
     * @param dir directory component
     * @param filename filename
     * @param ext extension (including leading dot ".")
     */
    void
    split (std::string & dir, std::string & filename, std::string & ext) const;


    /**
     * returns the temporary directory
     */
    static Path
    getTempDir ();


    /** return the length of the path-string */
    size_t 
    length () const;


    /** returns the path with native separators */
    std::string
    native () const;
  };
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
