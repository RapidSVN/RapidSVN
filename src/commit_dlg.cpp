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

// wxwindows
#include "wx/wx.h"
#include "wx/valgen.h"

// app
#include "commit_dlg.hpp"

BEGIN_EVENT_TABLE (CommitDlg, wxDialog)
END_EVENT_TABLE ()

CommitDlg::CommitDlg(wxWindow* parent)
  : wxDialog(parent, -1, _("Commit changes to repository"),
             wxDefaultPosition, wxDefaultSize,
             wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  InitializeData();
  CentreOnParent();
}

void
CommitDlg::InitializeData ()
{
  // create controls
  wxStaticBox* msgBox = new wxStaticBox(this, -1, _("Enter log message"));

  wxSize msgSize (GetCharWidth () * 80, GetCharHeight () * 10);

  wxTextCtrl* msg = 
    new wxTextCtrl(this, -1, _(""), wxDefaultPosition, 
                   msgSize, wxTE_MULTILINE,
                   wxTextValidator(wxFILTER_NONE, &m_data.LogMessage));
  wxCheckBox* recursive  = 
    new wxCheckBox(this, -1, "Recursive",
                   wxDefaultPosition, wxDefaultSize, 0,
                   wxGenericValidator(&m_data.Recursive));
  wxButton* ok =  new wxButton( this, wxID_OK, _("OK" ));
  wxButton* cancel = new wxButton( this, wxID_CANCEL, _("Cancel"));

  // position controls
  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

  // The message field:
  wxStaticBoxSizer *msgSizer = 
    new wxStaticBoxSizer(msgBox, wxHORIZONTAL);
  msgSizer->Add(msg, 1, wxALL | wxEXPAND, 5);
  
  // The buttons:
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
  buttonSizer->Add(recursive, 1, wxALL | wxALIGN_CENTER_VERTICAL | wxALIGN_LEFT, 10);
  buttonSizer->Add(ok, 0, wxALL, 10);
  buttonSizer->Add(cancel, 0, wxALL, 10);

  wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
  topSizer->Add(msgSizer, 1, wxALL | wxEXPAND, 5);

  // Add all the sizers to the main sizer
  mainSizer->Add (topSizer, 1, wxLEFT | wxRIGHT | wxEXPAND, 5);
  mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);

  SetAutoLayout(true);
  SetSizer(mainSizer);

  mainSizer->SetSizeHints(this);
  mainSizer->Fit(this);
}

const CommitData &
CommitDlg::GetData () const
{
  return m_data;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
