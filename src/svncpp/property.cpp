
#include "property.h"
#include "stdio.h"

namespace svn
{

Property::Property ()
{
  props = NULL;
  propCount = 0;
  currentProp = 0;
  propName = NULL;
}

Property::~Property ()
{
}

int
Property::count ()
{
  return propCount;
}

void
Property::loadPath (const char * path)
{
  filePath = path;
  svn_error_t * error = NULL;

  error = loadProperties ();
  if(error != NULL)
    throw ClientException (error);
}

svn_error_t *
Property::loadProperties ()
{
  Err = svn_client_proplist (&props,
                             filePath.c_str (), 
                             false /* recurse */,
                             pool);
  if(Err != NULL)
    return Err;

  propCount = 0;

  for (int j = 0; j < props->nelts; ++j)
  {
    svn_client_proplist_item_t *item = 
          ((svn_client_proplist_item_t **)props->elts)[j];

    const char *node_name_native;
    svn_utf_cstring_from_utf8_stringbuf (&node_name_native,
                                         item->node_name,
                                         pool);

    apr_hash_index_t *hi;

    for (hi = apr_hash_first (pool, item->prop_hash); hi; 
         hi = apr_hash_next (hi))
    {
      propCount++;
    } 
  }

  return Err;
}

const char *
Property::nextProperty ()
{
  if(props == NULL)
    return NULL;

  if(currentProp >= propCount)
    return NULL;

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

      if(index != currentProp)
      {
        index++;
        continue;
      }

      apr_hash_this (hi, &key, NULL, NULL);
      svn_utf_cstring_from_utf8 (&key_native, (char *)key, pool);

      propName = key_native;
      currentProp++;
    } 
  }

  return propName;
}

void
Property::reset ()
{
  currentProp = 0;
}

const char *
Property::getValue (const char * name)
{
  apr_hash_t *prop;
  apr_hash_index_t *hi;

  Err = svn_client_propget (&prop,
                            name,
                            filePath.c_str (),
                            false /* recurse */,
                            pool);

  if(Err != NULL)
    throw ClientException (Err);

  for (hi = apr_hash_first (pool, prop); hi; hi = apr_hash_next (hi))
  {
    const void *key;
    void *val;
    const svn_string_t *propval;

    apr_hash_this (hi, &key, NULL, &val);
    propval = (svn_string_t *)val;

    /* If this is a special Subversion property, it is stored as
       UTF8, so convert to the native format. */
    if (isSvnProperty (name))
      Err = svn_utf_string_from_utf8 (&propval, propval, pool);

    if(Err != NULL)
      throw ClientException (Err);

    return propval->data;
  }

  return NULL;
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

  error = loadProperties ();
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

  error = loadProperties ();
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
