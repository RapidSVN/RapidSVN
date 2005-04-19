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
#ifndef _IMPORT_DLG_H_INCLUDED_
#define _IMPORT_DLG_H_INCLUDED_

// wxWidgets
#include "wx/dialog.h"

// forward declarations
struct ImportData;

class ImportDlg:public wxDialog
{
public:
  /**
   * Constructor
   *
   * @param parent window
   */
  ImportDlg (wxWindow *parent);

  /**
   * Destructor
   */
  virtual ~ImportDlg ();

  /**
   * return the reference to @a ImportData
   */
  ImportData &
  GetData ();

private:
  /** hide implementation details */
  struct Data;
  Data *m;

  /**
   * Event handler for OK button
   */
  void OnOk (wxCommandEvent & event);

  /**
   * Event handler for Browser button
   */
  void OnBrowse (wxCommandEvent & event);

  /**
   * Event handler for changes in text fields
   */
  void OnText (wxCommandEvent & event);

  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
