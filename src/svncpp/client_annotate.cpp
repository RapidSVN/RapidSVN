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
#if defined( _MSC_VER) && _MSC_VER <= 1200
#pragma warning( disable: 4786 )// debug symbol truncated
#endif

// Subversion api
#include "svn_client.h"

// svncpp
#include "svncpp/client.hpp"


namespace svn
{
  static svn_error_t *
  annotateReceiver (void *baton,
                    apr_int64_t line_no,
                    svn_revnum_t revision,
                    const char *author,
                    const char *date,
                    const char *line,
                    apr_pool_t *pool)
  {
    AnnotatedFile * entries = (AnnotatedFile *) baton;
    entries->push_back( AnnotateLine (line_no, revision, author, date, line));

    return NULL;
  }

  AnnotatedFile *
  Client::annotate (const Path & path, 
            const Revision & revisionStart, 
            const Revision & revisionEnd) throw (ClientException)
  {
    Pool pool;
    AnnotatedFile * entries = new AnnotatedFile;
    svn_error_t *error;
    error = svn_client_blame (
      path.c_str (),
      revisionStart.revision (), 
      revisionEnd.revision (), 
      annotateReceiver,
      entries, 
      *m_context, // client ctx
      pool);

    if (error != NULL)
    {
      delete entries;
      throw ClientException (error);
    }

    return entries;
  }
}
