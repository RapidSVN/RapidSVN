/*
 * ====================================================================
 * Copyright (c) 2002-2012 The RapidSVN Group.  All rights reserved.
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

// app
#include "action_event.hpp"
#include "ids.hpp"

struct ActionEvent::Data
{
public:
  wxWindow * parent;
  wxCommandEvent * event;
  int eventId;

  wxString msg;
  void * data;

  Data(wxWindow * parent_, int eventId_) :
    parent(parent_), eventId(eventId_)
  {
    event = new wxCommandEvent(wxEVT_COMMAND_MENU_SELECTED, ACTION_EVENT);
    event->SetInt(eventId);
  }

  ~Data()
  {
    delete event;
  }
};

ActionEvent::ActionEvent(wxWindow * parent, int eventId)
  : m(NULL)
{
  init(parent, eventId);
}

ActionEvent::ActionEvent(wxWindow * parent, int eventId, const wxString & msg)
  : m(NULL)
{
  init(parent, eventId, msg);
//  m->event->SetString (msg);
}

ActionEvent::ActionEvent(wxWindow * parent, int eventId, LogItemType type,
                         const wxString & action, const wxString & msg)
  : m(NULL)
{
  init(parent, eventId, type, action, msg);
//  m->event->SetString (msg);
}


ActionEvent::ActionEvent(wxWindow * parent, int eventId, void * data)
  : m(NULL)
{
  init(parent, eventId, data);
//  m->event->SetClientData (data);
}

ActionEvent::~ActionEvent()
{
  delete m;
}

void
ActionEvent::init(wxWindow * parent, int eventId)
{
  if (m == NULL)
  {
    m = new Data(parent, eventId);
  }
  else
  {
    m->event->SetInt(eventId);
    m->event->SetString(wxEmptyString);
    m->event->SetClientData(NULL);
  }
}

void
ActionEvent::init(wxWindow * parent, int eventId, const wxString & msg)
{
  init(parent, eventId);
  m->event->SetString(msg);
  m->event->SetClientData(NULL);
}

void
ActionEvent::init(wxWindow * parent, int eventId, LogItemType type,
                  const wxString & action, const wxString & msg)
{
  init(parent, eventId);
  wxString cat;
  cat << action << "|||" << msg;
  m->event->SetString(cat);
  m->event->SetExtraLong(type);
  m->event->SetClientData(NULL);
}

void
ActionEvent::init(wxWindow * parent, int eventId, void * data)
{
  init(parent, eventId);
  m->event->SetClientData(data);
  m->event->SetString(wxEmptyString);
}

// void
// ActionEvent::SetCode (int code)
// {
//   m->event->SetInt (code);
// }

// void
// ActionEvent::SetStr (wxString msg)
// {
//   m->event->SetString (msg);
// }

// void
// ActionEvent::SetData (void * data)
// {
//   m->event->SetClientData (data);
// }

void
ActionEvent::Post() const
{
  wxPostEvent(m->parent, *m->event);
}

void
ActionEvent::Post(wxWindow * parent, int event_id)
{
  ActionEvent event(parent, event_id);
  event.Post();
}

void
ActionEvent::Post(wxWindow * parent, int event_id, const wxString & msg)
{
  ActionEvent event(parent, event_id, msg);
  event.Post();
}

void
ActionEvent::Post(wxWindow * parent, int event_id, LogItemType type, const wxString & action, const wxString & msg)
{
  ActionEvent event(parent, event_id, type, action, msg);
  event.Post();
}

void
ActionEvent::Post(wxWindow * parent, int event_id, void * data)
{
  ActionEvent event(parent, event_id, data);
  event.Post();
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
