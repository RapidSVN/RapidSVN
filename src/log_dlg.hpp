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
   * constructor. the entries are owned and deleted by 
   * this class
   *
   * @param parent parent window
   * @param entries log entries
   */
  LogDlg (wxWindow * parent, const svn::LogEntries * entries);

  virtual ~LogDlg ();

  void setLogMessage (const char * message);

private:
  const svn::LogEntries * m_entries;
  LogList * m_logList;
  wxTextCtrl * m_logMsg;

  void InitializeData ();
  void GetRevision (const svn_revnum_t revision);
  void OnClose (wxCommandEvent & event);
  void OnGet (wxCommandEvent & event);
  void OnSelected(wxListEvent& event);
  
  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
