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
#ifndef _CHECKOUT_DATA_H_INCLUDED_
#define _CHECKOUT_DATA_H_INCLUDED_

// wxwindows
#include "wx/string.h"

struct CheckoutData
{
  CheckoutData ()
  {
    UseLatest = true;
    Recursive = true;
    Bookmarks = true;
  }

  wxString RepUrl;
  wxString DestFolder;
  wxString Revision;
  bool Recursive;
  bool UseLatest;
  bool Bookmarks;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
