#include "cppunit/TestCase.h"
#include "cppunit/ui/text/TestRunner.h"
#include "cppunit/extensions/HelperMacros.h"

int
main( int argc, char **argv)
{
  CppUnit::TextUi::TestRunner runner;
  CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();
  runner.addTest( registry.makeTest() );
  bool wasSucessful = runner.run( "", false );

  return wasSucessful;
}
