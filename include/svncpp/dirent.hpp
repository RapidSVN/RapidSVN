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

#ifndef _SVNCPP_DIRENT_HPP_
#define _SVNCPP_DIRENT_HPP_ 

// subversion api
#include "svn_client.h"


namespace svn
{
  class DirEntry
  {
  public:
    /**
     * default constructor
     */
    DirEntry ();

    /**
     * constructor for existing @a svn_dirent_t entries
     */
    DirEntry (const char * name, svn_dirent_t * dirEntry);

    /**
     * copy constructor
     */
    DirEntry (const DirEntry & src);

    /**
     * destructor
     */
    ~DirEntry ();

    /**
     * assignment operator
     */
    DirEntry &
    operator = (const DirEntry &);

    const char *
    name () const;

    svn_node_kind_t 
    kind () const;

    svn_filesize_t
    size () const;

    bool
    hasProps () const;

    svn_revnum_t
    createdRev () const;

    apr_time_t
    time () const;

    const char *
    lastAuthor () const;


  private:
    struct Data;
    Data * m;

  };
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
