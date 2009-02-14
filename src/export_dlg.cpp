/*
 * ====================================================================
 * Copyright (c) 2002-2009 The RapidSvn Group.  All rights reserved.
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
#include "wx/app.h"
#include "wx/cshelp.h"
#include "wx/sizer.h"
#include "wx/valgen.h"

// svncpp
#include "svncpp/path.hpp"

// app
#include "hist_entries.hpp"
#include "hist_val.hpp"
#include "export_data.hpp"
#include "export_dlg.hpp"
#include "utils.hpp"

enum
{
  ID_USE_LATEST = 100,
  ID_NOT_SPECIFIED = 200,
  ID_NATIVE_EOL = 1,
  ID_BUTTON_BROWSE
};

struct ExportDlg::Data
{
private:
  wxCheckBox * m_checkUseLatest;
  wxCheckBox * m_checkNotSpecified;
  wxTextCtrl * m_textRevision;
  wxTextCtrl * m_textPegRevision;
  wxTextCtrl * m_textDestPath;
  wxComboBox * m_comboSrcPath;
  wxComboBox * m_comboNativeEol;
  wxButton * m_buttonOk;

public:
  ExportData data;

  Data(wxWindow * wnd)
  {
    // create controls
    wxStaticBox* urlBox =
      new wxStaticBox(wnd, 0, _("URL"));
    HistoryValidator valModule(HISTORY_REPOSITORY, &data.SrcPath);
    m_comboSrcPath =
      new wxComboBox(wnd, -1, wxEmptyString, wxDefaultPosition,
                     wxSize(235, -1), 0, 0, wxCB_DROPDOWN, valModule);
    m_comboSrcPath->SetHelpText(_("Enter the repository URL (not local path) here."));
    wxStaticBox* destBox =
      new wxStaticBox(wnd, 0, _("Destination Directory"));
    wxTextValidator valDestPath(wxFILTER_NONE, &data.DestPath);
    m_textDestPath =
      new wxTextCtrl(wnd, -1, wxEmptyString, wxDefaultPosition,
                     wxSize(205, -1), 0, valDestPath);
    m_textDestPath->SetHelpText(_("Enter the local path where you want the code be exported."));
    wxButton* browse =
      new wxButton(wnd, ID_BUTTON_BROWSE, wxT("..."),
                   wxDefaultPosition, wxSize(20, -1));

    wxStaticBox* revisionBox =
      new wxStaticBox(wnd, -1, _("Revision"));
    wxTextValidator valRevision(wxFILTER_NUMERIC, &data.Revision);
    m_textRevision =
      new wxTextCtrl(wnd, -1, wxEmptyString, wxDefaultPosition,
                     wxSize(50, -1), 0, valRevision);
    m_textRevision->SetHelpText(_("If not using the latest version of the files, specify which revision to use here."));

    wxGenericValidator valLatest(&data.UseLatest);
    m_checkUseLatest =
      new wxCheckBox(wnd, ID_USE_LATEST, _("Use latest"),
                     wxDefaultPosition, wxDefaultSize, 0, valLatest);
    m_checkUseLatest->SetHelpText(_("Set this to get the latest version of the files in the repository."));

    wxStaticBox* pegRevisionBox =
      new wxStaticBox(wnd, -1, _("Peg Revision"));
    wxTextValidator valPegRevision(wxFILTER_NUMERIC, &data.PegRevision);
    m_textPegRevision =
      new wxTextCtrl(wnd, -1, wxEmptyString, wxDefaultPosition,
                     wxSize(50, -1), 0, valPegRevision);
    m_textPegRevision->SetHelpText(_("If the files were renamed or moved some time, specify which peg revision to use here."));

    wxGenericValidator valNotSpecified(&data.NotSpecified);
    m_checkNotSpecified =
      new wxCheckBox(wnd, ID_NOT_SPECIFIED, _("Not specified"),
                     wxDefaultPosition, wxDefaultSize, 0, valNotSpecified);
    m_checkNotSpecified->SetHelpText(_("Set this to use BASE/HEAD (current) peg revision of the files."));

    wxCheckBox* recursive =
      new wxCheckBox(wnd, -1, _("Recursive"),
                     wxDefaultPosition, wxDefaultSize, 0,
                     wxGenericValidator(&data.Recursive));
    recursive->SetHelpText(_("Set to get all subdirectories from the URL also."));

    wxCheckBox* overwrite =
      new wxCheckBox(wnd, -1, _("Overwrite"),
                     wxDefaultPosition, wxDefaultSize, 0,
                     wxGenericValidator(&data.Overwrite));
    overwrite->SetHelpText(_("Force to execute even if destination directory not empty, causes overwriting of files with the same names."));

    wxCheckBox* ignoreExternals =
      new wxCheckBox(wnd, -1, _("Ignore externals"),
                     wxDefaultPosition, wxDefaultSize, 0,
                     wxGenericValidator(&data.IgnoreExternals));
    ignoreExternals->SetHelpText(_("Set to ignore external definitions and the external working copies managed by them."));
    wxStaticText * labelEol = new wxStaticText(
      wnd, -1, _("EOL:"), wxDefaultPosition);

    wxString eol [] =
    {
      _("native"),
      _("CRLF (Windows)"),
      _("LF (Unix)"),
      _("CR (MacOS)")
    };
    m_comboNativeEol = new wxComboBox(wnd, ID_NATIVE_EOL, _("native"),
                                      wxDefaultPosition, wxDefaultSize, 0,
                                      eol, wxCB_DROPDOWN);
    m_comboNativeEol->SetHelpText(_("Enter what kind of symbol(s) do you want as EOL (end of line) in exported files."));

    m_buttonOk = new wxButton(wnd, wxID_OK, _("OK"));
    wxButton* cancel = new wxButton(wnd, wxID_CANCEL, _("Cancel"));
    // TODO: online help. Help button doesn't work yet, so commented out.
    // wxButton* help = new wxButton( wnd, wxID_HELP, _("Help"));

    // place controls
    // URL row
    wxStaticBoxSizer *urlSizer =
      new wxStaticBoxSizer(urlBox, wxHORIZONTAL);
    urlSizer->Add(m_comboSrcPath, 1, wxALL | wxEXPAND, 5);

    // Destination row
    wxStaticBoxSizer *destSizer =
      new wxStaticBoxSizer(destBox, wxHORIZONTAL);
    destSizer->Add(m_textDestPath, 1, wxALL | wxEXPAND, 5);
    destSizer->Add(browse, 0, wxALL, 5);

    // Revision row
    wxBoxSizer *reSizer = new wxBoxSizer(wxHORIZONTAL);
    wxStaticBoxSizer *revisionSizer =
      new wxStaticBoxSizer(revisionBox, wxHORIZONTAL);
    revisionSizer->Add(m_textRevision, 1, wxALL | wxEXPAND, 5);
    revisionSizer->Add(m_checkUseLatest, 0,
                       wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
    reSizer->Add(revisionSizer, 1, wxALL | wxEXPAND, 0);

    wxBoxSizer *preSizer = new wxBoxSizer(wxHORIZONTAL);
    // Peg revision row
    wxStaticBoxSizer *pegRevisionSizer =
      new wxStaticBoxSizer(pegRevisionBox, wxHORIZONTAL);
    pegRevisionSizer->Add(m_textPegRevision, 1, wxALL | wxEXPAND, 5);
    pegRevisionSizer->Add(m_checkNotSpecified, 0,
                          wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
    preSizer->Add(pegRevisionSizer, 1, wxALL | wxEXPAND, 0);

    // the native eol combo
    wxBoxSizer *nativeEolSizer = new wxBoxSizer(wxHORIZONTAL);
    nativeEolSizer->Add(labelEol, 1, wxALL | wxEXPAND, 5);
    nativeEolSizer->Add(m_comboNativeEol, 1, wxALL | wxEXPAND, 5);

    // Button row
    wxBoxSizer *buttonSizer  = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(m_buttonOk, 0, wxALL, 10);
    buttonSizer->Add(cancel, 0, wxALL, 10);
    // Add explicit context-sensitive help button for non-MSW
    // TODO: Online Help
    // buttonSizer->Add(help, 0, wxALL, 10);
#ifndef __WXMSW__
    buttonSizer->Add(new wxContextHelpButton(wnd), 0, wxALL, 10);
#endif

    // Extras sizer
    wxBoxSizer *extrasSizer = new wxBoxSizer(wxHORIZONTAL);
    extrasSizer->Add(recursive, 0, wxALL | wxCENTER, 5);
    extrasSizer->Add(overwrite, 0, wxALL | wxCENTER, 5);
    extrasSizer->Add(ignoreExternals, 0, wxALL | wxCENTER, 5);

    // Add all sizers to main sizer
    wxBoxSizer *mainSizer    = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(urlSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(destSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(reSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(preSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(nativeEolSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add(extrasSizer, 0, wxALL | wxCENTER, 5);
    mainSizer->Add(buttonSizer, 0, wxALL | wxCENTER, 5);

    wnd->SetAutoLayout(true);
    wnd->SetSizer(mainSizer);

    mainSizer->SetSizeHints(wnd);
    mainSizer->Fit(wnd);
  }

  void
  CheckControls()
  {
    bool useLatest = m_checkUseLatest->IsChecked();
    bool notSpecified = m_checkNotSpecified->IsChecked();

    m_textRevision->Enable(!useLatest);
    m_textPegRevision->Enable(!notSpecified);

    bool ok = true;
    if (!useLatest)
    {
      ok = CheckRevision(m_textRevision->GetValue());
    }

    if (!notSpecified)
    {
      ok = CheckRevision(m_textPegRevision->GetValue());
    }

    if (m_textDestPath->GetValue().Length() <= 0)
    {
      ok = false;
    }

    if (m_comboSrcPath->GetValue().Length() <= 0)
    {
      ok = false;
    }

    if (m_comboNativeEol->GetValue().Length() <= 0)
    {
      ok = false;
    }
    else
    {
      wxString textEol = m_comboNativeEol->GetValue();
      if (textEol == wxT("CRLF (Windows)"))
        data.Eol = "CRLF";
      else if (textEol == wxT("LF (Unix)"))
        data.Eol = "LF";
      else if (textEol == wxT("CR (MacOS)"))
        data.Eol = "CR";
      else
        data.Eol = NULL;
    }
    m_buttonOk->Enable(ok);
    if (ok)
    {
      m_buttonOk->SetDefault();
    }
  }
};

BEGIN_EVENT_TABLE(ExportDlg, wxDialog)
  EVT_BUTTON(ID_BUTTON_BROWSE, ExportDlg::OnBrowse)
  EVT_CHECKBOX(ID_USE_LATEST, ExportDlg::OnUseLatest)
  EVT_CHECKBOX(ID_NOT_SPECIFIED, ExportDlg::OnNotSpecified)
  EVT_COMBOBOX(ID_NATIVE_EOL, ExportDlg::OnNativeEol)
  EVT_TEXT(-1, ExportDlg::OnText)
  //  EVT_BUTTON (wxID_HELP, ExportDlg::OnHelp)
END_EVENT_TABLE()

ExportDlg::ExportDlg(wxWindow * parent, const svn::Path & selectedUrl)
{
  // Add the context-sensitive help button on the caption for MSW
#ifdef __WXMSW__
  SetExtraStyle(wxDIALOG_EX_CONTEXTHELP);
#endif
  Create(parent, -1, _("Export"), wxDefaultPosition,
         wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

  m = new Data(this);
  CentreOnParent();

  m->data.SrcPath = Utf8ToLocal(selectedUrl.c_str());
}

ExportDlg::~ExportDlg()
{
  delete m;
}

/**
 * Brings up a directory dialog defaulted to the user's home directory.
 */
void
ExportDlg::OnBrowse(wxCommandEvent & event)
{
  // Transfer data from controls into m_pData:
  TransferDataFromWindow();
  wxDirDialog dialog(this,
                     _("Select a destination directory"),
                     wxGetHomeDir(), wxDD_NEW_DIR_BUTTON);

  if (dialog.ShowModal() == wxID_OK)
  {
    m->data.DestPath = dialog.GetPath();
    // Transfer data from m_pData back into controls:
    TransferDataToWindow();
  }
}

void
ExportDlg::InitDialog()
{
  wxDialog::InitDialog();
  m->CheckControls();
}

void
ExportDlg::OnUseLatest(wxCommandEvent &)
{
  m->CheckControls();
}

void
ExportDlg::OnNotSpecified(wxCommandEvent &)
{
  m->CheckControls();
}

void
ExportDlg::OnNativeEol(wxCommandEvent &)
{
  m->CheckControls();
}

const ExportData &
ExportDlg::GetData() const
{
  return m->data;
}

void
ExportDlg::OnText(wxCommandEvent &)
{
  m->CheckControls();
}

void
ExportDlg::OnHelp(wxCommandEvent & event)
{
  // @todo Has to be re-integrated for Online Help
  // ::wxGetApp ().GetHelpController().Display(wxT("Export dialog"));
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
