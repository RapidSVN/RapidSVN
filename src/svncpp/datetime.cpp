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

// apr
#include "apr_date.h"

// svncpp
#include "svncpp/datetime.hpp"


namespace svn
{
  DateTime::DateTime ()
  : m_time(APR_DATE_BAD)
  {
  }

  DateTime::DateTime (const apr_time_t time)
  : m_time(time)
  {
  }

  DateTime::DateTime (const DateTime & dateTime)
  : m_time(dateTime.m_time)
  {
  }

  const DateTime &
  DateTime::operator =(const DateTime & dateTime)
  {
    m_time = dateTime.m_time;
    return *this;
  }

  const bool
  DateTime::operator ==(const DateTime & dateTime)
  {
    return m_time == dateTime.m_time;
  }

  const bool
  DateTime::operator !=(const DateTime & dateTime)
  {
    return m_time != dateTime.m_time;
  }

  const bool
  DateTime::IsValid () const
  {
    return m_time != APR_DATE_BAD;
  }

  const apr_time_t
  DateTime::GetAPRTimeT () const
  {
    return m_time;
  }

  const bool
  DateTime::SetRFC822Date (const char* date)
  {
    m_time = apr_date_parse_rfc(date);
    return IsValid();
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
