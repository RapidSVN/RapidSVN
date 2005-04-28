/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
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
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../../rapidsvn-dev.el")
 * end:
 */
