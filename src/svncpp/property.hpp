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
#include "context.hpp"
#include "path.hpp"

namespace svn
{
  struct PropertyEntry
  {
    std::string name;
    std::string value;

    PropertyEntry (const char * name, const char * value);
  };    

  // forward declarations
  class Path;

  /**
   * Class for manipulating Subversion properties.
   */
  class Property 
  {
  public:
    Property (Context * context = 0,
              const Path & path = "");

    virtual ~Property ();

    /**
     * get the list of properties for the path.
     * throws an exception if the path isnt versioned.
     */
    const std::vector<PropertyEntry> &
    entries () const
    {
      return m_entries;
    }

    /**
     * Sets an existing property with a new value or adds a new 
     * property.  If a result is added it does not reload the 
     * result set.  Run loadPath again.
     * @exception ClientException
     */
    void set (const char * name, const char * value);

    /**
     * Deletes a property.  
     * @exception ClientException
     */
    void remove (const char * name);

  private:
    Context * m_context;
    Path m_path;
    std::vector<PropertyEntry> m_entries;

    std::string getValue (const char * name);
    void list ();

  };

}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
