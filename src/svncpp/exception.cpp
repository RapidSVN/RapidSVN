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

#include "exception.hpp"

namespace svn
{

// Generic Exception implementation
Exception::Exception (const std::string & message)  throw () : 
  m_message(message)
{
}

Exception::~Exception () throw ()
{
}

const char *
Exception::message ()
{
  return m_message.c_str ();
}

// ClientException implementation

ClientException::ClientException (svn_error_t * error, 
                                  const std::string message) throw () : 
  Exception (message), m_error(error)
{
}

ClientException::~ClientException () throw ()
{
}

const char *
ClientException::description ()
{
  return m_error->message;
}

const char *
ClientException::source ()
{
  return m_error->file;
}

int
ClientException::aprError ()
{
  return m_error->apr_err;
}

}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
