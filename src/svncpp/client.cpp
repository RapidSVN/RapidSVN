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

// svncpp
#include "client.hpp"

namespace svn
{
  static Context anonymous ;


  Client::Client (Context * context)
  {
    setContext (context);
  }

  Client::~Client ()
  {
  }

  const Context *
  Client::getContext () const
  {
    return m_context;
  }

  void
  Client::setContext (Context * context)
  {
    if (context != 0)
    {
      m_context = context;
    }
    else
    {
      m_context = &anonymous;
    }
  }

  const char *
  Client::getLastPath ()
  {
    return m_lastPath.c_str ();
  }



}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
