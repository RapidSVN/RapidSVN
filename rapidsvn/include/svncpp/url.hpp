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

#ifndef _SVNCPP_URL_H_
#define _SVNCPP_URL_H_

// stl
#include <string>
#include <vector>


namespace svn
{
  class Url
  {
  public:
    /** Constructor */
    Url ();

    /** Destructor */
    virtual ~Url ();

    /**
     * Checks if @a url is valid
     *
     * Example of a valid URL:
     *   http://svn.collab.net/repos/svn
     * Example of an invalid URL:
     *   /home/foo/bar
     */
    static bool
    isValid (const char * url);

    /**
     * returns a vector with url schemas that are
     * supported by svn 
     *
     * @return vector with entries like "file:", "http:"
     */
    static std::vector<std::string>
    supportedSchemas ();
  };
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */

