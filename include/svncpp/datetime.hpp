/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

#ifndef _SVNCPP_DATETIME_HPP_
#define _SVNCPP_DATETIME_HPP_ 

// subversion api
#include "svn_types.h"


namespace svn
{
  /**
   * Class that encapsulates apr_time_t.
   *
   * @see apr_time_t
   */
  class DateTime
  {
  private:
    apr_time_t m_time;

  public:

    /**
     * Default Constructor
     */
    DateTime ();

    /**
     * Constructor
     *
     * @param time number of microseconds since 00:00:00 january 1, 1970 UTC
     */
    DateTime (const apr_time_t time);

    /**
     * Copy constructor
     *
     * @param dateTime Source
     */
    DateTime (const DateTime & dateTime);

    /**
     * @param dateTime Source
     */
    const DateTime &
    operator =(const DateTime & dateTime);

    /**
     * @param dateTime Comparator
     */
    const bool
    operator ==(const DateTime & dateTime);

    /**
     * @param dateTime Comparator
     */
    const bool
    operator !=(const DateTime & dateTime);

    /**
     * @return Is a valid (non-zero) date
     */
    const bool
    IsValid () const;

    /**
     * @return APR apr_time_t
     */
    const apr_time_t
    GetAPRTimeT () const;

    /**
     * Set from date string of the form below, using apr_date_parse_rfc
     *
     * <PRE>
     *     Sun, 06 Nov 1994 08:49:37 GMT
     * </PRE>
     * 
     * @see apr_date_parse_rfc
     * @return Successfully parsed
     */
    const bool
    SetRFC822Date (const char* date);
  };
}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
