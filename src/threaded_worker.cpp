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

// wxWidgets
#include "wx/wx.h"

// svncpp
#include "svncpp/context.hpp"
#include "svncpp/exception.hpp"

// app
#include "action.hpp"
#include "ids.hpp"
#include "threaded_worker.hpp"
#include "tracer.hpp"


class ThreadedWorker::Data : public wxThread
{
public:
  wxWindow * parent;
  svn::Context * context;
  Action * action;
  Tracer * tracer;
  bool ownContext;
  ActionState state;
  ActionResult result;
  wxString actionName;


  /**
   * constructor
   */
  Data (wxWindow * parent)
  {
    Init (parent);
  }


  /**
   * destructor
   */
  virtual ~Data ()
  {
    SetContext (0, false);
  }


  /**
   * initialize values
   */
  void
  Init (wxWindow * parent)
  {
    this->parent = parent;
    context = 0;
    action = 0;
    tracer = 0;
    ownContext =false;
    state = ACTION_NONE;
    result = ACTION_NOTHING;

    //if (Create () != wxTHREAD_NO_ERROR)
    // TODO: error handling
    Create ();
    Run();
  }


  /**
   * set a context. if there is already a context and we
   * own this context, delete it
   *
   * @param ctx new context
   * @param own own new context
   */
  void
  SetContext (svn::Context * ctx, bool own)
  {
    if (own && (ownContext != 0))
    {
      delete context;
      context = 0;
    }

    context = ctx;
    ownContext = own;
  }


  /**
   * thread execution starts here
   */
  virtual void *
  Entry()
  {
    while (!TestDestroy ())
    {
        ExecuteAction ();

        Sleep (0);
    }

    return 0;
  }


  void
  DeleteAction ()
  {
    PostDataEvent (TOKEN_DELETE_ACTION, (void*)action);
    action = 0;
  }


  /**
   * executes the action if there is any
   */
  void
  ExecuteAction ()
  {
    if (action == 0)
      return;

    {
      wxString msg;
      msg.Printf (_("Execute: %s"), actionName.c_str ());
      PostStringEvent (TOKEN_ACTION_START, msg);
    }

    unsigned int actionFlags = 0;

    try
    {
      // this cursor stuff has to change...
      wxBusyCursor wait;
      state = ACTION_RUNNING;
      if (!action->Perform ())
        result = ACTION_ERROR;
      else
        result = ACTION_SUCCESS;
      actionFlags = action->GetFlags ();

      state = ACTION_NONE;
    }
    catch (svn::ClientException & e)
    {
      wxString msg, errtxt (Utf8ToLocal (e.message ()));
      msg.Printf (_("Error while performing action: %s"),
                  errtxt.c_str ());
      PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, msg);

      state = ACTION_NONE;
      result= ACTION_ERROR;
      DeleteAction ();
      return;
    }
    catch (...)
    {
      wxString msg (_("Error while performing action."));
      PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, msg);

      state = ACTION_NONE;
      result= ACTION_ERROR;
      DeleteAction ();
      return;
    }

    PostDataEvent (TOKEN_ACTION_END, (void*) new unsigned int(actionFlags));
    DeleteAction ();
  }


  /**
   * called when the thread exits - whether it terminates normally or is
   * stopped with Delete() (but not when it is Kill()ed!)
   */
  virtual void
  OnExit()
  {
  }


  void
  PostStringEvent (int code, wxString str)
  {
    wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, ACTION_EVENT);
    event.SetInt (code);
    event.SetString (str);

    // send in a thread-safe way
    wxPostEvent (parent, event);
  }


  void
  PostDataEvent (int code, void *data)
  {
    wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, ACTION_EVENT);
    event.SetInt (code);
    event.SetClientData (data);

    // send in a thread-safe way
    wxPostEvent (parent, event);
  }


  void
  Trace (const wxString & message)
  {
    if (tracer)
      tracer->Trace (message);
  }

};


ThreadedWorker::ThreadedWorker (wxWindow * parent)
{
  m = new Data (parent);
}

ThreadedWorker::~ThreadedWorker ()
{
  m->Delete ();
}

void
ThreadedWorker::Create (wxWindow * parent)
{
  m->Init (parent);
}

ActionState
ThreadedWorker::GetState ()
{
  return m->state;
}

ActionResult
ThreadedWorker::GetResult ()
{
  return m->result;
}

bool
ThreadedWorker::Perform (Action * action_)
{
  // is there already a thread running?
  if (m->action != 0)
  {
    m->Trace (_("Internal Error: There is another action running"));
    return false;
  }

  // is there a context? we need one
  if (m->context == 0)
  {
    m->Trace (_("Internal Error: no context available"));
    return false;
  }

  action_->SetContext (m->context);
  m->context->reset ();

  m->result = ACTION_NOTHING;
  m->state = ACTION_INIT;

  try
  {
    if (!action_->Prepare ())
    {
      m->result = ACTION_ABORTED;
      m->state = ACTION_NONE;
      delete action_;

      return true;
    }
  }
  catch (svn::ClientException & e)
  {
    wxString msg, errtxt (Utf8ToLocal (e.message ()));
    msg.Printf ( _("Error while preparing action: %s"), errtxt.c_str () );
    m->Trace (msg);

    m->result = ACTION_ERROR;
    m->state = ACTION_NONE;
    delete action_;
    return false;
  }
  catch (...)
  {
    m->Trace (_("Error while preparing action."));

    m->result = ACTION_ERROR;
    m->state = ACTION_NONE;
    delete action_;
    return false;
  }

  m->actionName = action_->GetName ();
  m->action = action_;
  return true;
}

void
ThreadedWorker::SetTracer (Tracer * tracer)
{
  m->tracer = tracer;
}

void
ThreadedWorker::SetContext (svn::Context * context, bool own)
{
  m->SetContext (context, own);
}

svn::Context *
ThreadedWorker::GetContext () const
{
  return m->context;
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
