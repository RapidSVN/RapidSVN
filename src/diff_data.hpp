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
#ifndef _DIFF_DATA_H_INCLUDED
#define _DIFF_DATA_H_INCLUDED

// wx
#include "wx/string.h"

struct DiffData
{
public:
  struct Revision
  {
    bool useRevnum;
    wxString revnum;
    wxString date;
    bool useUrl;
    wxString url;

    Revision ()
    {
      Init ();
    }

    
    void
    Init ()
    {
      useRevnum = true;
      revnum = "";
      date = "";
      useUrl = false;
      url = "";
    }

  };


  typedef enum
  {
    CMP_WC_WITH_SAME_REV,
    CMP_WC_WITH_DIFFERENT_REV,
    CMP_TWO_REV
  } CompareType;

  CompareType compareType;
  Revision revision1;
  Revision revision2;

  /** Constructor */
  DiffData ()
    : compareType (CMP_WC_WITH_SAME_REV)
  {
  }
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
