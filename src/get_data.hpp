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
#ifndef _GET_DATA_H_INCLUDED_
#define _GET_DATA_H_INCLUDED_

// wxWidgets
#include "wx/string.h"

// svncpp
#include "svncpp/revision.hpp"

/**
 * this structure is used to configure @a GetAction
 */
struct GetData
{
public:
  svn::Revision revision;
  wxString path;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
