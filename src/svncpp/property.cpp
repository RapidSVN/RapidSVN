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

// subversion api
#include "svn_client.h"
#include "svn_utf.h"

// svncpp
#include "exception.hpp"
#include "path.hpp"
#include "pool.hpp"
#include "property.hpp"


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
    m_size = 0;
    m_cursor = -1;
    m_propNames.clear ();
    m_propValues.clear ();
    m_versioned = false;
  }


  int
  Property::count ()
  {
    return m_size;
  }

  bool
  Property::last ()
  {
    if(count () < 1)
      return false;

    m_cursor = count () - 1;
    return true;
  }

  bool
  Property::first ()
  {
    if(count () < 1)
      return false;

    m_cursor = 0;
    return true;
  }

  void
  Property::beforeFirst ()
  {
    if(count () < 1)
      return;

    m_cursor = -1;
  }

  bool
  Property::next ()
  {
    if((m_cursor + 1) >= count ())
      return false;

    m_cursor++;
    return true;
  }

  bool
  Property::previous ()
  {
    if((m_cursor - 1) < 0)
      return false;

    m_cursor--;
    return true;
  }

  void
  Property::loadPath (const Path & path)
  {
    m_lastPath = path;
    apr_array_header_t * props;
    Pool subPool;
    apr_pool_t * apr_pool = subPool.pool ();

    reset ();
    svn_error_t * error =
      svn_client_proplist (&props,
                           path.c_str (), 
                           NULL, // revision
                           false /* recurse */,
                           context (apr_pool),
                           apr_pool);
    m_versioned = true;
    if(error != NULL)
    {
      m_versioned = false;
      return;
    }

    for (int j = 0; j < props->nelts; ++j)
    {
      svn_client_proplist_item_t *item = 
        ((svn_client_proplist_item_t **)props->elts)[j];

      const char *node_name_native;
      svn_utf_cstring_from_utf8_stringbuf (&node_name_native,
                                           item->node_name,
                                           apr_pool );

      apr_hash_index_t *hi;

      for (hi = apr_hash_first (apr_pool, item->prop_hash); hi; 
           hi = apr_hash_next (hi))
      {
        const void *key;
        const char *key_native;
        void *val;

        apr_hash_this (hi, &key, NULL, &val);
        svn_utf_cstring_from_utf8 (&key_native, (char *)key, apr_pool);

        m_propNames.push_back (key_native);
        m_propValues.push_back (propertyValue (key_native));
        m_size++;
      } 
    }
  }

  const char *
  Property::propertyValue (const char * key)
  {
    apr_hash_t *props;
    apr_hash_index_t *hi;
    Pool subPool;
    apr_pool_t *apr_pool = subPool.pool ();

    svn_client_propget (&props, 
                        key, 
                        m_lastPath.c_str (),
                        NULL, // revision
                        false, // recurse
                        context (apr_pool),
                        apr_pool);

    svn_boolean_t is_svn_prop = svn_prop_is_svn_prop (key);

    for (hi = apr_hash_first (apr_pool, props); hi; hi = apr_hash_next (hi))
    {
      const void *key;
      void *val;
      const svn_string_t *propval;
  
      apr_hash_this (hi, &key, NULL, &val);
      propval = (const svn_string_t *)val;
  
      /* If this is a special Subversion property, it is stored as
         UTF8, so convert to the native format. */
      if (is_svn_prop)
        svn_utf_string_from_utf8 (&propval, propval, apr_pool);
  
      return propval->data;
    }

    return NULL;
  }

  bool
  Property::isVersioned ()
  {
    return m_versioned;
  }

  const char *
  Property::value ()
  {
    if(m_cursor == -1)
      return NULL;

    return m_propValues[m_cursor].c_str ();
  }

  const char *
  Property::name ()
  {
    if(m_cursor == -1)
      return NULL;

    return m_propNames[m_cursor].c_str ();
  }

  void
  Property::set (const char * name, const char * value, bool recurse)
  {
    const char *pname_utf8;
    const svn_string_t *propval = NULL;
    svn_error_t * error = NULL;
    Pool subPool;
    apr_pool_t *apr_pool = subPool.pool ();

    propval = svn_string_create ((const char *) value, apr_pool);

    svn_utf_cstring_to_utf8 (&pname_utf8, name, NULL, apr_pool);

    error = svn_client_propset (pname_utf8, propval, m_lastPath.c_str (),
                                recurse, apr_pool);
    if(error != NULL)
      throw ClientException (error);
  }

  void 
  Property::remove (const char * name, bool recurse)
  {
    const char *pname_utf8;
    svn_error_t * error = NULL;
    Pool subPool;
    apr_pool_t *apr_pool = subPool.pool ();

    svn_utf_cstring_to_utf8 (&pname_utf8, name, NULL, apr_pool);

    error = svn_client_propset (pname_utf8, NULL, m_lastPath.c_str (),
                                recurse, apr_pool);
    if(error != NULL)
      throw ClientException (error);
  }

  svn_boolean_t
  Property::isSvnProperty (const char * name)
  {
    const char *pname_utf8;
    Pool subPool;
    apr_pool_t *apr_pool = subPool.pool ();

    svn_utf_cstring_to_utf8 (&pname_utf8, name, NULL, apr_pool);
    svn_boolean_t is_svn_prop = svn_prop_is_svn_prop (pname_utf8);

    return is_svn_prop;
  }

}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
