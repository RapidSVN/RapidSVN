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
  Svn::Status status;
  status.Password("");
  status.LoadPath("my_repos/README.txt");

  CPPUNIT_ASSERT (status.IsVersioned ());
  CPPUNIT_ASSERT (strcmp(status.StatusText (), "Normal") == 0);
  CPPUNIT_ASSERT_EQUAL ((long)2, status.Revision ());

  status.LoadPath ("my_repos/main.cpp");
  CPPUNIT_ASSERT (strcmp(status.StatusText (), "Modified") == 0);
  CPPUNIT_ASSERT_EQUAL ((long)1, status.Revision ());

  status.LoadPath ("my_repos/status.cpp");
  CPPUNIT_ASSERT (status.StatusText () == NULL);
  CPPUNIT_ASSERT_EQUAL ((long)-1, status.Revision ());
}

void
SvnCppTestCase::testCheckout ()
{
  Svn::Modify modify;
  svn_wc_notify_func_t my_func = NULL;
  std::string sPath = CHECKOUT_PATH;
  sPath += "/README.txt";

  my_func = notify;
  void * notify_baton = CreateNotifyBaton ();

  modify.Notification (my_func, notify_baton);
  CPPUNIT_ASSERT (modify.Checkout (REPOS_PATH, CHECKOUT_PATH, -1, 1));

  Svn::Status status;
  status.LoadPath ((char *)sPath.c_str ());

  CPPUNIT_ASSERT (status.IsVersioned ());
  CPPUNIT_ASSERT (strcmp(status.StatusText (), "Normal") == 0);
  CPPUNIT_ASSERT_EQUAL ((long)2, status.Revision ());
}

void
SvnCppTestCase::testDeleteRevert ()
{
  Svn::Modify modify;
  Svn::Status status;
  Svn::Error error;

  CPPUNIT_ASSERT (modify.Delete ("my_repos/README.txt", false));

  status.LoadPath ("my_repos/README.txt");
  CPPUNIT_ASSERT (status.IsVersioned ());
  CPPUNIT_ASSERT (strcmp(status.StatusText (), "Deleted") == 0);

  // Test with forcing
  CPPUNIT_ASSERT (modify.Delete ("my_repos/main.cpp", true));
  error.SetError (modify.GetError ());
  CPPUNIT_ASSERT (error.AprError () == -1);

  // Test revert
  CPPUNIT_ASSERT (modify.Revert ("my_repos/README.txt", false));
  CPPUNIT_ASSERT (modify.Revert ("my_repos/main.cpp", false));

  status.LoadPath ("my_repos/README.txt");
  CPPUNIT_ASSERT (strcmp(status.StatusText (), "Normal") == 0);
  status.LoadPath ("my_repos/main.cpp");
  CPPUNIT_ASSERT (strcmp(status.StatusText (), "Normal") == 0);
}

void
SvnCppTestCase::testAdd ()
{
  Svn::Modify modify;
  Svn::Status status;

  CPPUNIT_ASSERT (modify.Add ("my_repos/svncpp.h", true));
  status.LoadPath ("my_repos/svncpp.h");
  CPPUNIT_ASSERT (strcmp(status.StatusText (), "Added") == 0);
}

void
SvnCppTestCase::testUpdate ()
{
  Svn::Modify modify;
  CPPUNIT_ASSERT (modify.Update ("my_repos", -1, true));
}

void
SvnCppTestCase::testCommit ()
{
  Svn::Modify modify;
  Svn::Status status;

  modify.Username ("");
  modify.Password ("");
  CPPUNIT_ASSERT (modify.Commit ("my_repos", 
                                 "Message logged by SvnCpp!", 
                                 true));

  status.LoadPath("my_repos/svncpp.h");
  CPPUNIT_ASSERT (status.IsVersioned ());
  CPPUNIT_ASSERT (strcmp(status.StatusText (), "Normal") == 0);
  CPPUNIT_ASSERT_EQUAL ((long)3, status.Revision ());

  status.LoadPath ("my_repos/main.cpp");
  CPPUNIT_ASSERT (strcmp(status.StatusText (), "Normal") == 0);
  CPPUNIT_ASSERT_EQUAL ((long)2, status.Revision ());
}

void
SvnCppTestCase::testCopy ()
{
  Svn::Modify modify;
  Svn::Status status;

  CPPUNIT_ASSERT (modify.Copy ("my_repos/main.cpp", 
                               "my_repos/my_dir/main2.cpp"));

  CPPUNIT_ASSERT (status.LoadPath ("my_repos/my_dir/main2.cpp"));
  CPPUNIT_ASSERT (status.IsVersioned ());
  CPPUNIT_ASSERT (strcmp(status.StatusText (), "Added") == 0);
  CPPUNIT_ASSERT_EQUAL ((long)2, status.Revision ());
}

void
SvnCppTestCase::testMove ()
{
  Svn::Modify modify;
  Svn::Status status;

  CPPUNIT_ASSERT (modify.Move ("my_repos/svncpp.h", 
                               "my_repos/my_dir/svncpp.h", -1, true));

  // Check if deleted
  CPPUNIT_ASSERT (status.LoadPath ("my_repos/svncpp.h"));
  CPPUNIT_ASSERT (status.IsVersioned ());
  CPPUNIT_ASSERT (strcmp(status.StatusText (), "Deleted") == 0);

  // Check if added
  CPPUNIT_ASSERT (status.LoadPath ("my_repos/my_dir/svncpp.h"));
  CPPUNIT_ASSERT (status.IsVersioned ());
  CPPUNIT_ASSERT (strcmp(status.StatusText (), "Added") == 0);
  CPPUNIT_ASSERT_EQUAL ((long)2, status.Revision ());
}

void
SvnCppTestCase::testProperty ()
{
  Svn::Property property;

  // Load and count tests
  CPPUNIT_ASSERT (property.LoadPath ("my_repos/main.cpp"));
  CPPUNIT_ASSERT_EQUAL (2, property.Count ());

  // Iteration tests
  CPPUNIT_ASSERT (strcmp (property.NextProperty (), "test") == 0);
  CPPUNIT_ASSERT (strcmp (property.NextProperty (), "test2") == 0);
  CPPUNIT_ASSERT (property.NextProperty () == NULL);
  property.Reset ();
  CPPUNIT_ASSERT (strcmp (property.NextProperty (), "test") == 0);

  // Get and set value tests
  CPPUNIT_ASSERT (strcmp (property.GetValue ("test"), "test_value") == 0);
  CPPUNIT_ASSERT (property.Set ("test", "new_value", false ));
  CPPUNIT_ASSERT (strcmp (property.GetValue ("test"), "new_value") == 0);

  CPPUNIT_ASSERT (property.Delete ("test", false));
  CPPUNIT_ASSERT_EQUAL (1, property.Count ());
}
