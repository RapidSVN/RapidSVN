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

// svncpp
#include "svncpp/url.hpp"

// app
#include "update_dlg.hpp"
#include "update_data.hpp"
#include "utils.hpp"

enum
{
  ID_USELATEST = 100,
  ID_REVISION,
  ID_URL
};

struct UpdateDlg::Data
{
private:
  wxTextCtrl * m_textRevision;
  wxCheckBox * m_checkUseLatest;
  wxTextCtrl * m_textUrl;
  wxButton * m_buttonOk;
  const int m_flags;
public:
  UpdateData data;

  Data (wxWindow * window, int flags, bool recursive)
    : m_textRevision (0), m_checkUseLatest (0), m_textUrl (0),
      m_buttonOk (0), m_flags (flags)
  {
    data.recursive = recursive;

    wxBoxSizer *mainSizer = new wxBoxSizer (wxVERTICAL);
    wxBoxSizer *middleSizer = new wxBoxSizer (wxVERTICAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer (wxHORIZONTAL);

    // The URL fields:
    if (withUrl ())
    {
      wxStaticBox * box = new wxStaticBox (window, -1, _("URL"));
      wxStaticBoxSizer * sizer = 
        new wxStaticBoxSizer (box, wxHORIZONTAL);
      wxTextValidator val (wxFILTER_NONE, &data.url);
      m_textUrl = new wxTextCtrl (window, ID_URL, "",
                                  wxDefaultPosition, 
                                  wxDefaultSize, 0, val);
      sizer->Add (m_textUrl, 1, wxALL | wxEXPAND, 5);
      middleSizer->Add (sizer, 1, wxALL | wxEXPAND, 5);
    }

    // The revision fields:
    if (withRevision ())
    {
      wxStaticBox * box = new wxStaticBox (window, -1, _("Revision"));
      wxStaticBoxSizer *revSizer = 
        new wxStaticBoxSizer (box, wxHORIZONTAL);
      wxTextValidator val (wxFILTER_NUMERIC, &data.revision);
      m_textRevision = new wxTextCtrl (window, ID_REVISION, "",
                                       wxDefaultPosition, 
                                       wxDefaultSize, 0, val);
      revSizer->Add (m_textRevision, 1, 
                     wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

      wxGenericValidator valCheck (&data.useLatest);
      m_checkUseLatest = new wxCheckBox (window, ID_USELATEST, 
                                         _("Use latest"),
                                         wxDefaultPosition, 
                                         wxDefaultSize, 0, valCheck);
      revSizer->Add (m_checkUseLatest, 0, 
                     wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
      middleSizer->Add (revSizer, 1, wxALL | wxEXPAND, 5);
    }

    // The recursive checkbox
    if (withRecursive ())
    {
      wxGenericValidator val (&data.recursive);
      wxCheckBox * checkRecursive = 
        new wxCheckBox (window, -1, _("Recursive"), 
                        wxDefaultPosition, wxDefaultSize, 0, val);
      middleSizer->Add (checkRecursive, 0, 
                        wxALIGN_CENTER_HORIZONTAL | wxALL , 5);
    }
    
    // The buttons:
    m_buttonOk = new wxButton (window, wxID_OK, _("OK" ));
    buttonSizer->Add (m_buttonOk, 0, wxALL, 10);
    wxButton * button = new wxButton (window, wxID_CANCEL, _("Cancel"));
    buttonSizer->Add (button, 0, wxALL, 10);

    // Add all the sizers to the main sizer
    //mainSizer->Add (topSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add (middleSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add (5, 5, 1, wxEXPAND);
    mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

    window->SetAutoLayout(true);
    window->SetSizer(mainSizer);

    mainSizer->SetSizeHints(window);
    mainSizer->Fit(window);

  }

  void 
  EnableControls()
  {
    if (withRevision ())
      m_textRevision->Enable(!m_checkUseLatest->IsChecked());
  }

  void
  CheckButtons ()
  {
    bool ok = true;

    if (withRevision ())
    {
      if (!m_checkUseLatest->IsChecked ())
      {
        ok = CheckRevision (m_textRevision->GetValue ());
      }
    }
    if (ok && withUrl ())
    {
      ok = svn::Url::isValid(m_textUrl->GetValue ());
    }

    m_buttonOk->Enable (ok);
  }

  bool 
  withRevision ()
  {
    return (m_flags & WITHOUT_REVISION) == 0;
  }

  bool
  withUrl ()
  {
    return (m_flags & WITH_URL) != 0;
  }

  bool
  withRecursive ()
  {
    return (m_flags & WITHOUT_RECURSIVE) == 0;
  }

};

BEGIN_EVENT_TABLE (UpdateDlg, wxDialog)
  EVT_CHECKBOX (ID_USELATEST, UpdateDlg::OnUseLatest)
  EVT_TEXT (ID_URL, UpdateDlg::OnText)
  EVT_TEXT (ID_REVISION, UpdateDlg::OnText)
END_EVENT_TABLE ()

const int UpdateDlg::WITH_URL = 1;
const int UpdateDlg::WITHOUT_RECURSIVE = 2;
const int UpdateDlg::WITHOUT_REVISION = 4;

UpdateDlg::UpdateDlg (wxWindow* parent, const char * title, int flags, 
                      bool recursive)
  : wxDialog(parent, -1, title,
             wxDefaultPosition, wxDefaultSize,
             wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (this, flags, recursive);
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
  m->CheckButtons ();
}

void 
UpdateDlg::OnUseLatest(wxCommandEvent &)
{
  m->EnableControls();
  m->CheckButtons ();
}

UpdateData &
UpdateDlg::GetData ()
{
  return m->data;
}

void
UpdateDlg::OnText (wxCommandEvent &)
{
  m->CheckButtons ();
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
