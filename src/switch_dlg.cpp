/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.
 * If not, see <http://www.gnu.org/licenses/>.
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
#include "svncpp/revision.hpp"

// app
#include "hist_entries.hpp"
#include "hist_val.hpp"
#include "switch_dlg.hpp"
#include "utils.hpp"

enum
{
  ID_USELATEST = 100,
  ID_REVISION,
  ID_URL,
  ID_RELOCATE
};

struct SwitchDlg::Data
{
private:
  wxTextCtrl * m_textRevision;
  wxCheckBox * m_checkUseLatest;
  wxCheckBox * m_checkRelocate;
  wxComboBox * m_comboUrl;
  wxButton * m_buttonOk;
public:
  wxString url;
  wxString revision;
  bool recursive;
  bool relocate;
  bool useLatest;

  Data(wxWindow * window, const wxString & url_,
       bool recursive_, bool relocate_)
      : m_textRevision(0), m_checkUseLatest(0), m_comboUrl(0),
      m_buttonOk(0),
      url(url_), revision(wxT("")),
      recursive(recursive_), relocate(relocate_),
      useLatest(true)
  {
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *middleSizer = new wxBoxSizer(wxVERTICAL);
    wxBoxSizer *optionSizer = new wxBoxSizer(wxHORIZONTAL);
    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);

    // The URL fields:
    {
      wxStaticBox * box = new wxStaticBox(window, -1, _("URL"));
      wxStaticBoxSizer * sizer = new wxStaticBoxSizer(box, wxHORIZONTAL);

      HistoryValidator val(HISTORY_REPOSITORY, &url);
      m_comboUrl = new wxComboBox(window, ID_URL, wxEmptyString,
                                  wxDefaultPosition, wxSize(235, -1),
                                  0, 0, wxCB_DROPDOWN, val);

      sizer->Add(m_comboUrl, 1, wxALL | wxEXPAND, 5);
      middleSizer->Add(sizer, 1, wxALL | wxEXPAND, 5);
    }

    // The revision fields:
    {
      wxStaticBox * box = new wxStaticBox(window, -1, _("Revision"));
      wxStaticBoxSizer *revSizer = new wxStaticBoxSizer(box, wxHORIZONTAL);

      wxTextValidator val(wxFILTER_NUMERIC, &revision);
      m_textRevision = new wxTextCtrl(window, ID_REVISION, wxEmptyString,
                                      wxDefaultPosition,
                                      wxDefaultSize, 0, val);
      revSizer->Add(m_textRevision, 1,
                    wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

      wxGenericValidator valUseLatest(&useLatest);
      m_checkUseLatest = new wxCheckBox(window, ID_USELATEST,
                                        _("Use latest"),
                                        wxDefaultPosition,
                                        wxDefaultSize, 0, valUseLatest);
      revSizer->Add(m_checkUseLatest, 0,
                    wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);

      middleSizer->Add(revSizer, 1, wxALL | wxEXPAND, 5);
    }

    // The recursive checkbox

    {
      wxGenericValidator val(&recursive);
      wxCheckBox * checkRecursive =
        new wxCheckBox(window, -1, _("Recursive"),
                       wxDefaultPosition, wxDefaultSize, 0, val);
      optionSizer->Add(checkRecursive, 0,
                       wxALIGN_CENTER_HORIZONTAL | wxALL , 5);
    }

    // The "relocate" checkbox
    {
      wxGenericValidator val(&relocate);
      m_checkRelocate = new wxCheckBox(window, ID_RELOCATE,
                                       _("Relocate"),
                                       wxDefaultPosition,
                                       wxDefaultSize, 0,
                                       val);
      optionSizer->Add(m_checkRelocate, 0,
                       wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
    }


    // The buttons:
    m_buttonOk = new wxButton(window, wxID_OK, _("OK"));
    buttonSizer->Add(m_buttonOk, 0, wxALL, 10);
    wxButton * button = new wxButton(window, wxID_CANCEL, _("Cancel"));
    buttonSizer->Add(button, 0, wxALL, 10);

    // Add all the sizers to the main sizer
    mainSizer->Add(middleSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add(optionSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add(5, 5, 1, wxEXPAND);
    mainSizer->Add(buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

    window->SetAutoLayout(true);
    window->SetSizer(mainSizer);

    mainSizer->SetSizeHints(window);
    mainSizer->Fit(window);

    m_buttonOk->SetDefault();
  }

  void
  EnableControls()
  {
    m_textRevision->Enable(!m_checkUseLatest->IsChecked());
  }

  void
  CheckButtons()
  {
    bool ok = true;

    if (!m_checkUseLatest->IsChecked())
    {
      ok = CheckRevision(m_textRevision->GetValue());
    }

    if (ok)
    {
      svn::Path UrlUtf8(PathUtf8(m_comboUrl->GetValue()));
      ok = UrlUtf8.isUrl();
    }

    m_buttonOk->Enable(ok);
  }

};

BEGIN_EVENT_TABLE(SwitchDlg, wxDialog)
  EVT_CHECKBOX(ID_USELATEST, SwitchDlg::OnUseLatest)
  EVT_TEXT(ID_URL, SwitchDlg::OnText)
  EVT_TEXT(ID_REVISION, SwitchDlg::OnText)
END_EVENT_TABLE()

SwitchDlg::SwitchDlg(wxWindow* parent, const wxString & url,
                     bool recursive, bool relocate)
    : wxDialog(parent, -1, _("Switch URL"),
               wxDefaultPosition, wxDefaultSize,
               wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data(this, url, recursive, relocate);
  CentreOnParent();

  wxDialog::InitDialog();

  m->EnableControls();
  m->CheckButtons();
}

SwitchDlg::~SwitchDlg()
{
  delete m;
}

void
SwitchDlg::OnUseLatest(wxCommandEvent &)
{
  m->EnableControls();
  m->CheckButtons();
}

void
SwitchDlg::OnText(wxCommandEvent &)
{
  m->CheckButtons();
}

svn::Revision
SwitchDlg::GetRevision() const
{
  if (m->useLatest)
    return svn::Revision::HEAD;
  else
  {
    svn_revnum_t revnum;

    TrimString(m->revision);
    m->revision.ToLong(&revnum);

    return revnum;
  }
}


const wxString &
SwitchDlg::GetUrl() const
{
  return m->url;
}


bool
SwitchDlg::GetRelocate() const
{
  return m->relocate;
}


bool
SwitchDlg::GetRecursive() const
{
  return m->recursive;
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
