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
#if defined( _MSC_VER) && _MSC_VER <= 1200
#pragma warning( disable: 4786 )// debug symbol truncated
#endif

// subversion api
#include "svn_client.h"
#include "svn_utf.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/path.hpp"
#include "svncpp/exception.hpp"
#include "svncpp/pool.hpp"
#include "svncpp/revision.hpp"


namespace svn
{


  /**
   * lists properties in @a path no matter whether local or
   * repository
   *
   * @param path
   * @param revision
   * @param recurse
   * @return PropertiesList
   */
  PathPropertiesMapList
  Client::proplist(const Path &path,
                   const Revision &revision,
                   bool recurse)
  {
    Pool pool;

    apr_array_header_t * props;
    svn_error_t * error =
      svn_client_proplist (&props,
                           path.c_str (), 
                           revision.revision (),
                           recurse,
                           *m_context,
                           pool);
    if(error != NULL)
    {
      throw ClientException (error);
    }

    PathPropertiesMapList path_prop_map_list;
    for (int j = 0; j < props->nelts; ++j)
    {
      svn_client_proplist_item_t *item = 
        ((svn_client_proplist_item_t **)props->elts)[j];

      const char *node_name_native;
      svn_utf_cstring_from_utf8_stringbuf (&node_name_native,
                                           item->node_name,
                                           pool );

      PropertiesMap prop_map;

      apr_hash_index_t *hi;
      for (hi = apr_hash_first (pool, item->prop_hash); hi; 
           hi = apr_hash_next (hi))
      {
        const void *key;
        const char *key_native;
        void *val;

        apr_hash_this (hi, &key, NULL, &val);
        svn_utf_cstring_from_utf8 (&key_native, (char *)key, pool);
        const svn_string_t *propval = (const svn_string_t *)val;
        svn_utf_string_from_utf8 (&propval, propval, pool);
        const char *prop_val_str = propval->data;

        prop_map[ std::string( key_native ) ] = std::string( propval->data );
      }

      path_prop_map_list.push_back( PathPropertiesMapEntry( node_name_native,prop_map ) );
    }

    return path_prop_map_list;
  }

  /**
   * lists properties in @a path no matter whether local or
   * repository
   *
   * @param path
   * @param revision
   * @param recurse
   * @return PropertiesList
   */
  
  PathPropertiesMapList
  Client::propget(const char *propName,
                  const Path &path,
                  const Revision &revision,
                  bool recurse)
  {
    Pool pool;

    apr_hash_t *props;
    svn_error_t * error =
      svn_client_propget (&props,
                           propName,
                           path.c_str (), 
                           revision.revision (),
                           recurse,
                           *m_context,
                           pool);
    if(error != NULL)
    {
      throw ClientException (error);
    }

    PathPropertiesMapList path_prop_map_list;


    apr_hash_index_t *hi;
    for (hi = apr_hash_first (pool, props); hi; 
         hi = apr_hash_next (hi))
    {
      PropertiesMap prop_map;

      const void *key;
      const char *key_native;
      void *val;

      apr_hash_this (hi, &key, NULL, &val);
      svn_utf_cstring_from_utf8 (&key_native, (char *)key, pool);
      const svn_string_t *propval = (const svn_string_t *)val;
      svn_utf_string_from_utf8 (&propval, propval, pool);
      const char *prop_val_str = propval->data;

      prop_map[ std::string( propName ) ] = std::string( propval->data );

      path_prop_map_list.push_back( PathPropertiesMapEntry( key_native, prop_map ) );
    }

    return path_prop_map_list;
  }

  /**
   * set property in @a path no matter whether local or
   * repository
   *
   * @param path
   * @param revision
   * @param propName
   * @param propValue
   * @param recurse
   * @return PropertiesList
   */
  void
  Client::propset(const char *propName,
                  const char *propValue,
                  const Path &path,
                  const Revision &revision,
                  bool recurse)
    {
      Pool pool;
      const svn_string_t * propval 
        = svn_string_create ((const char *) propValue, pool);

      const char *pname_utf8;
      svn_utf_cstring_to_utf8 (&pname_utf8, propName, pool);

      svn_error_t * error = 
        svn_client_propset (pname_utf8, propval, path.c_str (),
                            recurse, pool);
      if(error != NULL)
        throw ClientException (error);
    }

  /**
   * delete property in @a path no matter whether local or
   * repository
   *
   * @param path
   * @param revision
   * @param propName
   * @param propValue
   * @param recurse
   * @return PropertiesList
   */
  void
  Client::propdel(const char *propName,
                  const Path &path,
                  const Revision &revision,
                  bool recurse)
  {
    Pool pool;

    const char *pname_utf8;
    svn_utf_cstring_to_utf8 (&pname_utf8, propName, pool);

    svn_error_t * error = 
      error = svn_client_propset (pname_utf8, 
                                  NULL, // value = NULL
                                  path.c_str (),
                                  recurse,
                                  pool);
    if(error != NULL)
      throw ClientException (error);
  }
//--------------------------------------------------------------------------------
//
//    revprop functions
//
//--------------------------------------------------------------------------------
  /**
   * lists revision properties in @a path no matter whether local or
   * repository
   *
   * @param path
   * @param revision
   * @param recurse
   * @return PropertiesList
   */
  std::pair<svn_revnum_t,PropertiesMap>
  Client::revproplist(const Path &path,
                      const Revision &revision)
  {
    Pool pool;

    apr_hash_t * props;
    svn_revnum_t revnum;
    svn_error_t * error =
      svn_client_revprop_list (&props,
                               path.c_str (), 
                               revision.revision (),
                               &revnum,
                               *m_context,
                               pool);
    if(error != NULL)
    {
      throw ClientException (error);
    }

    PropertiesMap prop_map;

    apr_hash_index_t *hi;
    for (hi = apr_hash_first (pool, props); hi; 
         hi = apr_hash_next (hi))
    {
      const void *key;
      const char *key_native;
      void *val;

      apr_hash_this (hi, &key, NULL, &val);
      svn_utf_cstring_from_utf8 (&key_native, (char *)key, pool);
      const svn_string_t *propval = (const svn_string_t *)val;
      svn_utf_string_from_utf8 (&propval, propval, pool);
      const char *prop_val_str = propval->data;

      prop_map[ std::string( key_native ) ] = std::string( propval->data );
    }

    return std::pair<svn_revnum_t,PropertiesMap>( revnum, prop_map );
  }

  /**
   * lists one revision property in @a path no matter whether local or
   * repository
   *
   * @param path
   * @param revision
   * @param recurse
   * @return PropertiesList
   */
  
  std::pair<svn_revnum_t,std::string>
  Client::revpropget(const char *propName,
                     const Path &path,
                     const Revision &revision)
  {
    Pool pool;

    svn_string_t *propval;
    svn_revnum_t revnum;
    svn_error_t * error =
      svn_client_revprop_get (propName,
                              &propval,
                              path.c_str (), 
                              revision.revision (),
                              &revnum,
                              *m_context,
                              pool);
    if(error != NULL)
    {
      throw ClientException (error);
    }

    // if the property does not exist NULL is returned
    if( propval == NULL )
      return std::pair<svn_revnum_t,std::string>( 0, std::string() );

    const svn_string_t *propval_str;
    svn_utf_string_from_utf8 (&propval_str, propval, pool);

    return std::pair<svn_revnum_t,std::string>( revnum, std::string (propval_str->data) );
  }

  /**
   * set property in @a path no matter whether local or
   * repository
   *
   * @param path
   * @param revision
   * @param propName
   * @param propValue
   * @param recurse
   * @param revprop
   * @return PropertiesList
   */
  svn_revnum_t
  Client::revpropset(const char *propName,
                     const char *propValue,
                     const Path &path,
                     const Revision &revision,
                     bool force)
  {
    Pool pool;

    const svn_string_t * propval 
      = svn_string_create ((const char *) propValue, pool);

    const char *pname_utf8;
    svn_utf_cstring_to_utf8 (&pname_utf8, propName, pool);

    svn_revnum_t revnum;
    svn_error_t * error = 
      svn_client_revprop_set (pname_utf8,
                              propval,
                              path.c_str (),
                              revision.revision (),
                              &revnum,
                              force,
                              *m_context,
                              pool);
    if(error != NULL)
      throw ClientException (error);

    return revnum;
  }

  /**
   * delete property in @a path no matter whether local or
   * repository
   *
   * @param path
   * @param revision
   * @param propName
   * @param propValue
   * @param recurse
   * @param revprop
   * @return PropertiesList
   */
  svn_revnum_t
  Client::revpropdel(const char *propName,
                  const Path &path,
                  const Revision &revision,
                  bool force)
  {
    Pool pool;

    const char *pname_utf8;
    svn_utf_cstring_to_utf8 (&pname_utf8, propName, pool);

    svn_revnum_t revnum;
    svn_error_t * error = 
      error = svn_client_revprop_set (pname_utf8, 
                                      NULL, // value = NULL
                                      path.c_str (),
                                      revision.revision (),
                                      &revnum,
                                      force,
                                      *m_context,
                                      pool);
    if(error != NULL)
      throw ClientException (error);

    return revnum;
  }

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
