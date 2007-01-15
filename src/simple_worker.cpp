/*
 * ====================================================================
 * Copyright (c) 2002-2007 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
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
#include "simple_worker.hpp"
#include "tracer.hpp"
#include "utils.hpp"

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
      return false;
    }
  }
  catch (svn::ClientException & e)
  {
    wxString msg, errtxt (Utf8ToLocal (e.message ()));
    msg.Printf ( _("Error while preparing action: %s"), errtxt.c_str () );
    TraceError (msg);
    return false;
  }
  catch (...)
  {
    TraceError (_("Error while preparing action."));
    return false;
  }

  ActionEvent event (m->parent, TOKEN_ACTION_START);

  {
    wxString msg;
    msg.Printf (_("Execute: %s"), action->GetName ().c_str ());

    event.init (m->parent, TOKEN_ACTION_START, msg);
    event.Post ();
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
    wxString msg, errtxt (Utf8ToLocal (e.message ()));
    msg.Printf (_("Error while performing action: %s"),
                errtxt.c_str ());

    event.init (m->parent, TOKEN_SVN_INTERNAL_ERROR, msg);
    event.Post ();

    return false;
  }
  catch (...)
  {
    wxString msg (_("Error while performing action."));

    event.init (m->parent, TOKEN_SVN_INTERNAL_ERROR, msg);
    event.Post ();

    return false;
  }

  event.init (m->parent, TOKEN_ACTION_END, (void*) new unsigned int(actionFlags));
  event.Post ();

  return true;
}

void
SimpleWorker::SetTracer (Tracer * tracer)
{
  m->tracer = tracer;
}

void
SimpleWorker::TraceError (const wxString & message)
{
  if (m->tracer)
  {
    m->tracer->TraceError (message);
  }
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
