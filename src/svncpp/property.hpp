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

#ifndef _SVNCPP_PROPERTY_H_
#define _SVNCPP_PROPERTY_H_

#ifdef WIN32
// Eliminate worthless win32 warnings
#pragma warning(disable: 4786)
#endif

// stl 
#include <vector>
#include <string>

// svncpp
#include "auth.hpp"
#include "path.hpp"

namespace svn
{
  // forward declarations
  class Path;

  /**
   * Class for manipulating Subversion properties.
   *
   * TODO replace pseudo-iterator interface
   *      with stl containers
   */
  class Property : public svn::Auth
  {
  public:
    Property ();
    ~Property ();

    /**
     * Returns the number of properties found.
     */
    int count ();

    /**
     * Loads the properties result set, clearing old result sets. 
     */
    void loadPath (const Path & path);

    /**
     * Moves to the next row in the result set.
     * @returns true if the cursor is in the result set.
     */
    bool next ();

    /**
     * Moves to the previous row in the result set.
     * @returns true if the cursor is in the result set.
     */
    bool previous ();

    /**
     * Moves to the last row in the log result set.
     * @returns true if the cursor is in the result set.
     */
    bool last ();

    /**
     * Moves to the first row in the log result set.
     * @returns true if the cursor is in the result set.
     */
    bool first ();

    /**
     * Moves to the cursor before first row in the log result set.
     */
    void beforeFirst ();

    /**
     * Returns true if the file called in loadPath is versioned.
     */
    bool isVersioned ();

    /**
     * Sets an existing property with a new value or adds a new 
     * property.  If a result is added it does not reload the 
     * result set.  Run loadPath again.
     * @exception ClientException
     */
    void set (const char * name, const char * value, bool recurse);

    /**
     * Returns the name of the current property of the result set.
     */
    const char * name ();

    /**
     * Returns the value of the current property of the result set.
     */
    const char * value ();

    /**
     * Deletes a property.  
     * @exception ClientException
     */
    void remove (const char * name, bool recurse);

  private:
    int m_size;
    int m_cursor;
    bool m_versioned;
    std::vector<std::string> m_propNames;
    std::vector<std::string> m_propValues;
    Path m_lastPath;

    void reset ();
    const char * propertyValue (const char * key);

    /**
     * Returns whether or not the property is a special Subversion property.
     */
    svn_boolean_t isSvnProperty (const char * name);

  };

}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
