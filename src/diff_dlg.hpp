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
#ifndef _DIFF_DLG_H_INCLUDED_
#define _DIFF_DLG_H_INCLUDED_

// wxwindows
#include "wx/wx.h"


// forward declarations
struct DiffData;


/**
 * Dialog that lets the user enter option for
 * comparing files against different revisions
 * and/or urls
 */
class DiffDlg : public wxDialog
{
public:
  /**
   * Constructor
   *
   * @param parent parent window
   */
  DiffDlg (wxWindow *parent);

  /**
   * destructor
   */
  virtual ~DiffDlg ();


  /**
   * returns the data the user has entered
   */
  const DiffData
  GetData () const;

private:
  /** hide implementation details */
  class Data;
  Data * m;
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
