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
#ifndef _BOOKMARKS_H_
#define _BOOKMARKS_H_

// forward declarations
class Bookmark;
namespace svn
{
  class Context;
};

class Bookmarks
{
public:
  /**
   * default constructor
   */
  Bookmarks ();

  /**
   * destructor
   */
  virtual ~Bookmarks ();

  /**
   * add a bookmark to the bookmarks. The list
   * of bookmarks is unique. So if the bookmark
   * already exists nothing happens
   */
  void
  AddBookmark (const wxString & name);

  /**
   * return the number of bookmarks in the
   * bookmarks
   *
   * @return number of bookmarks
   */
  const size_t
  Count () const;

  /**
   * return the bookmark at the @a index
   *
   * @param index 0 based index
   * @return Bookmark name at this index
   */
  const wxString &
  GetBookmark (const size_t index) const;

  /**
   * returns the authentication context
   * of the bookmark at @a index.
   *
   * @param index 0-based index
   * @return authentication context
   */
  svn::Context *
  GetContext (const size_t index);

  /**
   * returns the authentication context
   * for the given @a path
   *
   * @param path to the bookmark
   * @return authentication context
   */
  svn::Context *
  GetContext (const wxString & path);

  /**
   * remove all bookmarks
   */
  void
  Clear ();

  /**
   * remove bookmark with given @a path
   *
   * @param path bookmark path
   * @retval true success
   */
  bool
  RemoveBookmark (const wxString & path);

  /**
   * setting whether the authenticaion will
   * be remembered for each bookmark
   */
  void
  SetAuthPerBookmark (const bool value);

  const bool
  GetAuthPerBookmark () const;

private:
  /**
   * hide implementation details
   */
  struct Data;
  Data * m;

  /** 
   * disallow copy constructor
   */
  Bookmarks (const Bookmarks &);

  /**
   * disallow assignment operator
   */
  const Bookmarks &
  operator = (const Bookmarks &);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
