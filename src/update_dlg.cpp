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
#include "update_data.hpp"

enum
{
  ID_USELATEST = 100
};

struct UpdateDlg::Data
{
private:
  wxTextCtrl * m_revisionText;
  wxCheckBox * m_useLatestCheck;
public:
  UpdateData data;

  Data (wxWindow * window, bool recursive)
  {
    data.recursive = recursive;

    wxBoxSizer *mainSizer = new wxBoxSizer (wxVERTICAL);
    wxBoxSizer *topSizer = new wxBoxSizer (wxHORIZONTAL);
    wxBoxSizer *middleSizer = new wxBoxSizer (wxHORIZONTAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer (wxHORIZONTAL);

    // The revision fields:
    wxStaticBoxSizer *revSizer = new wxStaticBoxSizer (
      new wxStaticBox (window, -1, _("Revision")), wxHORIZONTAL);
    
    m_revisionText = new wxTextCtrl (window, -1, "",
      wxDefaultPosition, wxDefaultSize, 0,
      wxTextValidator (wxFILTER_NUMERIC, &data.revision));
    revSizer->Add (m_revisionText, 1, 
      wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

    m_useLatestCheck = new wxCheckBox (window, ID_USELATEST, "Use latest",
      wxDefaultPosition, wxDefaultSize, 0,
      wxGenericValidator (&data.useLatest));
    revSizer->Add (m_useLatestCheck, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
    topSizer->Add (revSizer, 1, wxALL, 5);

    // The recursive checkbox
    {
      wxGenericValidator val (&data.recursive);
      wxCheckBox * checkRecursive = 
        new wxCheckBox (window, -1, _("Recursive"), 
                        wxDefaultPosition, wxDefaultSize, 0, val);
      middleSizer->Add (checkRecursive, 0, wxALIGN_CENTER_HORIZONTAL | wxALL , 5);
    }
    
    // The buttons:
    buttonSizer->Add(new wxButton( window, wxID_OK, _("OK" )), 0, 
                     wxALL, 10);
    buttonSizer->Add(new wxButton( window, wxID_CANCEL, _("Cancel")), 0, 
                     wxALL, 10);

    // Add all the sizers to the main sizer
    mainSizer->Add (topSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add (middleSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

    window->SetAutoLayout(true);
    window->SetSizer(mainSizer);

    mainSizer->SetSizeHints(window);
    mainSizer->Fit(window);

  }

  void 
  EnableControls()
  {
    m_revisionText->Enable(!m_useLatestCheck->IsChecked());
  }

};

BEGIN_EVENT_TABLE (UpdateDlg, wxDialog)
  EVT_CHECKBOX (ID_USELATEST, UpdateDlg::OnUseLatest)
END_EVENT_TABLE ()

UpdateDlg::UpdateDlg (wxWindow* parent, int flags, bool recursive)
  : wxDialog(parent, -1, _("Update"),
             wxDefaultPosition, wxDefaultSize,
             wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (this, recursive);
  CentreOnParent();
}

UpdateDlg::~UpdateDlg ()
{
  delete m;
}

void
UpdateDlg::InitDialog()
{
  wxDialog::InitDialog();
  m->EnableControls();
}

void 
UpdateDlg::OnUseLatest(wxCommandEvent &)
{
  m->EnableControls();
}

const UpdateData &
UpdateDlg::GetData () const 
{
  return m->data;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
