/*
 * ====================================================================
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

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

void
Property::beforeFirst ()
{
  if(count () < 1)
    return;

  cursor = -1;
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
  svn_error_t * error = NULL;
  apr_array_header_t * props;
  m_lastPath = path;
  internalPath (m_lastPath);

  reset ();
  m_Err = svn_client_proplist (&props,
                             m_lastPath.c_str (), 
                             false /* recurse */,
                             m_pool);
  versioned = true;
  if(m_Err != NULL)
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
                                         m_pool);

    apr_hash_index_t *hi;
    int index = 0;

    for (hi = apr_hash_first (m_pool, item->prop_hash); hi; 
         hi = apr_hash_next (hi))
    {
      const void *key;
      const char *key_native;
      void *val;

      apr_hash_this (hi, &key, NULL, &val);
      svn_utf_cstring_from_utf8 (&key_native, (char *)key, m_pool);

      propName.push_back (key_native);
      propValue.push_back (propertyValue (key_native));
      size++;
    } 
  }
}

const char *
Property::propertyValue (const char * key)
{
  apr_hash_t *props;
  apr_hash_index_t *hi;

  svn_client_propget (&props, key, m_lastPath.c_str (),
                      false /* recurse */, m_pool);

  svn_boolean_t is_svn_prop = svn_prop_is_svn_prop (key);

  for (hi = apr_hash_first (m_pool, props); hi; hi = apr_hash_next (hi))
  {
    const void *key;
    void *val;
    const svn_string_t *propval;
  
    apr_hash_this (hi, &key, NULL, &val);
    propval = (const svn_string_t *)val;
  
    /* If this is a special Subversion property, it is stored as
       UTF8, so convert to the native format. */
    if (is_svn_prop)
      svn_utf_string_from_utf8 (&propval, propval, m_pool);
  
    return propval->data;
  }

  return NULL;
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

  propval = svn_string_create ((const char *) value, m_pool);

  svn_utf_cstring_to_utf8 (&pname_utf8, name, NULL, m_pool);

  error = svn_client_propset (pname_utf8, propval, m_lastPath.c_str (),
                              recurse, m_pool);
  if(error != NULL)
    throw ClientException (error);
}

void 
Property::remove (const char * name, bool recurse)
{
  const char *pname_utf8;
  svn_error_t * error = NULL;
  svn_utf_cstring_to_utf8 (&pname_utf8, name, NULL, m_pool);

  error = svn_client_propset (pname_utf8, NULL, m_lastPath.c_str (),
                            recurse, m_pool);
  if(error != NULL)
    throw ClientException (error);
}

svn_boolean_t
Property::isSvnProperty (const char * name)
{
  const char *pname_utf8;

  svn_utf_cstring_to_utf8 (&pname_utf8, name, NULL, m_pool);
  svn_boolean_t is_svn_prop = svn_prop_is_svn_prop (pname_utf8);

  return is_svn_prop;
}

}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
