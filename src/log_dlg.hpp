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

#ifndef _LOG_DLG_H_INCLUDED_
#define _LOG_DLG_H_INCLUDED_

// wxwindows
#include "wx/dialog.h"

// svncpp
#include "svncpp/client.hpp"

//forward declarations
class wxTextCtrl;
class LogList;
class wxListEvent;

class LogDlg : public wxDialog
{
public:
  /**
   * constructor. the @a entries are NOT owned by 
   * this class. 
   *
   * @param parent parent window
   * @param path path of selected item
   * @param entries log entries
   */
  LogDlg (wxWindow * parent, 
          const char * path,
          const svn::LogEntries * entries);

  /**
   * destructor
   */
  virtual ~LogDlg ();

private:
  /** hide implementation details */
  struct Data;
  Data * m;

  void OnGet (wxCommandEvent & event);
  void OnView (wxCommandEvent & event);
  void OnSelected(wxListEvent& event);
  
  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
