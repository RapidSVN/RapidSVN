#include "cppunit/TestCase.h"
#include "cppunit/extensions/HelperMacros.h"
#include "svncpp/status.h"
#include "svncpp/modify.h"
#include "svncpp/property.h"
#include "test_config.h"

/**
 * Tests the SvnCpp classes.
 */
class SvnCppTestCase : public CppUnit::TestCase
{
  CPPUNIT_TEST_SUITE( SvnCppTestCase );
  CPPUNIT_TEST( testStatus );
  CPPUNIT_TEST( testCheckout );
  CPPUNIT_TEST( testDeleteRevert );
  CPPUNIT_TEST( testAdd );
  CPPUNIT_TEST( testUpdate );
  CPPUNIT_TEST( testCommit );
  CPPUNIT_TEST( testCopy );
  CPPUNIT_TEST( testMove );
  CPPUNIT_TEST( testProperty );
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp ();

protected:
  /**
   * Tests all of the status methods.
   */
  void testStatus ();

  void testCheckout ();

  /**
   * Tests both the delete and revert functionality.
   */
  void testDeleteRevert ();

  void testAdd ();

  void testUpdate ();

  void testCommit ();

  void testCopy ();

  void testMove ();

  void testProperty ();
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

/**
 * Empty helper function for testCheckout.
 */
static void
notify (void *baton,
        const char *path,
        svn_wc_notify_action_t action,
        svn_node_kind_t kind,
        const char *mime_type,
        svn_wc_notify_state_t content_state,
        svn_wc_notify_state_t prop_state, svn_revnum_t revision)
{
}

/**
 * Creates an empty notify baton.
 */
NotifyBaton *
CreateNotifyBaton ()
{
  NotifyBaton *nb = new NotifyBaton;
  nb->received_some_change = FALSE;
  nb->sent_first_txdelta = FALSE;
  nb->is_checkout = 1;
  nb->suppress_final_line = 1;
  nb->pool = svn_pool_create (NULL);

  return nb;
}
