/*
 * ====================================================================
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#include "include.h"
#include "rapidsvn_app.h"

IMPLEMENT_APP (RapidSvnApp)
     bool RapidSvnApp::OnInit ()
{
  // application and vendor name are used by wxConfig to construct the name
  // of the config file/registry key and must be set before the first call
  // to Get() if you want to override the default values (the application
  // name is the name of the executable and the vendor name is the same)
  SetVendorName (APPLICATION_NAME);
  SetAppName (APPLICATION_NAME);

  appFrame = new RapidSvnFrame (APPLICATION_NAME);
  appFrame->Show (TRUE);
  SetTopWindow (appFrame);

  return TRUE;
}

int
RapidSvnApp::OnExit ()
{
  // destroy application configuration object
  delete wxConfigBase::Set ((wxConfigBase *) NULL);

  return 0;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
