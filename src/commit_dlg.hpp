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
#ifndef _COMMIT_DLG_H_INCLUDED_
#define _COMMIT_DLG_H_INCLUDED_

// wxWidgets
#include "wx/dialog.h"

// forward declarations
class CommitDlg : public wxDialog
{
public:
  /**
   * constructor
   *
   * @param parent parent window
   */
  CommitDlg(wxWindow* parent, bool unexpectedCommit = false);

  /**
   * destructor
   */
  virtual ~CommitDlg ();

  /**
   * @return log message
   */
  const wxString &
  GetMessage () const;

  /**
   * @return recursive setting
   */
  bool
  GetRecursive () const;

private:
  /** hide implementation details */
  struct Data;
  Data * m;

  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
