/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
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

#include <exception>
#include <string>
#include "svn_client.h"

namespace svn
{

/**
 * Generic exception class.
 */
class Exception : public std::exception
{
private:
  std::string m_message;

public:
  /**
   * Constructor.  Assigns the exception reason.
   */
  Exception (const std::string & message) throw ();

  ~Exception () throw ();

  /**
   * Returns the exception message.
   */
  const char * message ();

};

/**
 * Subversion client exception class.
 */
class ClientException : public Exception
{
private:
  svn_error_t * m_error;

public:
  /**
   * Constructor.  Sets the error template and an optional message.
   */
  ClientException (svn_error_t * error, 
                   const std::string message = "") throw ();

  virtual ~ClientException () throw ();

  /**
   * Returns the error message. 
   */
  const char * description ();

  /**
   * Returns the error source. 
   */
  const char * source ();

  /**
   * Returns the APR error id. 
   */
  int aprError ();
};

}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
