#include "svncpp.h"
#include <stdio.h>
#include <string>

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
  CPPUNIT_ASSERT_EQUAL ((long)2, status.revision ());

  status.loadPath ("my_repos/main.cpp");
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Modified") == 0);
  CPPUNIT_ASSERT_EQUAL ((long)1, status.revision ());

  status.loadPath ("my_repos/status.cpp");
  CPPUNIT_ASSERT (status.statusText () == NULL);
  CPPUNIT_ASSERT_EQUAL ((long)-1, status.revision ());
}

void
SvnCppTestCase::testCheckout ()
{
  svn::Modify modify;
  svn_wc_notify_func_t my_func = NULL;
  std::string sPath = CHECKOUT_PATH;
  sPath += "/README.txt";

  my_func = notify;
  void * notify_baton = CreateNotifyBaton ();

  modify.notification (my_func, notify_baton);
  CPPUNIT_ASSERT (modify.checkout (REPOS_PATH, CHECKOUT_PATH, -1, 1));

  svn::Status status;
  status.loadPath ((char *)sPath.c_str ());

  CPPUNIT_ASSERT (status.isVersioned ());
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Normal") == 0);
  CPPUNIT_ASSERT_EQUAL ((long)2, status.revision ());
}

void
SvnCppTestCase::testDeleteRevert ()
{
  svn::Modify modify;
  svn::Status status;
  svn::Error error;

  CPPUNIT_ASSERT (modify.remove ("my_repos/README.txt", false));

  status.loadPath ("my_repos/README.txt");
  CPPUNIT_ASSERT (status.isVersioned ());
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Deleted") == 0);

  // Test with forcing
  CPPUNIT_ASSERT (modify.remove ("my_repos/main.cpp", true));
  error.setError (modify.getError ());
  CPPUNIT_ASSERT (error.aprError () == -1);

  // Test revert
  CPPUNIT_ASSERT (modify.revert ("my_repos/README.txt", false));
  CPPUNIT_ASSERT (modify.revert ("my_repos/main.cpp", false));

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

  CPPUNIT_ASSERT (modify.add ("my_repos/svncpp.h", true));
  status.loadPath ("my_repos/svncpp.h");
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Added") == 0);
}

void
SvnCppTestCase::testUpdate ()
{
  svn::Modify modify;
  CPPUNIT_ASSERT (modify.update ("my_repos", -1, true));
}

void
SvnCppTestCase::testCommit ()
{
  svn::Modify modify;
  svn::Status status;

  modify.username ("");
  modify.password ("");
  CPPUNIT_ASSERT (modify.commit ("my_repos", 
                                 "Message logged by SvnCpp!", 
                                 true));

  status.loadPath("my_repos/svncpp.h");
  CPPUNIT_ASSERT (status.isVersioned ());
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Normal") == 0);
  CPPUNIT_ASSERT_EQUAL ((long)3, status.revision ());

  status.loadPath ("my_repos/main.cpp");
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Normal") == 0);
  CPPUNIT_ASSERT_EQUAL ((long)2, status.revision ());
}

void
SvnCppTestCase::testCopy ()
{
  svn::Modify modify;
  svn::Status status;

  CPPUNIT_ASSERT (modify.copy ("my_repos/main.cpp", 
                               "my_repos/my_dir/main2.cpp"));

  CPPUNIT_ASSERT (status.loadPath ("my_repos/my_dir/main2.cpp"));
  CPPUNIT_ASSERT (status.isVersioned ());
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Added") == 0);
  CPPUNIT_ASSERT_EQUAL ((long)2, status.revision ());
}

void
SvnCppTestCase::testMove ()
{
  svn::Modify modify;
  svn::Status status;

  CPPUNIT_ASSERT (modify.move ("my_repos/svncpp.h", 
                               "my_repos/my_dir/svncpp.h", -1, true));

  // Check if deleted
  CPPUNIT_ASSERT (status.loadPath ("my_repos/svncpp.h"));
  CPPUNIT_ASSERT (status.isVersioned ());
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Deleted") == 0);

  // Check if added
  CPPUNIT_ASSERT (status.loadPath ("my_repos/my_dir/svncpp.h"));
  CPPUNIT_ASSERT (status.isVersioned ());
  CPPUNIT_ASSERT (strcmp(status.statusText (), "Added") == 0);
  CPPUNIT_ASSERT_EQUAL ((long)2, status.revision ());
}

void
SvnCppTestCase::testProperty ()
{
  svn::Property property;

  // Load and count tests
  CPPUNIT_ASSERT (property.loadPath ("my_repos/main.cpp"));
  CPPUNIT_ASSERT_EQUAL (2, property.count ());

  // Iteration tests
  CPPUNIT_ASSERT (strcmp (property.nextProperty (), "test") == 0);
  CPPUNIT_ASSERT (strcmp (property.nextProperty (), "test2") == 0);
  CPPUNIT_ASSERT (property.nextProperty () == NULL);
  property.reset ();
  CPPUNIT_ASSERT (strcmp (property.nextProperty (), "test") == 0);

  // Get and set value tests
  CPPUNIT_ASSERT (strcmp (property.getValue ("test"), "test_value") == 0);
  CPPUNIT_ASSERT (property.set ("test", "new_value", false ));
  CPPUNIT_ASSERT (strcmp (property.getValue ("test"), "new_value") == 0);

  CPPUNIT_ASSERT (property.remove ("test", false));
  CPPUNIT_ASSERT_EQUAL (1, property.count ());
}
