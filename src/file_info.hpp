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
#ifndef _FILE_INFO_H_INCLUDED_
#define _FILE_INFO_H_INCLUDED_

// forward declarations
namespace svn
{
  class Context;
}

class FileInfo
{
public:
  /**
   * constructor.
   *
   * @param context context to use
   */
  FileInfo (svn::Context * context);

  /**
   * destructor
   */
  virtual ~FileInfo ();

  /**
   * adds a path to the list of paths that are to be examined
   *
   * @param path absolute path to add
   */
  void
  addPath (const char * path);

  /**
   * get an info string that contains informations about
   * the selection of files
   *
   * @return informational string
   */
  const char *
  info () const;

private:
  // hide implementation details
  struct Data;
  Data * m;
};


#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
