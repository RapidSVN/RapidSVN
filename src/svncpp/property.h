
#ifndef _SVNCPP_PROPERTY_H_
#define _SVNCPP_PROPERTY_H_

#include "client.h"
#include "svn_utf.h"

namespace svn
{

/**
 * Class for manipulating Subversion properties.
 */
class Property : public svn::Client
{
private:
  apr_array_header_t * props;
  char * filePath;
  int propCount;
  int currentProp;
  const char * propName;

  /**
   * Returns whether or not the property is a special Subversion property.
   */
  svn_boolean_t isSvnProperty (char * name);

  /**
   * Loads the initial data for the property list.
   */
  bool loadProperties ();

public:
  Property ();
  ~Property ();

  /**
   * Returns the number of properties found.
   */
  int count ();

  /**
   * Loads the file, verifies that it is valid, and records the 
   * property count.
   */
  bool loadPath (char * path);

  /**
   * Sets or adds a property with a new value.
   */
  bool set (char * path, char * value, bool recurse);

  /**
   * Returns the value of a property.
   * was not found.
   */
  const char * getValue (char * name);

  /**
   * Deletes a property.  
   */
  bool remove (char * name, bool recurse);

  /**
   * Returns the next property name in the list.  Returns 
   * NULL when the iteration is complete.
   */
  const char * nextProperty ();

  /**
   * Resets the iterator in which the NextProperty method 
   * runs.
   */
  void reset ();
};

}

#endif
