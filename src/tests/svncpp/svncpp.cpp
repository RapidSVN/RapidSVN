
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
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Normal") == 0);
  CPPUNIT_ASSERT_EQUAL ((unsigned long)2, status.revision ());

  status.loadPath ("my_repos/main.cpp");
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Modified") == 0);
  CPPUNIT_ASSERT_EQUAL ((unsigned long)1, status.revision ());

  try
  {
    status.loadPath ("my_repos/status.cpp");
    status.statusText ();
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

  modify.checkout (REPOS_PATH, CHECKOUT_PATH, -1, 1);

  svn::Status status;
  status.loadPath ((char *)sPath.c_str ());

  CPPUNIT_ASSERT (status.isVersioned ());
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Normal") == 0);
  CPPUNIT_ASSERT_EQUAL ((unsigned long)2, status.revision ());
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
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Deleted") == 0);

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
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Normal") == 0);
  status.loadPath ("my_repos/main.cpp");
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Normal") == 0);
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
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Added") == 0);
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
    CPPUNIT_ASSERT (strcmp(status.statusText (), "Normal") == 0);
    CPPUNIT_ASSERT_EQUAL ((unsigned long)3, status.revision ());

    status.loadPath ("my_repos/main.cpp");
    CPPUNIT_ASSERT (strcmp(status.statusText (), "Normal") == 0);
    CPPUNIT_ASSERT_EQUAL ((unsigned long)2, status.revision ());
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
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Added") == 0);
  CPPUNIT_ASSERT_EQUAL ((unsigned long)2, status.revision ());
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
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Deleted") == 0);

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
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Added") == 0);
  CPPUNIT_ASSERT_EQUAL ((unsigned long)2, status.revision ());
}

void
SvnCppTestCase::testProperty ()
{
  svn::Property property;

  // Load and count tests
  try
  {
    property.loadPath ("my_repos/main.cpp");
  }
  catch (svn::ClientException &e)
  {
    CPPUNIT_ASSERT_MESSAGE (e.description (), false);
  }
  CPPUNIT_ASSERT_EQUAL (2, property.count ());

  // Iteration tests
  CPPUNIT_ASSERT (strcmp (property.nextProperty (), "test") == 0);
  CPPUNIT_ASSERT (strcmp (property.nextProperty (), "test2") == 0);
  CPPUNIT_ASSERT (property.nextProperty () == NULL);
  property.reset ();
  CPPUNIT_ASSERT (strcmp (property.nextProperty (), "test") == 0);

  // Get and set value tests
  CPPUNIT_ASSERT (strcmp (property.getValue ("test"), "test_value") == 0);
  property.set ("test", "new_value", false );
  CPPUNIT_ASSERT (strcmp (property.getValue ("test"), "new_value") == 0);

  property.remove ("test", false);
  CPPUNIT_ASSERT_EQUAL (1, property.count ());
}
