#include "include.h"
#include "file_action.h"

FileAction::FileAction (wxFrame * frame, apr_pool_t * __pool)
:mainFrame (frame), pool (__pool)
{
  tracer = NULL;
  ownTracer = FALSE;
}

FileAction::~FileAction ()
{
}

void
FileAction::PostStringEvent (int code, wxString str, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetString (str);

  // send in a thread-safe way
  wxPostEvent (mainFrame, event);

}

void
FileAction::PostDataEvent (int code, void *data, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetClientData (data);

  // send in a thread-safe way
  wxPostEvent (mainFrame, event);

}

bool
FileAction::PerformAction()
{
  bool success = PerformUI();

  //see if the user cancelled the UI
  if (!success)
  {
    //if so clean up and return
    delete this;

    return false;
  }

  //user didn't cancel UI, create a new thread helper
  FileActionThreadHelper * helper = new FileActionThreadHelper(mainFrame, this);
  
  //let it go
  helper->Create();
  helper->Run();

  //once the thread finishes, it will delete us!
  return true;
}

/**
* Create a detatched thread for the file action helper.
*/
FileActionThreadHelper::FileActionThreadHelper(wxFrame * mainFrame, FileAction * action)
:wxThread ()
{
  _action = action;
  _mainFrame = mainFrame;
  _mainFrame->SetCursor(wxCURSOR_ARROWWAIT);
}

FileActionThreadHelper::~FileActionThreadHelper ()
{
  delete _action;
  _mainFrame->SetCursor(wxCURSOR_ARROW);
}

void *
FileActionThreadHelper::Entry ()
{
  //perform the action
  _action->Perform ();

  return NULL;
}
