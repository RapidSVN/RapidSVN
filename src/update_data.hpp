/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _UPDATE_DATA_H_INCLUDED_
#define _UPDATE_DATA_H_INCLUDED_

// wxWidgets
#include "wx/string.h"

struct UpdateData
{
  UpdateData()
    : revision (wxEmptyString), url (wxEmptyString), useLatest (true), recursive (true)
  {
  }

  wxString revision;
  wxString url;
  bool useLatest;
  bool recursive;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
