/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxwindows
#include "wx/wx.h"

// svncpp
#include "svncpp/context.hpp"
#include "svncpp/exception.hpp"

// app
#include "action.hpp"
#include "ids.hpp"
#include "simple_worker.hpp"
#include "tracer.hpp"

struct SimpleWorker::Data
{
public:
  wxWindow * parent;
  svn::Context * context;
  Action * action;
  Tracer * tracer;
  bool ownContext;
  ActionState state;
  ActionResult result;

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
};

SimpleWorker::SimpleWorker (wxWindow * parent)
{
  m = new Data (parent);
}

SimpleWorker::~SimpleWorker ()
{
  delete m;
}

void
SimpleWorker::Create (wxWindow * parent)
{
  m->Init (parent);
}

ActionState 
SimpleWorker::GetState ()
{
  return m->state;
}

ActionResult
SimpleWorker::GetResult ()
{
  return m->result;
}

bool
SimpleWorker::Perform (Action * action)
{
  // is there a context? we need one
  if (m->context == 0)
  {
    SetContext (new svn::Context (), true);
  }

  action->SetContext (m->context);
  m->context->reset ();

  m->result = ACTION_NOTHING;
  m->action = action;
  m->state = ACTION_INIT;

  try
  {
    if (!m->action->Prepare ())
    {
      m->result = ACTION_ABORTED;
      delete m->action;
      m->action = 0;
      m->state = ACTION_NONE;
      return true;
    }
  }
  catch (svn::ClientException & e)
  {
    wxString msg;
    msg.Printf ( _("Error while preparing action: %s"), e.message () );
    Trace (msg);
    return false;
  }
  catch (...)
  {
    Trace (_("Error while preparing action."));
    return false;
  }

  {
    wxString msg;
    msg.Printf (_("Execute: %s"), action->GetName ());
    PostStringEvent (TOKEN_ACTION_START, msg, ACTION_EVENT);
  }

  unsigned int actionFlags = 0;

  try
  {
    // this cursor stuff has to change...
    wxBusyCursor wait;
    m->state = ACTION_RUNNING;
    bool result = m->action->Perform ();
    if (!result)
    {
      m->result = ACTION_ERROR;
    }
    else
    {
      m->result = ACTION_SUCCESS;
    }
    actionFlags = m->action->GetFlags ();

    delete m->action;
    m->action = 0;
    m->state = ACTION_NONE;
  }
  catch (svn::ClientException & e)
  {
    wxString msg;
    msg.Printf (_("Error while performing action: %s"), 
                e.message ());
    PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, msg, ACTION_EVENT);

    return false;
  }
  catch (...)
  {
    wxString msg (_("Error while performing action."));
    PostStringEvent (TOKEN_SVN_INTERNAL_ERROR, msg, ACTION_EVENT);

    return false;
  }

  PostDataEvent (TOKEN_ACTION_END, (void*) new unsigned int(actionFlags), 
                 ACTION_EVENT);
  return true;
}

void
SimpleWorker::SetTracer (Tracer * tracer)
{
  m->tracer = tracer;
}

void
SimpleWorker::Trace (const wxString & message)
{
  if (m->tracer)
  {
    m->tracer->Trace (message);
  }
}

void
SimpleWorker::PostStringEvent (int code, wxString str, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetString (str);

  // send in a thread-safe way
  wxPostEvent (m->parent, event);
}

void
SimpleWorker::PostDataEvent (int code, void *data, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetClientData (data);

  // send in a thread-safe way
  wxPostEvent (m->parent, event);
}

void
SimpleWorker::SetContext (svn::Context * context, bool own)
{
  m->SetContext (context, own);
}

svn::Context * 
SimpleWorker::GetContext () const
{
  return m->context;
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
