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
#ifndef _COMMIT_DLG_H_INCLUDED_
#define _COMMIT_DLG_H_INCLUDED_

// wx
#include "wx/dialog.h"

// app
#include "commit_data.hpp"

// forward declarations
class CommitDlg : public wxDialog
{
public:
  /**
   * constructor
   *
   * @param parent parent window
   */
  CommitDlg(wxWindow* parent);

  /**
   * @return data
   */
  const CommitData &
  GetData () const;

private:
  void InitializeData ();
  
  CommitData m_data;
  
  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
