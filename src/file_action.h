#ifndef _FILE_ACTION_THREAD_H_INCLUDED_
#define _FILE_ACTION_THREAD_H_INCLUDED_

#include "tracer.h"

/**
* An action thread class is used as a base class
* for all thread classes that perform actions as checkout, 
* commit etc on a separate thread. Each of there threads 
* communicates with the main frame by sending events with 
* wxPostEvent.
*/
class FileAction
{
public:
  /**
  * Displays the UI, if necessary, and performs the action if the user accepts the UI dialog.
  */
  bool PerformAction();

protected:
  /**
   * The main frame of the application
   */
  wxFrame * mainFrame;

  /**
   * This member variable will take the address 
   * of a trace object allocated in a class 
   * derived from ActionThread. It will be used
   * from the svn_delta_editor callbacks.
   */
  Tracer *tracer;
  
  /**
   * If ownTracer is TRUE, then the ActionThread class
   * is responsible for deleting the tracer.
   */
  bool ownTracer;
  
  apr_pool_t *pool;
  apr_array_header_t *  _targets;

  FileAction (wxFrame * frame, apr_pool_t * __po0l);
  virtual ~FileAction ();

  Tracer *GetTracer ()
  {
    return tracer;
  }

  /**
   * Sets the tracer passed as an argument.
   * If own is TRUE, then the ActionThread class
   * is responsible for deleting the tracer.
   */
  void SetTracer (Tracer * t, bool own = TRUE)
  {
    tracer = t;
  }

  void PostStringEvent (int code, wxString str, int event_id);
  void PostDataEvent (int code, void *data, int event_id);

  /**
  * Abstract method that displays a UI appropriate to the action.
  */
  virtual bool PerformUI () = 0;
  /**
  * Abstract method that performs the file action.  targets will be deleted by caller.
  */
  virtual void Perform () = 0;

  friend class FileActionThreadHelper;
};

/**
* Class to perform background processing of action.
*/
class FileActionThreadHelper:public wxThread
{
public:
  FileActionThreadHelper (wxFrame * mainFrame, FileAction * action);
  virtual ~FileActionThreadHelper ();

  void* Entry();

protected:
  FileAction *  _action;
  wxFrame *     _mainFrame;
};

#endif
