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

// wx windows
#include "wx/wx.h"
#include "wx/valgen.h"
#include "wx/listctrl.h"

// app
#include "log_dlg.hpp"

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
  EVT_BUTTON             (ID_Close, LogDlg::OnClose)
  EVT_BUTTON             (ID_Get,   LogDlg::OnGet)
  EVT_LIST_ITEM_SELECTED (LOG_LIST, LogDlg::OnSelected)
END_EVENT_TABLE ()

class LogList : public wxListCtrl
{
public:
  LogList (wxWindow * parent, const svn::LogEntries * entries)
    : wxListCtrl (parent, LOG_LIST, wxDefaultPosition, 
                  wxSize(365, 150), wxLC_REPORT)
  {
    InitializeList (entries);
    CentreOnParent ();
  }

  virtual ~LogList ()
  {
    DeleteAllItems ();
  }


private:
  //const svn::LogEntries * m_entries;

  void OnSelected(wxListEvent& event);
  void InitializeList (const svn::LogEntries * entries)
  {
    SetSingleStyle (wxLC_REPORT);
    //const char * dateFormat = "%a %b %d %H:%M:%S %Y";

    InsertColumn (0, _("Revision"));
    InsertColumn (1, _("User"));
    InsertColumn (2, _("Date"));

    SetColumnWidth (0, 65);
    SetColumnWidth (1, 100);
    SetColumnWidth (2, 200);

    if (entries == 0)
      return;

    long index=0;
    svn::LogEntries::const_iterator it;
    for (it=entries->begin (); it != entries->end (); it++ )
    {
      const svn::LogEntry & entry = *it;
      wxString rev;
      rev.Printf("%ld", (long) entry.revision);
      //TODO perform date formatting. but dont use
      //subversion but wxDateTime
      //wxString date (entry.date.c_str ());
      InsertItem (index, rev);
      SetItem (index, 1, entry.author.c_str ());
      SetItem (index, 2, entry.date.c_str ());
      index++;
    }
 
  }
  
};

LogDlg::LogDlg (wxWindow * parent, const svn::LogEntries * entries)
  : wxDialog (parent, -1, _("Log History"), wxDefaultPosition, 
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
    m_entries(entries), m_logList (0)
{
  InitializeData ();
  CentreOnParent ();
}

LogDlg::~LogDlg ()
{
}

void
LogDlg::InitializeData ()
{
  // create controls
  wxString history;
  history.Printf(_("History: %d revisions"), m_entries->size ());
  wxStaticText * historyLabel = new wxStaticText (this, -1, history);

  m_logList = new LogList (this, m_entries);

  m_logMsg = new wxTextCtrl (this, LOG_MSG, "", 
                           wxDefaultPosition, wxSize (420, 110), 
                           wxTE_READONLY | wxTE_MULTILINE );

  wxButton * closeButton = new wxButton (this, ID_Close, _("Close"));
  wxButton * viewButton = new wxButton (this, ID_View, _("View"));
  wxButton * getButton = new wxButton (this, ID_Get, _("Get"));
  wxButton * diffButton = new wxButton (this, ID_Diff, _("Diff"));

  // View/Get/Diff disabled for Alpha 3 Milestone
  viewButton->Enable (false);
  getButton->Enable (false);
  diffButton->Enable (false);

  // position controls

  wxBoxSizer * logSizer = new wxBoxSizer (wxVERTICAL);
  logSizer->Add (historyLabel, 0, wxALL, 5);
  logSizer->Add (m_logList, 1, wxLEFT);

  wxBoxSizer * buttonSizer = new wxBoxSizer (wxVERTICAL);
  buttonSizer->Add (closeButton, 0, wxALL, 5);
  buttonSizer->Add (viewButton, 0, wxALL, 5);
  buttonSizer->Add (getButton, 0, wxALL, 5);
  buttonSizer->Add (diffButton, 0, wxALL, 5);

  wxBoxSizer * topSizer = new wxBoxSizer (wxHORIZONTAL);
  topSizer->Add (logSizer, 1, wxALL, 5);
  topSizer->Add (buttonSizer, 0, wxALL, 5);



  wxStaticBoxSizer *messageSizer = new wxStaticBoxSizer (
          new wxStaticBox(this, -1, _("Log Message:")), 
          wxHORIZONTAL);
  messageSizer->Add (m_logMsg, 1, wxALL | wxEXPAND, 2);

  wxBoxSizer * mainSizer = new wxBoxSizer (wxVERTICAL);
  mainSizer->Add (topSizer, 0, wxALL | wxEXPAND, 5);
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
    wxMessageDialog dlg (this, _("You must select exactly one revision."),
                         _("Error"), wxOK);
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
      svn_revnum_t revnum;
      info.m_text.ToLong (&revnum);
      GetRevision (revnum);
    }
  }
}


void
LogDlg::GetRevision (const svn_revnum_t revision)
{
//TODO this has to be checked. Is there duplication of functionality underway?
//   svn::Modify modify;
//   svn::Notify notify;
//   modify.notification (&notify);

//   try
//   {
//     modify.update (m_log->getLastPath (), svn::Revision(revision), false);
//     wxMessageDialog dlg (this, _("Revision retrieved successfully."),
//                          _("Message"), wxOK);
//     dlg.ShowModal ();
//   }
//   catch (svn::ClientException &e)
//   {
//     wxMessageDialog dlg (this, _("An update error occurred."),
//                          _(e.description ()), wxOK);
//     dlg.ShowModal ();
//   }
}

void
LogDlg::OnSelected (wxListEvent& event)
{
  wxString message;
  const svn::LogEntry & entry = (*m_entries)[event.m_itemIndex];
  message = entry.message.c_str ();
  message.Trim (false);
  m_logMsg->SetValue (message);
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
