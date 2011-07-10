/*
 * ====================================================================
 * Copyright (c) 2002-2011 The RapidSVN Group.  All rights reserved.
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
#include "cppunit/TestCase.h"
#include "cppunit/extensions/HelperMacros.h"
#include "svncpp/status.hpp"
#include "svncpp/modify.hpp"
#include "svncpp/property.hpp"
#include "svncpp/log.hpp"
#include "test_config.hpp"

/**
 * Tests the SvnCpp classes.
 */
class SvnCppTestCase : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE(SvnCppTestCase);
  CPPUNIT_TEST(testStatus);
  CPPUNIT_TEST(testCheckout);
  CPPUNIT_TEST(testDeleteRevert);
  CPPUNIT_TEST(testAdd);
  CPPUNIT_TEST(testUpdate);
  CPPUNIT_TEST(testCommit);
  CPPUNIT_TEST(testCopy);
  CPPUNIT_TEST(testMove);
  CPPUNIT_TEST(testMkdir);
  CPPUNIT_TEST(testExport);
  CPPUNIT_TEST(testProperty);
  CPPUNIT_TEST(testLog);
  CPPUNIT_TEST_SUITE_END();

private:
  svn::Notify notify;

public:
  void setUp();

protected:
  /**
   * Tests all of the status methods.
   */
  void testStatus();

  void testCheckout();

  void testDeleteRevert();

  void testAdd();

  void testUpdate();

  void testCommit();

  void testCopy();

  void testMove();

  void testProperty();

  void testMkdir();

  void testExport();

  void testLog();
};

/**
 * Required for testCheckout.
 */
struct NotifyBaton
{
  svn_boolean_t received_some_change;
  svn_boolean_t is_checkout;
  svn_boolean_t suppress_final_line;
  svn_boolean_t sent_first_txdelta;
  apr_pool_t *pool;
};
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../../rapidsvn-dev.el")
 * end:
 */
