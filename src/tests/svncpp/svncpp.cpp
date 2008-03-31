/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
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

#include "svncpp.hpp"
#include <stdio.h>

CPPUNIT_TEST_SUITE_REGISTRATION( SvnCppTestCase );

void
SvnCppTestCase::setUp ()
{
}

void
SvnCppTestCase::testStatus ()
{
  svn::Client client;
  svn::Status * status = client.singleStatus ("my_repos/README.txt");

  CPPUNIT_ASSERT (status->isVersioned ());
  CPPUNIT_ASSERT (strcmp(status->textDescription (), "normal") == 0);
  CPPUNIT_ASSERT_EQUAL (svn_wc_status_normal, status->textType ());
  CPPUNIT_ASSERT_EQUAL ((svn_revnum_t)3, status->revision ());
  CPPUNIT_ASSERT (status->isLocked () == false);
  CPPUNIT_ASSERT (status->isCopied () == false);

  svn::Status * status2 = client.singleStatus ("my_repos/main.cpp");
  CPPUNIT_ASSERT (strcmp(status2->textDescription (), "modified") == 0);
  CPPUNIT_ASSERT_EQUAL (svn_wc_status_modified, status2->textType ());
  CPPUNIT_ASSERT_EQUAL ((svn_revnum_t)1, status2->revision ());

  try
  {
    svn::Status * status3 = client.singleStatus ("my_repos/status.cpp");
    status3->textDescription ();
    status3->revision ();
    CPPUNIT_ASSERT_MESSAGE ("No exception!", false);
  }
  catch (...)
  {
  }
}

void
SvnCppTestCase::testCheckout ()
{
  svn::Modify modify;
  std::string sPath = CHECKOUT_PATH;
  sPath += "/README.txt";

  modify.notification (&notify);

  try
  {
    modify.checkout ("fake_path", CHECKOUT_PATH, -1, 1);
    CPPUNIT_ASSERT (false); // it should not get here
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT (strlen(e.description ()) > 0);
    CPPUNIT_ASSERT (strlen(e.source ()) > 0);
  }

  try
  {
    modify.checkout (REPOS_PATH, CHECKOUT_PATH, -1, 1);
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (),
                            strlen(e.description ()) < 1);
  }

  svn::Client client;
  svn::Status * status = client.singleStatus (sPath.c_str ());

  CPPUNIT_ASSERT (status->isVersioned ());
  CPPUNIT_ASSERT (strcmp(status->textDescription (), "normal") == 0);
  CPPUNIT_ASSERT_EQUAL ((svn_revnum_t)3, status->revision ());
}

void
SvnCppTestCase::testDeleteRevert ()
{
  svn::Modify modify;
  svn::Client client;

  modify.notification (&notify);

  try
  {
    modify.remove ("my_repos/README.txt", false);
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }

  svn::Status * status = client.singleStatus ("my_repos/README.txt");
  CPPUNIT_ASSERT (status->isVersioned ());
  CPPUNIT_ASSERT (strcmp(status->textDescription (), "deleted") == 0);

  try
  {
    modify.remove ("my_repos/main.cpp", true); // with forcing
    modify.revert ("my_repos/README.txt", false);
    modify.revert ("my_repos/main.cpp", false);
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }

  /*
  status.loadPath ("my_repos/README.txt");
  CPPUNIT_ASSERT (strcmp(status.textDescription (), "normal") == 0);
  status.loadPath ("my_repos/main.cpp");
  CPPUNIT_ASSERT (strcmp(status.textDescription (), "normal") == 0);
  */
}

void
SvnCppTestCase::testAdd ()
{
  svn::Modify modify;
  svn::Client client;

  modify.notification (&notify);

  try
  {
    modify.add ("my_repos/svncpp.h", true);
    svn::Status * status = client.singleStatus ("my_repos/svncpp.h");
    CPPUNIT_ASSERT (strcmp(status->textDescription (), "added") == 0);
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }
}

void
SvnCppTestCase::testUpdate ()
{
  svn::Modify modify;

  modify.notification (&notify);

  try
  {
    modify.update ("my_repos", -1, true);
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }
}

void
SvnCppTestCase::testCommit ()
{
  svn::Modify modify;
  svn::Client client;

  modify.notification (&notify);

  modify.username ("");
  modify.password ("");
  try
  {
    modify.commit ("my_repos", "Message logged by SvnCpp!", true);
    svn::Status * status = client.singleStatus ("my_repos/svncpp.h");
    CPPUNIT_ASSERT (status->isVersioned ());
    CPPUNIT_ASSERT (strcmp(status->textDescription (), "normal") == 0);
    CPPUNIT_ASSERT_EQUAL ((svn_revnum_t)4, status->revision ());

    svn::Status * status2 = client.singleStatus ("my_repos/main.cpp");
    CPPUNIT_ASSERT (strcmp(status2->textDescription (), "normal") == 0);
    CPPUNIT_ASSERT_EQUAL ((svn_revnum_t)3, status2->revision ());
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }
}

void
SvnCppTestCase::testCopy ()
{
  svn::Modify modify;
  svn::Client client;

  modify.notification (&notify);

  try
  {
    modify.copy ("my_repos/main.cpp", "my_repos/my_dir/main2.cpp");
    svn::Status * status = client.singleStatus ("my_repos/my_dir/main2.cpp");
    CPPUNIT_ASSERT (status->isVersioned ());
    CPPUNIT_ASSERT (strcmp(status->textDescription (), "added") == 0);
    CPPUNIT_ASSERT_EQUAL ((svn_revnum_t)3, status->revision ());
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }
}

void
SvnCppTestCase::testMove ()
{
  svn::Modify modify;
  svn::Client client;

  modify.notification (&notify);

  try
  {
    modify.move ("my_repos/svncpp.h", "my_repos/my_dir/svncpp.h", -1, true);
    svn::Status * status = client.singleStatus ("my_repos/svncpp.h");
    CPPUNIT_ASSERT (status->isVersioned ());
    CPPUNIT_ASSERT (strcmp(status->textDescription (), "deleted") == 0);
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }

  // Check if added
  try
  {
    svn::Status * status2 = client.singleStatus ("my_repos/my_dir/svncpp.h");
    CPPUNIT_ASSERT (status2->isVersioned ());
    CPPUNIT_ASSERT (strcmp(status2->textDescription (), "added") == 0);
    CPPUNIT_ASSERT_EQUAL ((svn_revnum_t)3, status2->revision ());
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }
}

void
SvnCppTestCase::testProperty ()
{
  svn::Property property;

  property.loadPath ("my_repos/main.cpp");
  CPPUNIT_ASSERT (property.isVersioned ());
  CPPUNIT_ASSERT_EQUAL (2, property.count ());

  // Iteration tests
  CPPUNIT_ASSERT (property.next ());
  CPPUNIT_ASSERT (strcmp (property.name (), "test") == 0);
  CPPUNIT_ASSERT (strcmp (property.value (), "test_value") == 0);

  CPPUNIT_ASSERT (property.next ());
  CPPUNIT_ASSERT (strcmp (property.name (), "test2") == 0);
  CPPUNIT_ASSERT (strcmp (property.value (), "test_value2") == 0);

  CPPUNIT_ASSERT (property.first ());
  CPPUNIT_ASSERT (strcmp (property.name (), "test") == 0);
  CPPUNIT_ASSERT (strcmp (property.value (), "test_value") == 0);

  try
  {
    property.set ("new_test", "something", false);
  }
  catch (...)
  {
    CPPUNIT_ASSERT (false);
  }

  property.loadPath ("my_repos/main.cpp");
  CPPUNIT_ASSERT_EQUAL (3, property.count ());

  try
  {
    property.remove ("new_test", false);
  }
  catch (...)
  {
    CPPUNIT_ASSERT (false);
  }

  property.loadPath ("my_repos/main.cpp");
  CPPUNIT_ASSERT_EQUAL (2, property.count ());
}

void
SvnCppTestCase::testMkdir ()
{
  svn::Modify modify;
  svn::Client client;

  modify.notification (&notify);

  try
  {
    modify.mkdir ("my_repos/new_dir", "Created a new directory");
    svn::Status * status = client.singleStatus ("my_repos/new_dir");
    CPPUNIT_ASSERT (status->isVersioned ());
    CPPUNIT_ASSERT (strcmp(status->textDescription (), "added") == 0);
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }
}

void
SvnCppTestCase::testExport ()
{
  svn::Modify modify;
  svn::Client client;

  modify.notification (&notify);

  try
  {
    modify.doExport ("my_repos", "my_repos/export_dir", 2);
    svn::Status * status = client.singleStatus ("my_repos/export_dir");
    CPPUNIT_ASSERT (status == NULL);
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }
}

void
SvnCppTestCase::testLog ()
{
  svn::Log log;

  log.loadPath ("my_repos/README.txt", 1, 3);
  CPPUNIT_ASSERT (log.isVersioned ());

  CPPUNIT_ASSERT_EQUAL (3, log.count ());
  CPPUNIT_ASSERT (log.next ());
  CPPUNIT_ASSERT_EQUAL ((svn_revnum_t)1, log.revision ());

  // Test next more than one time.
  CPPUNIT_ASSERT (log.next ());
  CPPUNIT_ASSERT_EQUAL ((svn_revnum_t)2, log.revision ());
  CPPUNIT_ASSERT (log.previous ());
  CPPUNIT_ASSERT_EQUAL ((svn_revnum_t)1, log.revision ());
  CPPUNIT_ASSERT (log.last ());
  CPPUNIT_ASSERT_EQUAL ((svn_revnum_t)3, log.revision ());
  CPPUNIT_ASSERT (log.next () == false);

  // Test message and date
  CPPUNIT_ASSERT (strcmp ("more changes", log.message ()) == 0);
  CPPUNIT_ASSERT (strlen (log.date ()) > 0);

  // Test first
  CPPUNIT_ASSERT (log.first ());
  CPPUNIT_ASSERT_EQUAL ((svn_revnum_t)1, log.revision ());
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../../rapidsvn-dev.el")
 * end:
 */
