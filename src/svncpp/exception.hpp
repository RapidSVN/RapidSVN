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

#ifndef _SVNCPP_EXCEPTION_H_
#define _SVNCPP_EXCEPTION_H_

// subversion api
#include "svn_client.h"

namespace svn
{

  /**
   * Generic exception class.
   */
  class Exception
  {
  public:
    /**
     * Constructor.  Assigns the exception reason.
     */
    Exception (const char * message) throw ();

    ~Exception () throw ();

    /**
     * @return the exception message.
     */
    const char * message () const;

    /**
     * @return the outermost error code.
     */
    const apr_status_t apr_err () const;

  protected:
    struct Data;
    Data * m;

  private:

    Exception (const Exception &) throw ();

    Exception () throw ();
    
    Exception & operator = (const Exception &);
  };

  /**
   * Subversion client exception class.
   */
  class ClientException : public Exception
  {
  public:
    /**
     * Constructor.  Sets the error template and an optional message.
     */
    ClientException (svn_error_t * error) throw ();

    /** 
     * Copy constructor
     */
    ClientException (const ClientException & src) throw ();

    virtual ~ClientException () throw ();

  private:
    ClientException () throw ();

    ClientException & operator = (ClientException &);
  };

}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
