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

#include "svncpp/modify.h"
#include "include.h"
#include "rapidsvn_app.h"
#include "log_dlg.h"
#include <wx/valgen.h>

enum
{
  ID_Close = 1,
  ID_View,
  ID_Get,
  ID_Diff,
  LOG_LIST,
  LOG_MSG
};

BEGIN_EVENT_TABLE (LogDlg, wxDialog)
  EVT_BUTTON    (ID_Close,    LogDlg::OnClose)
  EVT_BUTTON    (ID_Get,    LogDlg::OnGet)
END_EVENT_TABLE ()

BEGIN_EVENT_TABLE (LogList, wxListCtrl)
  EVT_LIST_ITEM_SELECTED (LOG_LIST, LogList::OnSelected)
END_EVENT_TABLE()

LogDlg::LogDlg (wxWindow * parent, svn::Log *log)
           : wxDialog (parent, -1, "Log History", wxDefaultPosition, 
             wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
             m_log(log)
{
  InitializeData ();
  CentreOnParent ();
}

void
LogDlg::InitializeData ()
{
  wxString history;
  char count[sizeof (int) * 8 + 1];

  sprintf (count, "%ld", (long) m_log->count ());
  history = "History: ";
  history += count;
  history += " revisions";

  wxBoxSizer * mainSizer = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer * topSizer = new wxBoxSizer (wxHORIZONTAL);

  m_logSizer = new wxBoxSizer (wxVERTICAL);
  wxStaticText * historyLabel = new wxStaticText (this, -1, history);
  m_logSizer->Add (historyLabel, 0, wxALL, 5);
  m_logList = new LogList (this, m_log);
  m_logSizer->Add (m_logList, 1, wxLEFT);

  wxBoxSizer * buttonSizer = new wxBoxSizer (wxVERTICAL);
  wxButton * closeButton = new wxButton (this, ID_Close, "Close");
  wxButton * viewButton = new wxButton (this, ID_View, "View");
  wxButton * getButton = new wxButton (this, ID_Get, "Get");
  wxButton * diffButton = new wxButton (this, ID_Diff, "Diff");
  buttonSizer->Add (closeButton, 0, wxALL, 5);
  buttonSizer->Add (viewButton, 0, wxALL, 5);
  buttonSizer->Add (getButton, 0, wxALL, 5);
  buttonSizer->Add (diffButton, 0, wxALL, 5);

  topSizer->Add (m_logSizer, 1, wxALL, 5);
  topSizer->Add (buttonSizer, 0, wxALL, 5);

  m_logMsg = new wxTextCtrl (this, LOG_MSG, _T(""), 
                           wxDefaultPosition, wxSize (420, 110), 
                           wxTE_READONLY | wxTE_MULTILINE );

  mainSizer->Add (topSizer, 0, wxALL | wxEXPAND, 5);

  wxStaticBoxSizer *messageSizer = new wxStaticBoxSizer (
          new wxStaticBox(this, -1, _T("Log Message:")), 
          wxHORIZONTAL);

  messageSizer->Add (m_logMsg, 1, wxALL | wxEXPAND, 2);
  mainSizer->Add (messageSizer, 1, wxALL | wxEXPAND, 5);

  SetAutoLayout (true);
  SetSizer (mainSizer);

  mainSizer->SetSizeHints (this);
  mainSizer->Fit (this);
}

void
LogDlg::OnClose (wxCommandEvent & event)
{
  Close (TRUE);
}

void
LogDlg::OnGet (wxCommandEvent & event)
{
  int sel = m_logList->GetSelectedItemCount ();
  wxListItem info;

  if(sel == 0 || sel > 1)
  {
    wxMessageDialog dlg (this, _T ("You must select exactly one revision."),
                         _T ("Error"), wxOK);
    dlg.ShowModal ();
  }
  else
  {
    long item = -1;

    for (;;)
    {
      item = m_logList->GetNextItem (item, wxLIST_NEXT_ALL, 
                                  wxLIST_STATE_SELECTED);
      if(item == -1)
        break;

      info.m_itemId = item;
      info.m_col = 0;
      info.m_mask = wxLIST_MASK_TEXT;
  
      if(!m_logList->GetItem (info))
        return;
      getRevision (atol (info.m_text.c_str ()));
    }
  }
}

void
LogDlg::getRevision (long revision)
{
  svn::Modify modify;
  svn::Notify notify;
  modify.notification (&notify);

  try
  {
    modify.update (m_log->getLastPath (), revision, false);
    wxMessageDialog dlg (this, _T ("Revision retrieved successfully."),
                         _T ("Message"), wxOK);
    dlg.ShowModal ();
  }
  catch (svn::ClientException &e)
  {
    wxMessageDialog dlg (this, _T ("An update error occurred."),
                         _T (e.description ()), wxOK);
    dlg.ShowModal ();
  }
}

void 
LogDlg::setLogMessage (const char * message)
{
  m_logMsg->SetValue (_T(message));
}

LogList::LogList (wxWindow * parent, svn::Log *log)
           : wxListCtrl (parent, LOG_LIST, wxDefaultPosition, 
             wxSize(365, 150), wxLC_REPORT),
             m_log(log)
{
  InitializeList ();
  CentreOnParent ();
}

void
LogList::InitializeList ()
{
  int index = 0;
  char rev[sizeof (long) * 8 + 1];

  SetSingleStyle (wxLC_REPORT);

  InsertColumn (0, _T("Revision"));
  InsertColumn (1, _T("User"));
  InsertColumn (2, _T("Date"));

  SetColumnWidth (0, 65);
  SetColumnWidth (1, 100);
  SetColumnWidth (2, 200);

  while(m_log->next ())
  {
    sprintf (rev, "%ld", (long) m_log->revision ());
    InsertItem (index, _T (rev));
    SetItem (index, 1, _T (m_log->author ()));
    SetItem (index, 2, _T (m_log->formatDate (m_log->date (), 
                                             "%a %b %d %H:%M:%S %Y")));
    index++;
  }
 
}

void
LogList::OnSelected (wxListEvent& event)
{
  wxListItem info;
  wxString message;

  info.m_itemId = event.m_itemIndex;
  info.m_col = 0;
  info.m_mask = wxLIST_MASK_TEXT;
  
  if(!GetItem (info))
    return;
  long rev = atol (info.m_text.c_str ());

  m_log->beforeFirst ();
  while(m_log->next ())
  {
    if(m_log->revision () == rev)
    {
      message = m_log->message ();
      message.Trim (FALSE);
      wxGetApp ().GetAppFrame ()->getLogAction ()->setLogMessage 
               (message.c_str ());
      break;
    }
  }
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
