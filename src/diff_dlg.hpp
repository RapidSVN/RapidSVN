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

private:
  /** hide implementation details */
  struct Data;
  Data * m;

  void 
  OnBrowse (wxCommandEvent & event);

  void 
  OnText (wxCommandEvent & event);

  void 
  OnUseLatest (wxCommandEvent & event);

private:
  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
