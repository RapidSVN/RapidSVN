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

// wxWidgets
#include "wx/wx.h"
#include "wx/valgen.h"

// app
#include "commit_dlg.hpp"

struct CommitDlg::Data
{
public:
  wxString message;
  bool recursive;

  Data (wxWindow * window, bool unexpectedCommit)
    : recursive (true)
  {
    // create controls
    wxStaticBox* msgBox =
      new wxStaticBox(window, -1, unexpectedCommit ? _("This action has resulted in a Commit - please enter a log message") : _("Enter log message"));

    wxSize msgSize (window->GetCharWidth () * 80,
                    window->GetCharHeight () * 10);

    wxTextCtrl* msg;
    {
      wxTextValidator val (wxFILTER_NONE, &message);
      msg = new wxTextCtrl (window, -1, wxEmptyString, wxDefaultPosition,
                            msgSize, wxTE_MULTILINE, val);
    }
    wxCheckBox * checkRecursive = NULL;
    if (!unexpectedCommit)
    {
      wxGenericValidator val (&recursive);
      checkRecursive =
        new wxCheckBox (window, -1, _("Recursive"),
                        wxDefaultPosition, wxDefaultSize, 0,
                        val);
    }
    wxButton* ok =
      new wxButton (window, wxID_OK, _("OK" ));
    wxButton* cancel =
      new wxButton (window, wxID_CANCEL, _("Cancel"));

    // position controls
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // The message field:
    wxStaticBoxSizer *msgSizer =
      new wxStaticBoxSizer (msgBox, wxHORIZONTAL);
    msgSizer->Add (msg, 1, wxALL | wxEXPAND, 5);

    // The buttons:
    wxBoxSizer *buttonSizer = new wxBoxSizer (wxHORIZONTAL);
    if (!unexpectedCommit)
    {
      buttonSizer->Add (checkRecursive, 1,
                        wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT,
                        10);
    }
    buttonSizer->Add (ok, 0, wxALL, 10);
    buttonSizer->Add (cancel, 0, wxALL, 10);

    wxBoxSizer *topSizer = new wxBoxSizer (wxHORIZONTAL);
    topSizer->Add (msgSizer, 1, wxALL | wxEXPAND, 5);

    // Add all the sizers to the main sizer
    mainSizer->Add (topSizer, 1, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);

    window->SetAutoLayout (true);
    window->SetSizer (mainSizer);

    mainSizer->SetSizeHints (window);
    mainSizer->Fit (window);
  }
};


BEGIN_EVENT_TABLE (CommitDlg, wxDialog)
END_EVENT_TABLE ()

CommitDlg::CommitDlg (wxWindow* parent, bool unexpectedCommit)
  : wxDialog(parent, -1, unexpectedCommit ? _("Commit Log Message") : _("Commit"),
             wxDefaultPosition, wxDefaultSize,
             wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (this, unexpectedCommit);
  CentreOnParent ();
}

CommitDlg::~CommitDlg ()
{
  delete m;
}

const wxString &
CommitDlg::GetMessage () const
{
  return m->message;
}

bool
CommitDlg::GetRecursive () const
{
  return m->recursive;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
