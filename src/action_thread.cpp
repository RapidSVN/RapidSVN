#include "include.h"
#include "action_thread.h"

ActionThread::ActionThread (wxFrame * frame, apr_pool_t * __pool):wxThread ()
  // default creates the thread DETACHED
  , mainFrame (frame), pool (__pool)
{
  tracer = NULL;
  ownTracer = FALSE;
}

ActionThread::~ActionThread ()
{
  // The derived classes allocate a tracer, 
  // set ActionThread::tracer to point to it
  // and this class deletes it.
  if (tracer && ownTracer)
    delete tracer;
}

void
ActionThread::PostStringEvent (int code, wxString str, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetString (str);

  // send in a thread-safe way
  wxPostEvent (mainFrame, event);

}

void
ActionThread::PostDataEvent (int code, void *data, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetClientData (data);

  // send in a thread-safe way
  wxPostEvent (mainFrame, event);

}
