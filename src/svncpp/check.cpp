/*
 * ====================================================================
 * Copyright (c) 2002-2007 The RapidSvn Group.  All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library (in the file LGPL.txt); if not, 
 * write to the Free Software Foundation, Inc., 51 Franklin St, 
 * Fifth Floor, Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// svncpp
#include "svncpp/check.hpp"
#include "m_check.hpp"


#ifndef CHECK_SVN_VERSION
#error "CHECK_SVN_VERSION not defined"
#endif

#ifndef CHECK_SVN_SUPPORTS_LOCK
#error "CHECK_SVN_SUPPORTS_LOCK not defined"
#endif


namespace svn
{
#if CHECK_SVN_SUPPORTS_LOCK
  const bool SUPPORTS_LOCK = true;
#else
  const bool SUPPORTS_LOCK = false;
#endif

#if CHECK_SVN_SUPPORTS_PEG
  const bool SUPPORTS_PEG = true;
#else
  const bool SUPPORTS_PEG = false;
#endif

#if CHECK_SVN_SUPPORTS_EXTERNALS
  const bool SUPPORTS_EXTERNALS = true;
#else
  const bool SUPPORTS_EXTERNALS = false;
#endif
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
