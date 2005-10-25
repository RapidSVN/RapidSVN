/*
 * ====================================================================
 * Copyright (c) 2002-2005 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _DIFF_DATA_H_INCLUDED
#define _DIFF_DATA_H_INCLUDED

// wx
#include "wx/string.h"

// svncpp
#include "svncpp/revision.hpp"

struct DiffData
{
public:

  typedef enum
  {
    INVALID_COMPARE_TYPE = 0,
    WITH_BASE,
    WITH_HEAD,
    WITH_DIFFERENT_REVISION,
    TWO_REVISIONS,
    COMPARE_TYPE_COUNT
  } CompareType;

  CompareType compareType;

  bool useUrl1;
  wxString url1;

  bool useUrl2;
  wxString url2;

  svn::Revision revision1;
  svn::Revision revision2;

  /** Constructor */
  DiffData (svn_opt_revision_kind kind = svn::Revision::BASE)
   : compareType (WITH_BASE),
     useUrl1 (false), url1 (wxEmptyString),
     useUrl2 (false), url2 (wxEmptyString),
     revision1 (kind)
  {
    if (kind != svn::Revision::BASE) 
      compareType = WITH_DIFFERENT_REVISION;
  }
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
