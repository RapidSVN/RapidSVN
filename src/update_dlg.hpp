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
#ifndef _UPDATE_DLG_H_INCLUDED_
#define _UPDATE_DLG_H_INCLUDED_

// wxwindows
#include "wx/dialog.h"

// forward declarations
struct UpdateData;

class UpdateDlg:public wxDialog
{
public:
  /** show dialog without "recursiv" checkbox */
  static const int WITHOUT_RECURSIVE;

  /** show dialog with URL line */
  static const int WITH_URL;

  /** show dialog without revision information */
  static const int WITHOUT_REVISION;

  /**
   * constructor
   *
   * @param parent parent window
   * @param flags flags for the window (@a WITH_URL)
   * @param recursive default flag for the "recursive" check
   */
  UpdateDlg (wxWindow* parent, 
             const char * title,
             int flags = 0,
             bool recursive = true);

  /**
   * destructor
   */
  virtual ~UpdateDlg ();
  
  UpdateData &
  GetData ();

  void InitDialog ();
private:
  struct Data;
  Data * m;

  void 
  OnUseLatest(wxCommandEvent &event);

  void 
  OnText (wxCommandEvent & event);
  
  DECLARE_EVENT_TABLE()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
