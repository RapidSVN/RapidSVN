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

// wxWidgets
#include "wx/wx.h"
#include "wx/valgen.h"

// svncpp
#include "svncpp/path.hpp"

// app
#include "hist_entries.hpp"
#include "hist_val.hpp"
#include "update_dlg.hpp"
#include "update_data.hpp"
#include "utils.hpp"

enum
{
  ID_USELATEST = 100,
  ID_REVISION,
  ID_URL,
  ID_IGNOREEXTERNALS
};

struct UpdateDlg::Data
{
private:
  wxTextCtrl * m_textRevision;
  wxCheckBox * m_checkUseLatest;
  wxCheckBox * m_checkIgnoreExternals;
  wxComboBox * m_comboUrl;
  wxButton * m_buttonOk;
  const int m_flags;
public:
  UpdateData data;

  Data (wxWindow * window, int flags, bool recursive)
    : m_textRevision (0), m_checkUseLatest (0), m_comboUrl (0),
      m_buttonOk (0), m_flags (flags)
  {
    data.recursive = recursive;

    wxBoxSizer *mainSizer = new wxBoxSizer (wxVERTICAL);
    wxBoxSizer *middleSizer = new wxBoxSizer (wxVERTICAL);
    wxBoxSizer *optionSizer = new wxBoxSizer (wxHORIZONTAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer (wxHORIZONTAL);

    // The URL fields:
    if (withUrl ())
    {
      wxStaticBox * box = new wxStaticBox (window, -1, _("URL"));
      wxStaticBoxSizer * sizer = new wxStaticBoxSizer (box, wxHORIZONTAL);

      HistoryValidator val (HISTORY_REPOSITORY, &data.url);
      m_comboUrl = new wxComboBox (window, ID_URL, wxEmptyString,
                                   wxDefaultPosition, wxSize (235, -1),
                                   0, 0, wxCB_DROPDOWN, val);

      sizer->Add (m_comboUrl, 1, wxALL | wxEXPAND, 5);
      middleSizer->Add (sizer, 1, wxALL | wxEXPAND, 5);
    }

    // The revision fields:
    if (withRevision ())
    {
      wxStaticBox * box = new wxStaticBox (window, -1, _("Revision"));
      wxStaticBoxSizer *revSizer = new wxStaticBoxSizer (box, wxHORIZONTAL);

      wxTextValidator val (wxFILTER_NUMERIC, &data.revision);
      m_textRevision = new wxTextCtrl (window, ID_REVISION, wxEmptyString,
                                       wxDefaultPosition,
                                       wxDefaultSize, 0, val);
      revSizer->Add (m_textRevision, 1,
                     wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

      wxGenericValidator valUseLatest (&data.useLatest);
      m_checkUseLatest = new wxCheckBox (window, ID_USELATEST,
                                         _("Use latest"),
                                         wxDefaultPosition,
                                         wxDefaultSize, 0, valUseLatest);
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
      optionSizer->Add (checkRecursive, 0,
                        wxALIGN_CENTER_HORIZONTAL | wxALL , 5);
    }

    // The "ignore externals" checkbox
    wxGenericValidator valIgnoreExternals (&data.ignoreExternals);
    m_checkIgnoreExternals = new wxCheckBox (window, ID_IGNOREEXTERNALS,
                                             _("Ignore externals"),
                                             wxDefaultPosition,
                                             wxDefaultSize, 0,
                                             valIgnoreExternals);
    optionSizer->Add (m_checkIgnoreExternals, 0,
                      wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);


    // The buttons:
    m_buttonOk = new wxButton (window, wxID_OK, _("OK" ));
    buttonSizer->Add (m_buttonOk, 0, wxALL, 10);
    wxButton * button = new wxButton (window, wxID_CANCEL, _("Cancel"));
    buttonSizer->Add (button, 0, wxALL, 10);

    // Add all the sizers to the main sizer
    mainSizer->Add (middleSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add (optionSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add (5, 5, 1, wxEXPAND);
    mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

    window->SetAutoLayout(true);
    window->SetSizer(mainSizer);

    mainSizer->SetSizeHints(window);
    mainSizer->Fit(window);

    m_buttonOk->SetDefault ();
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
      svn::Path UrlUtf8 (PathUtf8 (m_comboUrl->GetValue ()));
      ok = UrlUtf8.isUrl ();
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

UpdateDlg::UpdateDlg (wxWindow* parent, const wxString & title, int flags,
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
