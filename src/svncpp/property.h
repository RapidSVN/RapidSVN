
#ifndef _SVNCPP_PROPERTY_H_
#define _SVNCPP_PROPERTY_H_

#include "client.h"
#include "svn_utf.h"

namespace Svn
{

/**
 * Class for manipulating Subversion properties.
 */
class Property : public Svn::Client
{
private:
  apr_array_header_t * props;
  char * sTarget;
  int nPropCount;
  int nCurrentProp;
  const char * sProperty;

  /**
   * Returns whether or not the property is a special Subversion property.
   */
  svn_boolean_t IsSvnProperty (char * sName);

  /**
   * Loads the initial data for the property list.
   */
  bool LoadProperties ();

public:
  Property ();
  ~Property ();

  /**
   * Returns the number of properties found.
   */
  int Count ();

  /**
   * Loads the file, verifies that it is valid, and records the 
   * property count.
   */
  bool LoadPath (char * sPath);

  /**
   * Sets or adds a property with a new value.
   */
  bool Set (char * sName, char * sValue, bool recurse);

  /**
   * Returns the value of a property.
   * was not found.
   */
  const char * GetValue (char * sName);

  /**
   * Deletes a property.  
   */
  bool Delete (char * sName, bool recurse);

  /**
   * Returns the next property name in the list.  Returns 
   * NULL when the iteration is complete.
   */
  const char * NextProperty ();

  /**
   * Resets the iterator in which the NextProperty method 
   * runs.
   */
  void Reset ();
};

}

#endif
