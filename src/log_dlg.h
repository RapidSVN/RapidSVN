/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
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

#include "svncpp/log.h"

class LogList : public wxListCtrl
{
public:
  LogList (wxWindow * parent, svn::Log * log);

private:
  svn::Log * m_log;
  void OnSelected(wxListEvent& event);
  void InitializeList ();
  
  DECLARE_EVENT_TABLE ()
};

class LogDlg : public wxDialog
{
public:
  LogDlg (wxWindow * parent, svn::Log * log);
  void setLogMessage (const char * message);

private:
  svn::Log * m_log;
  LogList * m_logList;
  wxBoxSizer * m_logSizer;
  wxTextCtrl * m_logMsg;

  void InitializeData ();
  void getRevision (long revision);
  void OnClose (wxCommandEvent & event);
  void OnGet (wxCommandEvent & event);
  
  DECLARE_EVENT_TABLE ()
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
