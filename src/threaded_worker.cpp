/*
 * ====================================================================
 * Copyright (c) 2002-2011 The RapidSVN Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.
 * If not, see <http://www.gnu.org/licenses/>.
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
#include "action_event.hpp"
#include "ids.hpp"
#include "threaded_worker.hpp"
#include "tracer.hpp"
#include "utils.hpp"

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
  Data(wxWindow * parent)
  {
    Init(parent);
  }

  /**
   * destructor
   */
  virtual ~Data()
  {
    SetContext(0, false);
  }

  /**
   * initialize values
   */
  void
  Init(wxWindow * parent)
  {
    this->parent = parent;
    context = 0;
    action = 0;
    tracer = 0;
    ownContext = false;
    state = ACTION_NONE;
    result = ACTION_NOTHING;

    //if (Create () != wxTHREAD_NO_ERROR)
    // TODO: error handling
    Create();
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
  SetContext(svn::Context * ctx, bool own)
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
    while (!TestDestroy())
    {
      ExecuteAction();

      Sleep(10);
    }
    return 0;
  }

  void
  DeleteAction()
  {
    ActionEvent::Post(parent, TOKEN_DELETE_ACTION, (void*)action);
    action = 0;
  }

  /**
   * executes the action if there is any
   */
  void
  ExecuteAction()
  {
    if (action == 0)
      return;

    ActionEvent event(parent, TOKEN_ACTION_START);

    {
      wxString msg;
      msg.Printf(_("Execute: %s"), actionName.c_str());

      event.init(parent, TOKEN_ACTION_START, msg);
      event.Post();
    }

    unsigned int actionFlags = 0;

    try
    {
      state = ACTION_RUNNING;
      if (!action->Perform())
        result = ACTION_ERROR;
      else
        result = ACTION_SUCCESS;
      actionFlags = action->GetFlags();

      state = ACTION_NONE;
    }
    catch (svn::ClientException & e)
    {
      wxString msg, errtxt(Utf8ToLocal(e.message()));
      msg.Printf(_("Error while performing action: %s"),
                 errtxt.c_str());

      event.init(parent, TOKEN_SVN_INTERNAL_ERROR, msg);
      event.Post();

      state = ACTION_NONE;
      result = ACTION_ERROR;
      DeleteAction();
      return;
    }
    catch (...)
    {
      wxString msg(_("Error while performing action."));

      event.init(parent, TOKEN_SVN_INTERNAL_ERROR, msg);
      event.Post();

      state = ACTION_NONE;
      result = ACTION_ERROR;
      DeleteAction();
      return;
    }

    event.init(parent, TOKEN_ACTION_END, (void*) new unsigned int(actionFlags));
    event.Post();
    DeleteAction();
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
  TraceError(const wxString & message)
  {
    if (tracer)
      tracer->TraceError(message);
  }
};

ThreadedWorker::ThreadedWorker(wxWindow * parent)
{
  m = new Data(parent);
}

ThreadedWorker::~ThreadedWorker()
{
  m->Delete();
}

void
ThreadedWorker::Create(wxWindow * parent)
{
  m->Init(parent);
}

ActionState
ThreadedWorker::GetState()
{
  return m->state;
}

ActionResult
ThreadedWorker::GetResult()
{
  return m->result;
}

bool
ThreadedWorker::Perform(Action * action_)
{
  // is there already a thread running?
  if (m->action != 0)
  {
    m->TraceError(_("Internal Error: There is another action running"));
    return false;
  }

  // is there a context? we need one
  if (m->context == 0)
  {
    m->TraceError(_("Internal Error: no context available"));
    return false;
  }

  action_->SetContext(m->context);
  m->context->reset();

  m->result = ACTION_NOTHING;
  m->state = ACTION_INIT;

  try
  {
    if (!action_->Prepare())
    {
      m->result = ACTION_ABORTED;
      m->state = ACTION_NONE;
      delete action_;

      return false;
    }
  }
  catch (svn::ClientException & e)
  {
    wxString msg, errtxt(Utf8ToLocal(e.message()));
    msg.Printf(_("Error while preparing action: %s"), errtxt.c_str());
    m->TraceError(msg);

    m->result = ACTION_ERROR;
    m->state = ACTION_NONE;
    delete action_;
    return false;
  }
  catch (...)
  {
    m->TraceError(_("Error while preparing action."));

    m->result = ACTION_ERROR;
    m->state = ACTION_NONE;
    delete action_;
    return false;
  }

  m->actionName = action_->GetName();
  m->action = action_;

  return true;
}

void
ThreadedWorker::SetTracer(Tracer * tracer)
{
  m->tracer = tracer;
}

void
ThreadedWorker::SetContext(svn::Context * context, bool own)
{
  m->SetContext(context, own);
}

svn::Context *
ThreadedWorker::GetContext() const
{
  return m->context;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
