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

// stl
#include <string>

// svncpp
#include "svncpp/exception.hpp"


namespace svn
{

  struct Exception::Data
  {
  public:
    std::string message;
    apr_status_t apr_err;

    Data (const char * msg)
      : message (msg)
    {
    }
  };
   
  Exception::Exception (const char * message)  throw ()
  {
    m = new Data (message);
  }

  Exception::~Exception () throw ()
  {
    delete m;
  }

  const apr_status_t
  Exception::apr_err () const
  {
    return m->apr_err;
  }

  const char *
  Exception::message () const
  {
    return m->message.c_str ();
  }


  ClientException::ClientException (svn_error_t * error) throw () 
    : Exception ("")
  {
    if (error == 0)
      return;

    m->apr_err = error->apr_err;
    svn_error_t * next = error->child;
    std::string & message = m->message;
    message = error->message;
    while (next != 0)
    {
      message = message + "\n" + next->message;

      next = next->child;
    }
    svn_error_clear (error);
  }


  ClientException::ClientException (apr_status_t status) throw () 
    : Exception ("")
  {
    m->apr_err = status;
  }


  ClientException::~ClientException () throw ()
  {
  }

  ClientException::ClientException (const ClientException & src) throw ()
    : Exception (src.message ())
  {
  }
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
