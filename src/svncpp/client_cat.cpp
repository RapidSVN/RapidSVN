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

// Subversion api
#include "svn_client.h"
//#include "svn_io.h"

// svncpp
#include "client.hpp"
#include "exception.hpp"
#include "pool.hpp"
#include "status.hpp"

namespace svn
{
  std::string
  Client::cat (const Path & path, const Revision & revision)
  {
    Pool pool;
    
    svn_stringbuf_t * stringbuf = svn_stringbuf_create ("", pool);
    svn_stream_t * stream = svn_stream_from_stringbuf (stringbuf, pool);

    svn_error_t * error;
    error = svn_client_cat (stream, path.c_str (),
                            revision.revision (), 
                            *m_context, 
                            pool);

    return stringbuf->data;
  }

}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
