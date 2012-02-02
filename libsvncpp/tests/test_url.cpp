/*
 * ====================================================================
 * Copyright (c) 2002-2012 The RapidSVN Group.  All rights reserved.
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

// app
#include "test_url.hpp"
#include "svncpp/string_wrapper.hpp"

//svncpp
#include "svncpp/url.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(UrlTestCase);

void
UrlTestCase::setUp()
{
}

void
UrlTestCase::testIsValid()
{
  CPPUNIT_ASSERT(svn::Url::isValid("my_repos/some/dummy/path") == 0);
  CPPUNIT_ASSERT(svn::Url::isValid("\\is that a path?/\\/") == 0);
  CPPUNIT_ASSERT(svn::Url::isValid("https://yahoo.de/index.de.html") == 1);
  CPPUNIT_ASSERT(svn::Url::isValid("file:///local/file/here") == 1);
  CPPUNIT_ASSERT(svn::Url::isValid("http://yahoo.de") == 1);
}

void
UrlTestCase::testEscape()
{
  CPPUNIT_ASSERT("http://tigris.org/x%20y%20z.html" == svn::Url::escape("http://tigris.org/x y z.html"));
  CPPUNIT_ASSERT("http://tigris.org/xyz.php" == svn::Url::escape("http://tigris.org/xyz.php"));
  CPPUNIT_ASSERT("file:///local/file/here" == svn::Url::escape("file:///local/file/here"));

  // first encode single chars
  CPPUNIT_ASSERT("http://foo/%22" == svn::Url::escape("http://foo/\""));
  CPPUNIT_ASSERT("http://foo/%23" == svn::Url::escape("http://foo/#"));
  CPPUNIT_ASSERT("http://foo/%25" == svn::Url::escape("http://foo/%"));
  CPPUNIT_ASSERT("http://foo/%3B" == svn::Url::escape("http://foo/;"));
  CPPUNIT_ASSERT("http://foo/%3C" == svn::Url::escape("http://foo/<"));
  CPPUNIT_ASSERT("http://foo/%3E" == svn::Url::escape("http://foo/>"));
  CPPUNIT_ASSERT("http://foo/%3F" == svn::Url::escape("http://foo/?"));
  CPPUNIT_ASSERT("http://foo/%5B" == svn::Url::escape("http://foo/["));
  CPPUNIT_ASSERT("http://foo/%5C" == svn::Url::escape("http://foo/\\"));
  CPPUNIT_ASSERT("http://foo/%5D" == svn::Url::escape("http://foo/]"));
  CPPUNIT_ASSERT("http://foo/%5E" == svn::Url::escape("http://foo/^"));
  CPPUNIT_ASSERT("http://foo/%60" == svn::Url::escape("http://foo/`"));
  CPPUNIT_ASSERT("http://foo/%7B" == svn::Url::escape("http://foo/{"));
  CPPUNIT_ASSERT("http://foo/%7C" == svn::Url::escape("http://foo/|"));
  CPPUNIT_ASSERT("http://foo/%7D" == svn::Url::escape("http://foo/}"));
  CPPUNIT_ASSERT("http://foo/%20" == svn::Url::escape("http://foo/ "));

  // and then all together
  CPPUNIT_ASSERT("http://foo/%22%23%25%3B%3C%3E%3F%5B%5C%5D%5E%60%7B%7C%7D%20" == svn::Url::escape("http://foo/\"#%;<>?[\\]^`{|} "));
}


void
UrlTestCase::testUnescape()
{
  // this test is only a reversed test of UrlTestCase::testEscape

  CPPUNIT_ASSERT("http://tigris.org/x%20y%20z.html" == svn::Url::escape("http://tigris.org/x y z.html"));
  CPPUNIT_ASSERT("http://tigris.org/xyz.php" == svn::Url::escape("http://tigris.org/xyz.php"));
  CPPUNIT_ASSERT("file:///local/file/here" == svn::Url::escape("file:///local/file/here"));

  // first encode single chars
  CPPUNIT_ASSERT("http://foo/%22" == svn::Url::escape("http://foo/\""));
  CPPUNIT_ASSERT("http://foo/%23" == svn::Url::escape("http://foo/#"));
  CPPUNIT_ASSERT("http://foo/%25" == svn::Url::escape("http://foo/%"));
  CPPUNIT_ASSERT("http://foo/%3B" == svn::Url::escape("http://foo/;"));
  CPPUNIT_ASSERT("http://foo/%3C" == svn::Url::escape("http://foo/<"));
  CPPUNIT_ASSERT("http://foo/%3E" == svn::Url::escape("http://foo/>"));
  CPPUNIT_ASSERT("http://foo/%3F" == svn::Url::escape("http://foo/?"));
  CPPUNIT_ASSERT("http://foo/%5B" == svn::Url::escape("http://foo/["));
  CPPUNIT_ASSERT("http://foo/%5C" == svn::Url::escape("http://foo/\\"));
  CPPUNIT_ASSERT("http://foo/%5D" == svn::Url::escape("http://foo/]"));
  CPPUNIT_ASSERT("http://foo/%5E" == svn::Url::escape("http://foo/^"));
  CPPUNIT_ASSERT("http://foo/%60" == svn::Url::escape("http://foo/`"));
  CPPUNIT_ASSERT("http://foo/%7B" == svn::Url::escape("http://foo/{"));
  CPPUNIT_ASSERT("http://foo/%7C" == svn::Url::escape("http://foo/|"));
  CPPUNIT_ASSERT("http://foo/%7D" == svn::Url::escape("http://foo/}"));
  CPPUNIT_ASSERT("http://foo/%20" == svn::Url::escape("http://foo/ "));

  // and then all together
  CPPUNIT_ASSERT("http://foo/%22%23%25%3B%3C%3E%3F%5B%5C%5D%5E%60%7B%7C%7D%20" == svn::Url::escape("http://foo/\"#%;<>?[\\]^`{|} "));
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../../rapidsvn-dev.el")
 * end:
 */
