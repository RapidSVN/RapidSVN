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

// stl
#include <string>

// svncpp
#include "exception.hpp"

namespace svn
{

  struct Exception::Data
  {
  public:
    std::string message;

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

  const char *
  Exception::message () const
  {
    return m->message.c_str ();
  }


  ClientException::ClientException (svn_error_t * error) throw () : 
    Exception ("")
  {
    if (error == 0)
      return;

    svn_error_t * next = error->child;
    std::string & message = m->message;
    message = error->message;
    while (next != 0)
    {
      message = message + "\n" + next->message;

      next = next->child;
    }
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
