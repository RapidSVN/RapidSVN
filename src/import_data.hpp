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
#ifndef _IMPORT_DATA_H_INCLUDED_
#define _IMPORT_DATA_H_INCLUDED_

// wxWidgets
#include "wx/string.h"

/**
 * this structure is used to configure @a ImportAction
 */
struct ImportData
{
  wxString Repository;
  wxString Path;
  wxString LogMessage;
  bool Recursive;
  bool TreeType;
  bool FileType;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
