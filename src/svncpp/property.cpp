
#include "property.h"
#include "stdio.h"

namespace svn
{

Property::Property ()
{
  reset ();
}

Property::~Property ()
{
}

void
Property::reset ()
{
  size = 0;
  cursor = -1;
  propName.clear ();
  propValue.clear ();
  versioned = false;
}


int
Property::count ()
{
  return size;
}

bool
Property::last ()
{
  if(count () < 1)
    return false;

  cursor = count () - 1;
  return true;
}

bool
Property::first ()
{
  if(count () < 1)
    return false;

  cursor = 0;
  return true;
}

bool
Property::next ()
{
  if((cursor + 1) >= count ())
    return false;

  cursor++;
  return true;
}

bool
Property::previous ()
{
  if((cursor - 1) < 0)
    return false;

  cursor--;
  return true;
}

void
Property::loadPath (const char * path)
{
  filePath = path;
  svn_error_t * error = NULL;
  apr_array_header_t * props;

  reset ();
  Err = svn_client_proplist (&props,
                             filePath.c_str (), 
                             false /* recurse */,
                             pool);
  versioned = true;
  if(Err != NULL)
  {
    versioned = false;
    return;
  }

  for (int j = 0; j < props->nelts; ++j)
  {
    svn_client_proplist_item_t *item = 
          ((svn_client_proplist_item_t **)props->elts)[j];

    const char *node_name_native;
    svn_utf_cstring_from_utf8_stringbuf (&node_name_native,
                                         item->node_name,
                                         pool);

    apr_hash_index_t *hi;
    int index = 0;

    for (hi = apr_hash_first (pool, item->prop_hash); hi; 
         hi = apr_hash_next (hi))
    {
      const void *key;
      const char *key_native;
      void *val;
      const svn_string_t *propval;

      apr_hash_this (hi, &key, NULL, &val);
      propval = (svn_string_t *)val;
      svn_utf_cstring_from_utf8 (&key_native, (char *)key, pool);

      propName.push_back (key_native);
      propValue.push_back (propval->data);
      size++;
    } 
  }
}

bool
Property::isVersioned ()
{
  return versioned;
}

const char *
Property::value ()
{
  if(cursor == -1)
    return NULL;

  return propValue[cursor].c_str ();
}

const char *
Property::name ()
{
  if(cursor == -1)
    return NULL;

  return propName[cursor].c_str ();
}

void
Property::set (const char * name, const char * value, bool recurse)
{
  const char *pname_utf8;
  const svn_string_t *propval = NULL;
  svn_error_t * error = NULL;

  propval = svn_string_create ((const char *) value, pool);

  svn_utf_cstring_to_utf8 (&pname_utf8, name, NULL, pool);

  error = svn_client_propset (pname_utf8, propval, filePath.c_str (),
                              recurse, pool);
  if(error != NULL)
    throw ClientException (error);
}

void 
Property::remove (const char * name, bool recurse)
{
  const char *pname_utf8;
  svn_error_t * error = NULL;
  svn_utf_cstring_to_utf8 (&pname_utf8, name, NULL, pool);

  error = svn_client_propset (pname_utf8, NULL, filePath.c_str (),
                            recurse, pool);
  if(error != NULL)
    throw ClientException (error);
}

svn_boolean_t
Property::isSvnProperty (const char * name)
{
  const char *pname_utf8;

  svn_utf_cstring_to_utf8 (&pname_utf8, name, NULL, pool);
  svn_boolean_t is_svn_prop = svn_prop_is_svn_prop (pname_utf8);

  return is_svn_prop;
}

}
