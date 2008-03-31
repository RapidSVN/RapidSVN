/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
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
#ifndef _TRACER_H_INCLUDED_
#define _TRACER_H_INCLUDED_

// wxWidgets
#include "wx/string.h"

// forward declarations
class wxFrame;
class wxString;

/**
* The Tracer class is an interface used for
* traceing inside functions as svn_client_checkout,
* svn_client_copy, svn_client_import etc, in general
* for functions requiring a trace editor.
*/
class Tracer
{
public:
  virtual void Trace (const wxString &) = 0;
  virtual void TraceError (const wxString &) = 0;
  virtual ~Tracer () { }
};

/**
* A Tracer that sends events to a frame.
*/
class EventTracer : public Tracer
{
private:
  wxFrame * m_frame;

public:
  EventTracer (wxFrame * fr);

  void Trace (const wxString & msg);
  void TraceError (const wxString & msg);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
