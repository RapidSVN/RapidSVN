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
#include "revision.hpp"


namespace svn
{

  PropertyEntry::PropertyEntry (const char * name, const char * value)
  {
    this->name = name;
    this->value = value;
  }

  Property::Property (Context * context, const Path & path)
    : m_context (context), m_path (path)
  {
    list ();
  }

  Property::~Property ()
  {
  }

  void
  Property::list ()
  {
    Pool pool;
    Revision revision;

    m_entries.clear ();
    apr_array_header_t * props;
    svn_error_t * error =
      svn_client_proplist (&props,
                           m_path.c_str (), 
                           revision,
                           false, /* recurse */
                           *m_context,
                           pool);
    if(error != NULL)
    {
      throw ClientException (error);
    }

    for (int j = 0; j < props->nelts; ++j)
    {
      svn_client_proplist_item_t *item = 
        ((svn_client_proplist_item_t **)props->elts)[j];

      const char *node_name_native;
      svn_utf_cstring_from_utf8_stringbuf (&node_name_native,
                                           item->node_name,
                                           pool );

      apr_hash_index_t *hi;

      for (hi = apr_hash_first (pool, item->prop_hash); hi; 
           hi = apr_hash_next (hi))
      {
        const void *key;
        const char *key_native;
        void *val;

        apr_hash_this (hi, &key, NULL, &val);
        svn_utf_cstring_from_utf8 (&key_native, (char *)key, pool);

        m_entries.push_back (PropertyEntry (key_native, getValue (key_native).c_str ()));
      } 
    }
  }

  std::string 
  Property::getValue (const char * name)
  {
    Pool pool;
    Revision revision;

    apr_hash_t *props;
    svn_client_propget (&props, 
                        name, 
                        m_path.c_str (),
                        revision,
                        false, // recurse
                        *m_context,
                        pool);

    //svn_boolean_t is_svn_prop = svn_prop_is_svn_prop (name);

    apr_hash_index_t *hi;
    hi = apr_hash_first (pool, props); 
    if( !hi )
    {
      return "";
    }

    const void *key;
    void *val;
    const svn_string_t *propval;
    apr_hash_this (hi, &key, NULL, &val);
    propval = (const svn_string_t *)val;

    /* If this is a special Subversion property, it is stored as
       UTF8, so convert to the native format. */
    //TODO we are doing this now all the time
    //if (is_svn_prop)
    svn_utf_string_from_utf8 (&propval, propval, pool);

    return propval->data;
  }

  void
  Property::set (const char * name, const char * value)
  {
    Pool pool;

    const svn_string_t * propval 
      = svn_string_create ((const char *) value, pool);

    const char *pname_utf8;
    svn_utf_cstring_to_utf8 (&pname_utf8, name, NULL, pool);

    svn_error_t * error = 
      svn_client_propset (pname_utf8, propval, m_path.c_str (),
                          false, pool);
    if(error != NULL)
      throw ClientException (error);
  }

  void 
  Property::remove (const char * name)
  {
    Pool pool;

    const char *pname_utf8;
    svn_utf_cstring_to_utf8 (&pname_utf8, name, NULL, pool);

    svn_error_t * error = 
      error = svn_client_propset (pname_utf8, 
                                  NULL, // value = NULL
                                  m_path.c_str (),
                                  false, //dont recurse
                                  pool);
    if(error != NULL)
      throw ClientException (error);
  }

  /* REMOVE
  bool
  PropertyValue::isSvnProperty (const char * name)
  {
    Pool pool;

    const char *pname_utf8;
    svn_utf_cstring_to_utf8 (&pname_utf8, name, NULL, pool);
    svn_boolean_t is_svn_prop = svn_prop_is_svn_prop (pname_utf8);

    return is_svn_prop;
  }
  */

}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
