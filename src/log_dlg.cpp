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
#include "get_data.hpp"
#include "ids.hpp"
#include "log_dlg.hpp"
#include "utils.hpp"

enum
{
  ID_View = 1,
  ID_Get,
  LOG_LIST,
  LOG_MSG
};

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

  /**
   * returns the selected revision.
   *
   * @return selected revision
   * @retval -1 if nothing was selected or the cell
   *            contains an invalid string 
   */
  svn_revnum_t
  GetSelectedRevision () const
  {
    long item = GetNextItem (-1, wxLIST_NEXT_ALL, 
                             wxLIST_STATE_SELECTED);
    if (item == -1)
      return -1;

    wxListItem info;
    info.m_itemId = item;
    info.m_col = 0;
    info.m_mask = wxLIST_MASK_TEXT;
  
    if (!GetItem (info))
      return -1;
    svn_revnum_t revnum = -1;
    info.m_text.ToLong (&revnum);
    
    return revnum;
  }


private:
  void OnSelected(wxListEvent& event);
  void InitializeList (const svn::LogEntries * entries)
  {
    SetSingleStyle (wxLC_REPORT);

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

struct LogDlg::Data
{
  LogList * m_logList;
  wxTextCtrl * m_logMsg;
  wxButton * m_buttonView;
  wxButton * m_buttonGet;
  wxButton * m_buttonDiff;

private:
public:
  const svn::LogEntries * entries;
  wxString path;
  wxWindow * parent;
  wxWindow * window;


  Data (wxWindow * parent_,
        wxWindow * wnd,
        const char * path_,
        const svn::LogEntries * entries_)
    : entries (entries_), path (path_), 
      parent (parent_), window (wnd)
  {
    // create controls
    wxString history;
    history.Printf(_("History: %d revisions"), entries->size ());
    wxStaticText * historyLabel = new wxStaticText (wnd, -1, history);

    m_logList = new LogList (wnd, entries);

    m_logMsg = new wxTextCtrl (wnd, LOG_MSG, "", 
                               wxDefaultPosition, wxSize (420, 110), 
                               wxTE_READONLY | wxTE_MULTILINE );

    wxButton * buttonClose = new wxButton (wnd, wxID_OK, _("Close"));
    m_buttonView = new wxButton (wnd, ID_View, _("View"));
    m_buttonGet = new wxButton (wnd, ID_Get, _("Get"));
    m_buttonDiff = new wxButton (wnd, ID_Diff, _("Diff"));

    // View/Get/Diff disabled for Alpha 3 Milestone
    m_buttonView->Enable (false);
    m_buttonGet->Enable (false);
    m_buttonDiff->Enable (false);

    // position controls

    wxBoxSizer * logSizer = new wxBoxSizer (wxVERTICAL);
    logSizer->Add (historyLabel, 0, wxALL, 5);
    logSizer->Add (m_logList, 1, wxLEFT);

    wxBoxSizer * buttonSizer = new wxBoxSizer (wxVERTICAL);
    buttonSizer->Add (buttonClose, 0, wxALL | wxEXPAND, 5);
    buttonSizer->Add (m_buttonView, 0, wxALL | wxEXPAND, 5);
    buttonSizer->Add (m_buttonGet, 0, wxALL | wxEXPAND, 5);
    buttonSizer->Add (m_buttonDiff, 0, wxALL | wxEXPAND, 5);

    wxBoxSizer * topSizer = new wxBoxSizer (wxHORIZONTAL);
    topSizer->Add (logSizer, 1, wxALL, 5);
    topSizer->Add (buttonSizer, 0, wxALL, 5);


    wxStaticBox * boxMessage = 
      new wxStaticBox(wnd, -1, _("Log Message:"));
    wxStaticBoxSizer *messageSizer = 
      new wxStaticBoxSizer (boxMessage, wxHORIZONTAL);
    messageSizer->Add (m_logMsg, 1, wxALL | wxEXPAND, 2);

    wxBoxSizer * mainSizer = new wxBoxSizer (wxVERTICAL);
    mainSizer->Add (topSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add (messageSizer, 1, wxALL | wxEXPAND, 5);

    wnd->SetAutoLayout (true);
    wnd->SetSizer (mainSizer);

    mainSizer->SetSizeHints (wnd);
    mainSizer->Fit (wnd);
  }

  /**
   * handler for the "get" button.
   * @a CheckButtons makes sure this command will
   * only be enabled if one entry in the list is
   * selected
   */ 
  void
  OnGet ()
  {
    wxBusyCursor busy;
 
    svn_revnum_t revnum = m_logList->GetSelectedRevision ();
 
    wxCommandEvent event = CreateActionEvent (TOKEN_GET);
    GetData * data = new GetData ();
    data->revision = revnum;
    data->path = path;
    event.SetClientData (data);

    wxPostEvent (parent, event);
  }

  /**
   * handle the "View" button
   */
  void
  OnView ()
  {
    wxBusyCursor busy;
 
    svn_revnum_t revnum = m_logList->GetSelectedRevision ();
 
    wxCommandEvent event = CreateActionEvent (TOKEN_VIEW);
    GetData * data = new GetData ();
    data->revision = revnum;
    data->path = path;
    event.SetClientData (data);

    wxPostEvent (parent, event);
  }
    
  void
  OnSelected (long index)
  {
    wxString message;
    const svn::LogEntry & entry = (*entries)[index];
    message = entry.message.c_str ();
    message.Trim (false);

    m_logMsg->Show (false);
    m_logMsg->SetValue (message);
    m_logMsg->Show (true);

    CheckButtons ();
  }

  void 
  CheckButtons ()
  {
    int count = m_logList->GetSelectedItemCount ();

    m_buttonGet->Enable (count == 1);
    m_buttonView->Enable (count == 1);
    m_buttonDiff->Enable (count == 2);
  }
};

BEGIN_EVENT_TABLE (LogDlg, wxDialog)
  EVT_BUTTON (ID_Get, LogDlg::OnGet)
  EVT_BUTTON (ID_View, LogDlg::OnView)
  EVT_LIST_ITEM_SELECTED (LOG_LIST, LogDlg::OnSelected)
END_EVENT_TABLE ()

LogDlg::LogDlg (wxWindow * parent,
                const char * path,
                const svn::LogEntries * entries)
  : wxDialog (parent, -1, _("Log History"), wxDefaultPosition, 
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (parent, this, path, entries);

  CentreOnParent ();
}

LogDlg::~LogDlg ()
{
  delete m;
}

void
LogDlg::OnGet (wxCommandEvent & event)
{
  m->OnGet ();
}

void
LogDlg::OnView (wxCommandEvent & event)
{
  m->OnView ();
}

void
LogDlg::OnSelected (wxListEvent& event)
{
  m->OnSelected (event.m_itemIndex);
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
