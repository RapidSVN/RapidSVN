/*
 * ====================================================================
 * Copyright (c) 2002-2006 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
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
#include "action_event.hpp"
#include "diff_data.hpp"
#include "get_data.hpp"
#include "ids.hpp"
#include "log_dlg.hpp"
#include "merge_dlg.hpp"
#include "utils.hpp"
#include "annotate_data.hpp"

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
                  wxSize (365, 150), wxLC_REPORT)
  {
    InitializeList (entries);
    CentreOnParent ();
  }

  virtual ~LogList ()
  {
    DeleteAllItems ();
  }

  /**
   * Returns the revision for the given @a item
   *
   * @param item
   * @return revnum
   * @retval -1 not found/error
   */
  svn_revnum_t
  GetRevisionForItem (long item) const
  {
    wxListItem info;
    info.m_itemId = item;
    info.m_col = 0;
    info.m_mask = wxLIST_MASK_TEXT;
  
    if (!GetItem (info))
      return -1;

    svn_revnum_t revnum=-1;
    info.m_text.ToLong (&revnum);
    return revnum;
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

    return GetRevisionForItem (item);
  }

  /**
   * returns the selected revisions.
   * Like @a GetSelectedRevision, but can return 
   * more revisions at once.
   *
   * @return if nothing is selected, an empty array
   *         will be returned
   */
  RevnumArray
  GetSelectedRevisions () const
  {
    RevnumArray array;
    long item = -1;

    do
    {
      item = GetNextItem (item, wxLIST_NEXT_ALL,
                               wxLIST_STATE_SELECTED);
      if (item != -1)
      {
        svn_revnum_t revnum (GetRevisionForItem (item));

        array.Add (revnum);
      }
    } 
    while (item != -1);

    return array;
  }

private:
  void OnSelected (wxListEvent& event);

  void InitializeList (const svn::LogEntries * entries)
  {
    SetSingleStyle (wxLC_REPORT);

    InsertColumn (0, _("Revision"));
    InsertColumn (1, _("User"));
    InsertColumn (2, _("Date"));
    InsertColumn (3, _("Log Message"));

    SetColumnWidth (0, 65);
    SetColumnWidth (1, 100);
    SetColumnWidth (2, 150);
    SetColumnWidth (3, 200);

    if (entries == 0)
      return;

    long index=0;
    svn::LogEntries::const_iterator it;
    for (it=entries->begin (); it != entries->end (); it++ )
    {
      const svn::LogEntry & entry = *it;
      wxString rev;
      wxString dateStr (FormatDateTime (entry.date));

      rev.Printf (wxT("%ld"), (long) entry.revision);

      //TODO perform date formatting. but dont use
      //subversion but wxDateTime
      //wxString date (entry.date.c_str ());
      InsertItem (index, rev);
      SetItem (index, 1, Utf8ToLocal (entry.author.c_str ()));
      SetItem (index, 2, dateStr);
      SetItem (index, 3, Utf8ToLocal (entry.message.c_str ()));
      index++;
    }
  }
};

struct LogDlg::Data
{
private:
  LogList * m_logList;
  wxTextCtrl * m_logMsg;
  wxButton * m_buttonView;
  wxButton * m_buttonGet;
  wxButton * m_buttonDiff;
  wxButton * m_buttonMerge;
  wxButton * m_buttonAnnotate;

public:
  const svn::LogEntries * entries;
  wxString path;
  wxWindow * parent;
  wxWindow * window;

  Data (wxWindow * parent_,
        wxWindow * wnd,
        const char * path_,
        const svn::LogEntries * entries_)
    : entries (entries_), path (Utf8ToLocal (path_)), 
      parent (parent_), window (wnd)
  {
    // create controls
    wxString history;
    history.Printf (_("History: %d revisions"), entries->size ());
    wxStaticText * historyLabel = new wxStaticText (wnd, -1, history);

    m_logList = new LogList (wnd, entries);

    m_logMsg = new wxTextCtrl (wnd, LOG_MSG, wxEmptyString, 
                               wxDefaultPosition, wxSize (420, 110), 
                               wxTE_READONLY | wxTE_MULTILINE );

    wxButton * buttonClose = new wxButton (wnd, wxID_OK, _("&Close"));
    m_buttonView  = new wxButton (wnd, ID_View,  _("&View"));
    m_buttonGet   = new wxButton (wnd, ID_Get,   _("&Get"));
    m_buttonDiff  = new wxButton (wnd, ID_Diff,  _("&Diff"));
    m_buttonMerge = new wxButton (wnd, ID_Merge, _("&Merge"));
    m_buttonAnnotate = new wxButton (wnd, ID_Annotate, _("&Annotate"));

    // View/Get/Diff disabled for Alpha 3 Milestone
    m_buttonView ->Enable (false);
    m_buttonGet  ->Enable (false);
    m_buttonDiff ->Enable (false);
    m_buttonMerge->Enable (false);
    m_buttonAnnotate->Enable (false);

    // position controls

    wxBoxSizer * logSizer = new wxBoxSizer (wxVERTICAL);
    logSizer->Add (historyLabel, 0, wxALL, 5);
    logSizer->Add (m_logList, 1, wxLEFT | wxEXPAND);

    wxBoxSizer * buttonSizer = new wxBoxSizer (wxVERTICAL);
    buttonSizer->Add (buttonClose,   0, wxALL | wxEXPAND, 5);
    buttonSizer->Add (m_buttonView,  0, wxALL | wxEXPAND, 5);
    buttonSizer->Add (m_buttonGet,   0, wxALL | wxEXPAND, 5);
    buttonSizer->Add (m_buttonDiff,  0, wxALL | wxEXPAND, 5);
    buttonSizer->Add (m_buttonMerge, 0, wxALL | wxEXPAND, 5);
    buttonSizer->Add (m_buttonAnnotate, 0, wxALL | wxEXPAND, 5);

    wxBoxSizer * topSizer = new wxBoxSizer (wxHORIZONTAL);
    topSizer->Add (logSizer, 1, wxEXPAND | wxALL, 5);
    topSizer->Add (buttonSizer, 0, wxALL, 5);


    wxStaticBox * boxMessage = 
      new wxStaticBox (wnd, -1, _("Log Message:"));
    wxStaticBoxSizer *messageSizer = 
      new wxStaticBoxSizer (boxMessage, wxHORIZONTAL);
    messageSizer->Add (m_logMsg, 1, wxALL | wxEXPAND, 2);

    wxBoxSizer * mainSizer = new wxBoxSizer (wxVERTICAL);
    mainSizer->Add (topSizer, 1, wxALL | wxEXPAND, 5);
    mainSizer->Add (messageSizer, 0, wxALL | wxEXPAND, 5);

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
    svn_revnum_t revnum = m_logList->GetSelectedRevision ();

    GetData * data = new GetData ();
    data->revision = revnum;
    data->path = path;

    ActionEvent::Post (parent, TOKEN_GET, data);
  }

  /**
   * handle the "View" button
   */
  void
  OnView ()
  {
    svn_revnum_t revnum = m_logList->GetSelectedRevision ();
 
    GetData * data = new GetData ();
    data->revision = revnum;
    data->path = path;

    ActionEvent::Post (parent, TOKEN_VIEW, data);
  }
    
  void
  OnSelected (long index)
  {
    const svn::LogEntry & entry = (*entries)[index];

    wxString message (Utf8ToLocal (entry.message.c_str ()));
    message.Trim (false);

    m_logMsg->Show (false);
    m_logMsg->SetValue (message);
    m_logMsg->Show (true);

    CheckButtons ();
  }

  void
  OnDiff ()
  {
    RevnumArray array (m_logList->GetSelectedRevisions ());

    DiffData * data = 0;
    if (array.Count () == 2)
    {
      data = new DiffData ();
      data->compareType = DiffData::TWO_REVISIONS;
      data->revision1 = svn::Revision (array[0]);
      data->revision2 = svn::Revision (array[1]);
    }
    else if (array.Count () == 1)
    {
      data = new DiffData ();
      data->compareType = DiffData::WITH_DIFFERENT_REVISION;
      data->revision1 = svn::Revision (array[0]);
    }
    else
    {
      wxMessageBox (_("Invalid selection. At least one revisions is needed for diff and no more than two."),
                    _("Error"), wxOK | wxICON_ERROR, parent);
      return;
    }

    ActionEvent::Post (parent, TOKEN_DIFF, data);
  }

  void
  OnMerge ()
  {
    RevnumArray array (m_logList->GetSelectedRevisions ());

    if (array.Count () != 2)
    {
      wxMessageBox (_("Invalid selection. Exactly two revisions needed for merge."),
                    _("Error"), wxOK | wxICON_ERROR, parent);
      return;
    }

    MergeData * data = new MergeData ();
    data->Path1 = path;
    data->Path2 = path;
    if (array[0] > array[1])
    {
      data->Path1Rev << array[1];
      data->Path2Rev << array[0];
    }
    else
    {
      data->Path1Rev << array[0];
      data->Path2Rev << array[1];
    }

    ActionEvent::Post (parent, TOKEN_MERGE, data);
  }

  void
  OnAnnotate ()
  {
    RevnumArray array (m_logList->GetSelectedRevisions ());

    AnnotateData * data = 0;
    if (array.Count () == 1)
    {
      data = new AnnotateData ();
      data->path = path;
      data->endRevision = svn::Revision (array[0]);
    }
    else
    {
      wxMessageBox (_("Invalid selection. Only one revision may be selected for annotate"),
                    _("Error"), wxOK | wxICON_ERROR, parent);
      return;
    }

    ActionEvent::Post (parent, TOKEN_ANNOTATE, data);
  }

  void 
  CheckButtons ()
  {
    int count = m_logList->GetSelectedItemCount ();

    m_buttonGet  ->Enable (count == 1);
    m_buttonView ->Enable (count == 1);
    // If the bookmark is a working copy, then we can compare a revision against the local copy
    svn::Path bookmarkFilePath (PathUtf8 (path));
    if (bookmarkFilePath.isUrl())
    {
      m_buttonDiff ->Enable ((count == 2));
    }
    else
    {
      m_buttonDiff ->Enable ((count == 1) || (count == 2));
    }
    m_buttonMerge->Enable (count == 2);
    m_buttonAnnotate->Enable (count == 1);
  }
};

BEGIN_EVENT_TABLE (LogDlg, wxDialog)
  EVT_BUTTON (ID_Get,   LogDlg::OnGet)
  EVT_BUTTON (ID_View,  LogDlg::OnView)
  EVT_BUTTON (ID_Diff,  LogDlg::OnDiff)
  EVT_BUTTON (ID_Merge, LogDlg::OnMerge)
  EVT_BUTTON (ID_Annotate, LogDlg::OnAnnotate)
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


void
LogDlg::OnDiff (wxCommandEvent & event)
{
  m->OnDiff ();
}

void
LogDlg::OnMerge (wxCommandEvent & event)
{
  m->OnMerge ();
}

void
LogDlg::OnAnnotate (wxCommandEvent & event)
{
  m->OnAnnotate ();
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
