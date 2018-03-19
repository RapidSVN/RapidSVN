/*
 * ====================================================================
 * Copyright (c) 2002-2018 The RapidSVN Group.  All rights reserved.
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

#ifndef LOG_DLG_TESTS_H
#define LOG_DLG_TESTS_H

#include "log_dlg.hpp"

#include "cppunit/TestCase.h"
#include "cppunit/extensions/HelperMacros.h"


class LogDlgTests : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE(LogDlgTests);
  //CPPUNIT_TEST(testInit);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp()
  {
    m_logEntries = svn::LogEntries();
    m_dlg = new LogDlg(0, "/foo/path/", &m_logEntries);
  }

  void tearDown()
  {
  }

protected:
  LogDlg *m_dlg;
  svn::LogEntries m_logEntries;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
