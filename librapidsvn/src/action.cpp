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
#include "wx/utils.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/status_selection.hpp"

// app
#include "action.hpp"
#include "config.hpp"
#include "tracer.hpp"
#include "rapidsvn_app.hpp"
#include "utils.hpp"

// subversion api
#include "svn_wc.h"

#ifdef USE_SIMPLE_WORKER
void
SafeYield()
{
  wxSafeYield();
}
#else
void
SafeYield() {};
#endif


struct Action::Data
{
public:
  /**
   * The parent
   */
  wxWindow * parent;

  /** the name of the action */
  wxString name;

  /** flags for the action */
  unsigned int flags;

  /**
   * This member variable will take the address
   * of a trace object allocated in a class
   * derived from ActionThread. It will be used
   * from the svn_delta_editor callbacks.
   */
  Tracer * tracer;

  /**
   * If ownTracer is TRUE, then the ActionThread class
   * is responsible for deleting the tracer.
   */
  bool ownTracer;

  /**
   * the context under which the action will be
   * performed. this includes auth info and callback
   * addresses
   */
  svn::Context * context;

  /**
   * the path where the action will take place
   */
  svn::Path path;


  svn::StatusSel statusSel;

  Data(wxWindow * parnt, const wxString & nam, unsigned int flgs)
    :  parent(parnt), name(nam), flags(flgs),
       tracer(0), ownTracer(false)
  {
  }

  virtual ~Data()
  {
    if (tracer && ownTracer)
    {
      delete tracer;
    }
  }

};

const unsigned int Action::DONT_UPDATE          = 0x0001;
const unsigned int Action::UPDATE_LATER         = 0x0002;
const unsigned int Action::UPDATE_TREE          = 0x0004;


Action::Action(wxWindow * parent, const wxString & name, unsigned int flgs)
{
  m = new Data(parent, name, flgs);
}

Action::~Action()
{
  delete m;
}

void
Action::SetTracer(Tracer * t, bool own)
{
  m->tracer = t;
  m->ownTracer = own;
}

Tracer *
Action::GetTracer()
{
  return m->tracer;
}

wxWindow *
Action::GetParent()
{
  return m->parent;
}

void
Action::Trace(const wxString & msg)
{
  if (m->tracer)
  {
    m->tracer->Trace(msg);
  }

  SafeYield();
}

void
Action::TraceError(const wxString & msg)
{
  if (m->tracer)
  {
    m->tracer->TraceError(msg);
  }

  SafeYield();
}

void
Action::SetParent(wxWindow * parent)
{
  m->parent = parent;
}

void
Action::SetPath(const svn::Path & path)
{
  m->path = path;
}

const svn::Path &
Action::GetPath()
{
  return m->path;
}

void
Action::SetContext(svn::Context * context)
{
  m->context = context;
}

svn::Context *
Action::GetContext()
{
  return m->context;
}

void
Action::SetStatusSel(const svn::StatusSel & statusSel)
{
  m->statusSel = statusSel;
}

const svn::StatusVector &
Action::GetStatusVector() const
{
  return m->statusSel.statusVector();
}

const svn::Targets &
Action::GetTargets() const
{
  return m->statusSel.targets();
}

bool
Action::Prepare()
{
  const wxString & dir = Utf8ToLocal(m->path.c_str());
  if (!dir.empty())
    wxSetWorkingDirectory(dir);

  return true;
}

const svn::Path
Action::GetTarget() const
{
  return m->statusSel.target();
}

const wxString &
Action::GetName() const
{
  return m->name;
}

void
Action::SetName(const wxString & name)
{
  m->name = name;
}

unsigned int
Action::GetFlags() const
{
  return m->flags;
}

svn::Path
Action::GetPathAsTempFile(const svn::Path & path,
                          const svn::Revision & revision)
{
  svn::Client client(GetContext());


  svn::Revision peg_revision;
  if (path.isUrl())
    peg_revision = svn::Revision::HEAD;
  else
    peg_revision = svn::Revision::BASE;

  wxString revStr;
  if (revision.kind() == revision.HEAD)
    revStr = _("HEAD");
  else if (revision.kind() == revision.BASE)
    revStr = _("BASE");
  else
    revStr.Printf(wxT("%") wxT(SVN_REVNUM_T_FMT), revision.revnum());

  wxString msg, wxpath(Utf8ToLocal(path.c_str()));
  msg.Printf(_("Get file %s rev. %s"),
             wxpath.c_str(), revStr.c_str());
  Trace(msg);

  svn::Path dstPath("");
  client.get(dstPath, path, revision, peg_revision);

  // Remember this temporary file so we can delete it when the application exits
  ::wxGetApp().OptionallyRegisterTempFile(
    Utf8ToLocal(dstPath.c_str()));

  return dstPath;
}

const svn::StatusSel &
Action::GetStatusSel(void) const
{
  return m->statusSel;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
