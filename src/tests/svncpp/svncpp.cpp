
#include "svncpp.h"
#include <stdio.h>

CPPUNIT_TEST_SUITE_REGISTRATION( SvnCppTestCase );

void
SvnCppTestCase::setUp ()
{
}

void
SvnCppTestCase::testStatus ()
{
  svn::Status status;
  status.password("");
  status.loadPath("my_repos/README.txt");

  CPPUNIT_ASSERT (status.isVersioned ());
  CPPUNIT_ASSERT (strcmp(status.textDescription (), "normal") == 0);
  CPPUNIT_ASSERT_EQUAL (svn_wc_status_normal, status.textType ());
  CPPUNIT_ASSERT_EQUAL ((unsigned long)3, status.revision ());
  CPPUNIT_ASSERT (status.isLocked () == false);
  CPPUNIT_ASSERT (status.isCopied () == false);

  status.loadPath ("my_repos/main.cpp");
  CPPUNIT_ASSERT (strcmp(status.textDescription (), "modified") == 0);
  CPPUNIT_ASSERT_EQUAL (svn_wc_status_modified, status.textType ());
  CPPUNIT_ASSERT_EQUAL ((unsigned long)1, status.revision ());

  try
  {
    status.loadPath ("my_repos/status.cpp");
    status.textDescription ();
    status.revision ();
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

  svn::Status status;
  status.loadPath ((char *)sPath.c_str ());

  CPPUNIT_ASSERT (status.isVersioned ());
  CPPUNIT_ASSERT (strcmp(status.textDescription (), "normal") == 0);
  CPPUNIT_ASSERT_EQUAL ((unsigned long)3, status.revision ());
}

void
SvnCppTestCase::testDeleteRevert ()
{
  svn::Modify modify;
  svn::Status status;

  modify.notification (&notify);

  try
  {
    modify.remove ("my_repos/README.txt", false);
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }

  status.loadPath ("my_repos/README.txt");
  CPPUNIT_ASSERT (status.isVersioned ());
  CPPUNIT_ASSERT (strcmp(status.textDescription (), "deleted") == 0);

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

  status.loadPath ("my_repos/README.txt");
  CPPUNIT_ASSERT (strcmp(status.textDescription (), "normal") == 0);
  status.loadPath ("my_repos/main.cpp");
  CPPUNIT_ASSERT (strcmp(status.textDescription (), "normal") == 0);
}

void
SvnCppTestCase::testAdd ()
{
  svn::Modify modify;
  svn::Status status;

  modify.notification (&notify);

  try
  {
    modify.add ("my_repos/svncpp.h", true);
    status.loadPath ("my_repos/svncpp.h");
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }
  CPPUNIT_ASSERT (strcmp(status.textDescription (), "added") == 0);
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
  svn::Status status;

  modify.notification (&notify);

  modify.username ("");
  modify.password ("");
  try
  {
    modify.commit ("my_repos", "Message logged by SvnCpp!", true);
    status.loadPath("my_repos/svncpp.h");
    CPPUNIT_ASSERT (status.isVersioned ());
    CPPUNIT_ASSERT (strcmp(status.textDescription (), "normal") == 0);
    CPPUNIT_ASSERT_EQUAL ((unsigned long)4, status.revision ());

    status.loadPath ("my_repos/main.cpp");
    CPPUNIT_ASSERT (strcmp(status.textDescription (), "normal") == 0);
    CPPUNIT_ASSERT_EQUAL ((unsigned long)3, status.revision ());
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
  svn::Status status;

  modify.notification (&notify);

  try
  {
    modify.copy ("my_repos/main.cpp", "my_repos/my_dir/main2.cpp");
    status.loadPath ("my_repos/my_dir/main2.cpp");
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }
  CPPUNIT_ASSERT (status.isVersioned ());
  CPPUNIT_ASSERT (strcmp(status.textDescription (), "added") == 0);
  CPPUNIT_ASSERT_EQUAL ((unsigned long)3, status.revision ());
}

void
SvnCppTestCase::testMove ()
{
  svn::Modify modify;
  svn::Status status;

  modify.notification (&notify);

  try
  {
    modify.move ("my_repos/svncpp.h", "my_repos/my_dir/svncpp.h", -1, true);
    status.loadPath ("my_repos/svncpp.h");
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }

  CPPUNIT_ASSERT (status.isVersioned ());
  CPPUNIT_ASSERT (strcmp(status.textDescription (), "deleted") == 0);

  // Check if added
  try
  {
    status.loadPath ("my_repos/my_dir/svncpp.h");
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }
  CPPUNIT_ASSERT (status.isVersioned ());
  CPPUNIT_ASSERT (strcmp(status.textDescription (), "added") == 0);
  CPPUNIT_ASSERT_EQUAL ((unsigned long)3, status.revision ());
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
  svn::Status status;

  modify.notification (&notify);

  try
  {
    modify.mkdir ("my_repos/new_dir", "Created a new directory");
    status.loadPath ("my_repos/new_dir");
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }

  CPPUNIT_ASSERT (status.isVersioned ());
  CPPUNIT_ASSERT (strcmp(status.textDescription (), "added") == 0);
}

void
SvnCppTestCase::testExport ()
{
  svn::Modify modify;
  svn::Status status;

  modify.notification (&notify);

  try
  {
    modify.doExport ("my_repos", "my_repos/export_dir", 2);
    status.loadPath ("my_repos/export_dir");
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }

  CPPUNIT_ASSERT (status.isVersioned () == false);
}

void
SvnCppTestCase::testLog ()
{
  svn::Log log;

  log.loadPath ("my_repos/README.txt", 1, 3);
  CPPUNIT_ASSERT (log.isVersioned ());

  CPPUNIT_ASSERT_EQUAL (3, log.count ());
  CPPUNIT_ASSERT (log.next ());
  CPPUNIT_ASSERT_EQUAL ((unsigned long)1, log.revision ());

  // Test next more than one time.
  CPPUNIT_ASSERT (log.next ());
  CPPUNIT_ASSERT_EQUAL ((unsigned long)2, log.revision ());
  CPPUNIT_ASSERT (log.previous ());
  CPPUNIT_ASSERT_EQUAL ((unsigned long)1, log.revision ());
  CPPUNIT_ASSERT (log.last ());
  CPPUNIT_ASSERT_EQUAL ((unsigned long)3, log.revision ());
  CPPUNIT_ASSERT (log.next () == false);

  // Test message and date
  CPPUNIT_ASSERT (strcmp ("more changes", log.message ()) == 0);
  CPPUNIT_ASSERT (strlen (log.date ()) > 0);

  // Test first
  CPPUNIT_ASSERT (log.first ());
  CPPUNIT_ASSERT_EQUAL ((unsigned long)1, log.revision ());
}

