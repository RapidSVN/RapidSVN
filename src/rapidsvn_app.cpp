#include "include.h"
#include "rapidsvn_app.h"

IMPLEMENT_APP (VsvnApp)
     bool VsvnApp::OnInit ()
{
  // application and vendor name are used by wxConfig to construct the name
  // of the config file/registry key and must be set before the first call
  // to Get() if you want to override the default values (the application
  // name is the name of the executable and the vendor name is the same)
  SetVendorName (APPLICATION_NAME);
  SetAppName (_T ("RapidSVN"));

  appFrame = new VSvnFrame (APPLICATION_NAME);
  appFrame->Show (TRUE);
  SetTopWindow (appFrame);

  return TRUE;
}

int
VsvnApp::OnExit ()
{
  // destroy application configuration object
  delete wxConfigBase::Set ((wxConfigBase *) NULL);

  return 0;
}
