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
#include "update_dlg.hpp"

enum
{
  ID_USELATEST = 100
};

BEGIN_EVENT_TABLE (UpdateDlg, wxDialog)
EVT_CHECKBOX (ID_USELATEST, UpdateDlg::OnUseLatest)
END_EVENT_TABLE ()

UpdateDlg::UpdateDlg(wxWindow* parent)
  : wxDialog(parent, -1, _("Update"),
             wxDefaultPosition, wxDefaultSize,
             wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  InitData();
  CentreOnParent();
}

void
UpdateDlg::InitData ()
{
  wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer *topSizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *middleSizer = new wxBoxSizer(wxHORIZONTAL);
  wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

  // The revision fields:
  wxStaticBoxSizer *revSizer = new wxStaticBoxSizer(
    new wxStaticBox(this, -1, _("Revision")), wxHORIZONTAL);
    
  m_revisionLabel = new wxStaticText(this, -1, _("Number")); 
  revSizer->Add(m_revisionLabel, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
   
  m_revisionText = new wxTextCtrl (this, -1, _(""),
    wxDefaultPosition, wxDefaultSize, 0,
    wxTextValidator(wxFILTER_NUMERIC, &m_data.Revision));
  revSizer->Add (m_revisionText, 1, 
    wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

  m_useLatestCheck = new wxCheckBox(this, ID_USELATEST, "Use latest",
    wxDefaultPosition, wxDefaultSize, 0,
    wxGenericValidator(&m_data.UseLatest));
  revSizer->Add (m_useLatestCheck, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);

  topSizer->Add(revSizer, 1, wxALL, 5);
  
  // The buttons:
  buttonSizer->Add(new wxButton( this, wxID_OK, _("OK" )), 0, 
                   wxALL, 10);
  buttonSizer->Add(new wxButton( this, wxID_CANCEL, _("Cancel")), 0, 
                   wxALL, 10);

  // Add all the sizers to the main sizer
  mainSizer->Add (topSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
  mainSizer->Add (middleSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
  mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

  SetAutoLayout(true);
  SetSizer(mainSizer);

  mainSizer->SetSizeHints(this);
  mainSizer->Fit(this);
}

void
UpdateDlg::InitDialog()
{
  wxDialog::InitDialog();
  EnableControls();
}

void 
UpdateDlg::OnUseLatest(wxCommandEvent &)
{
  UpdateDlg::EnableControls();
}

void 
UpdateDlg::EnableControls()
{
  m_revisionLabel->Enable(!m_useLatestCheck->IsChecked());
  m_revisionText->Enable(!m_useLatestCheck->IsChecked());
}

const UpdateData &
UpdateDlg::GetData () const 
{
  return m_data;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
