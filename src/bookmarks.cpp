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
#include "wx/dynarray.h"
#include "wx/filename.h"

// svncpp
#include "svncpp/context.hpp"
#include "svncpp/url.hpp"

// app
#include "bookmarks.hpp"
#include "utils.hpp"


WX_DEFINE_ARRAY (svn::Context *, ContextArray);

struct Bookmarks::Data 
{
public:
  svn::Context * singleContext;
  ContextArray contexts;
  wxArrayString bookmarks;

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
   * remove all bookmarks.
   * all of the memory is released as well
   */
  void Clear ()
  {
    bookmarks.Clear ();

    ClearContexts ();
  }

  /**
   * add a new bookmark, but only if it doesnt
   * exist yet. Add a new context as well.
   *
   * @param name full path/url of the bookmark
   */
  void 
  AddBookmark (const char * name_)
  {
    wxString name (name_);

    TrimString (name);

    if (!svn::Url::isValid (name_))
    {
      wxFileName filename (name_);
      name = filename.GetFullPath (wxPATH_NATIVE);
    }

    name = BeautifyPath (name);

    if (bookmarks.Index (name) != wxNOT_FOUND)
      return;

    bookmarks.Add (name);
    if (singleContext == 0)
      contexts.Add (CreateContext ());
  }

  bool
  RemoveBookmark (const char * path)
  {
    int index = bookmarks.Index (path);

    if (index == wxNOT_FOUND)
      return false;

    bookmarks.RemoveAt (index);

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

  /**
   * factory method to create a new context 
   */
  svn::Context *
  CreateContext ()
  {
    svn::Context * context = new svn::Context ();
    
    // disable authentication caching.
    context->setAuthCache(false);

    return context;
  }
};

Bookmarks::Bookmarks ()
{
  m = new Data ();
}

Bookmarks::~Bookmarks ()
{
  delete m;
}

void 
Bookmarks::AddBookmark (const char * name)
{
  m->AddBookmark (name);
}

const size_t
Bookmarks::Count () const
{
  return m->bookmarks.Count ();
}

void
Bookmarks::Clear ()
{
  m->Clear ();
}

const char *
Bookmarks::GetBookmark (const size_t index) const
{
  wxString bookmark = m->bookmarks[index];
  return bookmark.c_str ();
}

svn::Context *
Bookmarks::GetContext (const size_t index)
{
  if (m->singleContext != 0)
    return m->singleContext;

  svn::Context * context = m->contexts[index];
  return context;
}

svn::Context *
Bookmarks::GetContext (const char * path)
{
  int index = m->bookmarks.Index (path);

  if (index == wxNOT_FOUND)
    return 0;

  return GetContext (index);
}

bool  
Bookmarks::RemoveBookmark (const char * path)
{
  return m->RemoveBookmark (path);
}

void
Bookmarks::SetAuthPerBookmark (const bool perBookmark)
{
  bool oldPerBookmark = m->singleContext != 0;

  // has anything changed?
  if (perBookmark == oldPerBookmark)
    return;

  if (!perBookmark)
  {
    // one Context for all
    m->ClearContexts ();
    m->singleContext = m->CreateContext ();
  }
  else
  {
    // each bookmarks has its own Context
    const size_t count = m->bookmarks.Count ();

    if (m->singleContext != 0)
    {
      delete m->singleContext;
      m->singleContext = 0;
    }
    size_t index;
    for (index=0; index < count; index++)
    {
      m->contexts.Add (m->CreateContext ());
    }
  }
}

const bool
Bookmarks::GetAuthPerBookmark () const
{
  return m->singleContext == 0;
}



/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
