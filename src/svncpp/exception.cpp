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

  struct Exception::Data
  {
  public:
    Data (const std::string & message)
      : m_message (message)
    {
    }

    const char *
    message ()
    {
      return m_message.c_str ();
    }

  private:
    std::string m_message;
  };
   
  Exception::Exception (const std::string & message)  throw ()
  {
    m = new Data (message);
  }

  Exception::~Exception () throw ()
  {
    delete m;
  }

  const char *
  Exception::message ()
  {
    return m->message ();
  }

  struct ClientException::Data
  {
  public:
    std::vector<std::string> messages;
    apr_status_t apr_err;

    Data (svn_error_t * error, const std::string message)
    {
      messages.push_back (message);

      if (error == 0)
      {
        return;
      }

      apr_err = error->apr_err;

      // Now add the whole stack of messages
      svn_error_t * next_error = error;
      while (next_error != 0)
      {
        const char * msg = next_error->message;
        if (msg != 0)
        {
          messages.push_back (msg);
        }

        next_error = next_error->child;
      }
    }

  };

  ClientException::ClientException (svn_error_t * error, 
                                    const std::string message) throw () : 
    Exception (message)
  {
    m = new Data (error, message);
  }

  ClientException::~ClientException () throw ()
  {
    delete m;
  }

  int
  ClientException::aprError ()
  {
    return m->apr_err;
  }

  const std::vector<std::string> & 
  ClientException::messages () const
  {
    return m->messages;
  }
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
