
#include "property.h"
#include "stdio.h"

namespace Svn
{

Property::Property ()
{
  props = NULL;
  nPropCount = 0;
  nCurrentProp = 0;
  sProperty = NULL;
}

Property::~Property ()
{
}

int
Property::Count ()
{
  return nPropCount;
}

bool
Property::LoadPath (char * sPath)
{
  sTarget = sPath;

  if(!LoadProperties ())
    return false;

  return true;
}

bool
Property::LoadProperties ()
{
  Err = svn_client_proplist (&props,
                             sTarget, 
                             false /* recurse */,
                             pool);
  if(Err != NULL)
    return false;

  nPropCount = 0;

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
      nPropCount++;
    } 
  }

  return true;
}

const char *
Property::NextProperty ()
{
  if(props == NULL)
    return NULL;

  if(nCurrentProp >= nPropCount)
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

      if(index != nCurrentProp)
      {
        index++;
        continue;
      }

      apr_hash_this (hi, &key, NULL, NULL);
      svn_utf_cstring_from_utf8 (&key_native, (char *)key, pool);

      sProperty = key_native;
      nCurrentProp++;
    } 
  }

  return sProperty;
}

void
Property::Reset ()
{
  nCurrentProp = 0;
}

const char *
Property::GetValue (char * sName)
{
  apr_hash_t *prop;
  apr_hash_index_t *hi;

  Err = svn_client_propget (&prop,
                            sName,
                            sTarget,
                            false /* recurse */,
                            pool);

  if(Err != NULL)
    return NULL;

  for (hi = apr_hash_first (pool, prop); hi; hi = apr_hash_next (hi))
  {
    const void *key;
    void *val;
    const svn_string_t *propval;

    apr_hash_this (hi, &key, NULL, &val);
    propval = (svn_string_t *)val;

    /* If this is a special Subversion property, it is stored as
       UTF8, so convert to the native format. */
    if (IsSvnProperty (sName))
      Err = svn_utf_string_from_utf8 (&propval, propval, pool);

    return propval->data;
  }

  return NULL;
}

bool
Property::Set (char * sName, char * sValue, bool recurse)
{
  const char *pname_utf8;
  const svn_string_t *propval = NULL;
  propval = svn_string_create ((const char *) sValue, pool);

  svn_utf_cstring_to_utf8 (&pname_utf8, sName, NULL, pool);

  Err = svn_client_propset (pname_utf8, propval, sTarget,
                            recurse, pool);
  if(Err != NULL)
    return false;

  LoadProperties ();

  return true;
}

bool 
Property::Delete (char * sName, bool recurse)
{
  const char *pname_utf8;
  svn_utf_cstring_to_utf8 (&pname_utf8, sName, NULL, pool);

  Err = svn_client_propset (pname_utf8, NULL, sTarget,
                            recurse, pool);
  if(Err != NULL)
    return false;

  LoadProperties ();

  return true;
}

svn_boolean_t
Property::IsSvnProperty (char * sName)
{
  const char *pname_utf8;

  svn_utf_cstring_to_utf8 (&pname_utf8, sName, NULL, pool);
  svn_boolean_t is_svn_prop = svn_prop_is_svn_prop (pname_utf8);

  return is_svn_prop;
}

}
