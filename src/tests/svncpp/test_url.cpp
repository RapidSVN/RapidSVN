/*
 * ====================================================================
 * Copyright (c) 2002-2006 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// app
#include "test_url.hpp"
#include <string>

//svncpp
#include "svncpp/url.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION (UrlTestCase);

void
UrlTestCase::setUp ()
{
}

void
UrlTestCase::testIsValid ()
{
  CPPUNIT_ASSERT (svn::Url::isValid ("my_repos/some/dummy/path") == 0);
  CPPUNIT_ASSERT (svn::Url::isValid ("\\is that a path?/\\/") == 0);
  CPPUNIT_ASSERT (svn::Url::isValid ("https://yahoo.de/index.de.html") == 1);
  CPPUNIT_ASSERT (svn::Url::isValid ("file:///local/file/here") == 1);
  CPPUNIT_ASSERT (svn::Url::isValid ("http://yahoo.de") == 1);
}

void
UrlTestCase::testEscape ()
{
  CPPUNIT_ASSERT ("http://tigris.org/x%20y%20z.html" == svn::Url::escape ("http://tigris.org/x y z.html"));
  CPPUNIT_ASSERT ("http://tigris.org/xyz.php" == svn::Url::escape ("http://tigris.org/xyz.php"));
  CPPUNIT_ASSERT ("file:///local/file/here" == svn::Url::escape ("file:///local/file/here"));
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../../rapidsvn-dev.el")
 * end:
 */
