
#ifndef _SVNCPP_PROPERTY_H_
#define _SVNCPP_PROPERTY_H_

#ifdef WIN32
// Eliminate worthless win32 warnings
#pragma warning(disable: 4786)
#endif

#include "client.h"
#include "svn_utf.h"
#include <vector>
#include <string>

#ifndef _SVNCPP_EXCEPTION_H_
#include "exception.h"
#endif

namespace svn
{

/**
 * Class for manipulating Subversion properties.
 */
class Property : public svn::Client
{
private:
  std::string filePath;
  int size;
  int cursor;
  bool versioned;
  std::vector<std::string> propName;
  std::vector<std::string> propValue;

  void reset ();

  /**
   * Returns whether or not the property is a special Subversion property.
   */
  svn_boolean_t isSvnProperty (const char * name);

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
  void loadPath (const char * path);

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
};

}

#endif
