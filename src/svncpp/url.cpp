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

// stl
#include <string>
#include <vector>

// subversion api
#include "svn_ra.h"

// svncpp
#include "svncpp/pool.hpp"
#include "svncpp/url.hpp"


namespace svn
{
  static const int SCHEMA_COUNT=5;
  static const char * 
  VALID_SCHEMAS [SCHEMA_COUNT] =
  {
    "http:", "https:", "svn:", "svn+ssh:", "file:"
  };

  static bool mSchemasInitialized = false;
  std::vector<std::string> mSchemas;

  Url::Url () {}

  Url::~Url () {}

  bool
  Url::isValid (const char * url)
  {
    std::string urlTest (url);
    for (int index=0; index < SCHEMA_COUNT; index++)
    {
      std::string schema = VALID_SCHEMAS[index];
      std::string urlComp = urlTest.substr (0, schema.length ());

      if (schema == urlComp)
      {
        return true;
      }
    }

    return false;
  }

  /**
   * the implementation of the function that pull the supported
   * url schemas out of the ra layer it rather dirty now since
   * we are lacking a higher level of abstraction
   */
  std::vector<std::string>
  Url::supportedSchemas ()
  {
    if (mSchemasInitialized)
      return mSchemas;

    mSchemasInitialized = true;
    Pool pool;
    void * ra_baton;

    svn_error_t * error = 
      svn_ra_init_ra_libs (&ra_baton, pool);
    if (error)
      return mSchemas;

    svn_stringbuf_t *descr;
    error = 
      svn_ra_print_ra_libraries (&descr, ra_baton, pool);
    if (error)
      return mSchemas;

    // schemas are in the following form:
    // <schema>:<whitespace><description>\n...
    // find the fírst :
    std::string descriptions (descr->data);
    size_t pos=0;
    const size_t not_found = std::string::npos;
    do
    {
      const std::string tokenStart ("handles '");
      const std::string tokenEnd ("' schema");
      pos = descriptions.find (tokenStart, pos);
      if (pos == not_found)
        break;

      pos += tokenStart.length ();

      size_t posEnd = descriptions.find (tokenEnd, pos);
      if (posEnd == not_found)
        break;

      // found
      std::string schema (descriptions.substr (pos, posEnd-pos) + ":");
      mSchemas.push_back (schema);

      // forward to the next newline
      pos = posEnd + tokenEnd.length ();
    }
    while (pos != not_found);

    return mSchemas;
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
