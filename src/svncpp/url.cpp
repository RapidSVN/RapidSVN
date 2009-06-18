/*
 * ====================================================================
 * Copyright (c) 2002-2009 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.  
 * If not, see <http://www.gnu.org/licenses/>.
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
#include "svn_path.h"

// svncpp
#include "svncpp/pool.hpp"
#include "svncpp/url.hpp"

namespace svn
{
  Url::Url() {}

  Url::~Url() {}

  bool
  Url::isValid(const char * urlToValidate)
  {
    std::string escapedUrlToValidate = escape(urlToValidate);

    return svn_path_is_url(escapedUrlToValidate.c_str()) != 0;
  }


  std::string
  Url::escape(const char * url)
  {
    Pool pool;

    return svn_path_uri_autoescape(url, pool);
  }


  std::string
  Url::unescape(const char * url)
  {
    Pool pool;

    return svn_path_uri_decode(url, pool);
  }

  /**
   * the implementation of the function that pull the supported
   * url schemas out of the ra layer it rather dirty now since
   * we are lacking a higher level of abstraction
   */
  std::vector<std::string>
  Url::supportedSchemas()
  {
    std::vector<std::string> schemas;

    return schemas;
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
