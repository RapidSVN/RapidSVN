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

// stl
#include <string>

// svncpp
#include "url.hpp"

namespace svn
{
  static const int SCHEME_COUNT=5;
  static const char * 
  VALID_SCHEMES [SCHEME_COUNT] =
  {
    "http:", "https:", "svn:", "svn+ssh:", "file:"
  };

  Url::Url () {}

  Url::~Url () {}

  bool
  Url::isValid (const char * url)
  {
    std::string urlTest (url);
    for (int index=0; index < SCHEME_COUNT; index++)
    {
      std::string scheme = VALID_SCHEMES[index];
      std::string urlComp = urlTest.substr (0, scheme.length ());

      if (scheme == urlComp)
      {
        return true;
      }
    }

    return false;
  }

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
