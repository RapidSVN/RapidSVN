/*
 * ====================================================================
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#include "exception.h"

namespace svn
{

// Generic Exception implementation
Exception::Exception (std::string &message)  throw () : 
  _message(message)
{
}

Exception::~Exception () throw ()
{
}

const char *
Exception::message ()
{
  return _message.c_str ();
}

// ClientException implementation

ClientException::ClientException (svn_error_t * error, 
                                  std::string message) throw () : 
  Exception (message), _error(error)
{
}

ClientException::~ClientException () throw ()
{
}

const char *
ClientException::description ()
{
  return _error->message;
}

const char *
ClientException::source ()
{
  return _error->file;
}

int
ClientException::aprError ()
{
  return _error->apr_err;
}

}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
