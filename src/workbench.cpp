/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
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
#include "wx/dynarray.h"

// svncpp
#include "svncpp/context.hpp"

// app
#include "workbench.hpp"


WX_DEFINE_ARRAY (svn::Context *, ContextArray);

struct Workbench::Data 
{
public:
  svn::Context * singleContext;
  ContextArray contexts;
  wxArrayString projects;

  /**
   * constructor
   */
  Data ()
    : singleContext (0)
  {
  };

  /**
   * destructor
   */
  virtual ~Data ()
  {
    Clear ();
  }

  /**
   * remove all projects from the workbench.
   * all of the memory is released as well
   */
  void Clear ()
  {
    projects.Clear ();

    ClearContexts ();
  }

  /**
   * add a new project, but only if it doesnt
   * exist yet. Add a new context as well
   */
  void 
  AddProject (const char * name)
  {
    if (projects.Index (name) != wxNOT_FOUND)
      return;

    projects.Add (name);

    if (singleContext == 0)
      contexts.Add (new svn::Context ());
  }

  bool
  RemoveProject (const char * path)
  {
    int index = projects.Index (path);

    if (index == wxNOT_FOUND)
      return false;

    projects.RemoveAt (index);

    svn::Context * context = contexts.Item (index);
    if (context != 0)
      delete context;

    contexts.RemoveAt (index);

    return true;
  }


  void ClearContexts ()
  {
    while (contexts.Count () > 0)
    {
      svn::Context * context = contexts.Item (0);
      if (context != 0)
      {
        delete context;
      }

      contexts.RemoveAt (0);
    }
  }
};

Workbench::Workbench ()
{
  m = new Data ();
}

Workbench::~Workbench ()
{
  delete m;
}

void 
Workbench::AddProject (const char * name)
{
  m->AddProject (name);
}

const size_t
Workbench::Count () const
{
  return m->projects.Count ();
}

void
Workbench::Clear ()
{
  m->Clear ();
}

const char *
Workbench::GetProject (const size_t index) const
{
  wxString project = m->projects[index];
  return project.c_str ();
}

svn::Context *
Workbench::GetContext (const size_t index)
{
  if (m->singleContext != 0)
    return m->singleContext;

  svn::Context * context = m->contexts[index];
  return context;
}

svn::Context *
Workbench::GetContext (const char * path)
{
  int index = m->projects.Index (path);

  if (index == wxNOT_FOUND)
    return 0;

  return GetContext (index);
}

bool  
Workbench::RemoveProject (const char * path)
{
  return m->RemoveProject (path);
}

void
Workbench::SetAuthPerProject (const bool perProject)
{
  bool oldPerProject = m->singleContext != 0;

  // has anything changed?
  if (perProject == oldPerProject)
    return;

  if (!perProject)
  {
    // one Context for all
    m->ClearContexts ();
    m->singleContext = new svn::Context ();
  }
  else
  {
    // each projects has its own Context
    const size_t count = m->projects.Count ();

    if (m->singleContext != 0)
    {
      delete m->singleContext;
      m->singleContext = 0;
    }
    size_t index;
    for (index=0; index < count; index++)
    {
      m->contexts.Add (new svn::Context ());
    }
  }
}

const bool
Workbench::GetAuthPerProject () const
{
  return m->singleContext == 0;
}



/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
