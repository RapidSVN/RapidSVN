/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
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
#include "wx/app.h"
#include "wx/window.h"

// app
#include "action.hpp"
#include "tracer.hpp"

Action::Action (wxWindow * parent, Tracer * tracer, bool own)
  :  m_parent (parent), m_tracer (tracer), m_ownTracer (own)
{
}

Action::~Action ()
{
  if (m_tracer && m_ownTracer)
  {
    delete m_tracer;
  }
}

void
Action::PostStringEvent (int code, wxString str, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetString (str);

  // send in a thread-safe way
  wxPostEvent (m_parent, event);
}

void
Action::PostDataEvent (int code, void *data, int event_id)
{
  wxCommandEvent event (wxEVT_COMMAND_MENU_SELECTED, event_id);
  event.SetInt (code);
  event.SetClientData (data);

  // send in a thread-safe way
  wxPostEvent (m_parent, event);
}

void 
Action::SetTracer (Tracer * t, bool own = TRUE)
{
  m_tracer = t;
  m_ownTracer = own;
}

Tracer *
Action::GetTracer ()
{
  return m_tracer;
}

wxWindow *
Action::GetParent ()
{
  return m_parent;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
