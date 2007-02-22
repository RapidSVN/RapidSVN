/*
 * ====================================================================
 * Copyright (c) 2002-2007 The RapidSvn Group.  All rights reserved.
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
#include "wx/cshelp.h"
#include "wx/sizer.h"
#include "wx/valgen.h"

// svncpp
#include "svncpp/path.hpp"

// app
#include "hist_entries.hpp"
#include "hist_val.hpp"
#include "rapidsvn_app.hpp"
#include "checkout_dlg.hpp"
#include "utils.hpp"

enum
{
  ID_USELATEST = 100,
  ID_NOTSPECIFIED = 100,
  ID_BUTTON_BROWSE
};

struct CheckoutDlg::Data
{
private:
  wxCheckBox * m_checkUseLatest;
  wxCheckBox * m_checkNotSpecified;
  wxTextCtrl * m_textRevision;
  wxTextCtrl * m_textPegRevision;
  wxComboBox * m_comboDest;
  wxComboBox * m_comboRepUrl;
  wxButton * m_buttonOk;

public:
  CheckoutData data;

  Data (wxWindow * wnd)
  {
    // create controls
    wxStaticBox * urlBox =
      new wxStaticBox (wnd, 0, _("URL"));
    HistoryValidator valModule (HISTORY_REPOSITORY, &data.RepUrl);
    m_comboRepUrl =
      new wxComboBox (wnd, -1, wxEmptyString, wxDefaultPosition,
                      wxSize (235, -1), 0, 0, wxCB_DROPDOWN, valModule);
    m_comboRepUrl->SetHelpText (_("Enter the repository URL (not local path) here."));
    wxStaticBox * destBox =
      new wxStaticBox (wnd, 0, _("Destination Directory"));
    HistoryValidator valWorkingDir (HISTORY_WORKING_DIRECTORY, &data.DestFolder);
    m_comboDest =
      new wxComboBox (wnd, -1, wxEmptyString, wxDefaultPosition,
                      wxSize(205, -1), 0, 0, wxCB_DROPDOWN, valWorkingDir);
    m_comboDest->SetHelpText (_("Enter the local path where you want the code checked out to here."));
    wxButton * browse =
      new wxButton (wnd, ID_BUTTON_BROWSE, wxT("..."),
                   wxDefaultPosition, wxSize(20, -1) );

    wxStaticBox * revisionBox =
      new wxStaticBox (wnd, -1, _("Revision"));
    wxTextValidator valRevision (wxFILTER_NUMERIC, &data.Revision);
    m_textRevision =
      new wxTextCtrl (wnd, -1, wxEmptyString, wxDefaultPosition,
                      wxSize(50, -1), 0, valRevision);
    m_textRevision->SetHelpText (_("If not using the latest version of the files, specify which revision to use here."));

    wxGenericValidator valLatest (&data.UseLatest);
    m_checkUseLatest =
      new wxCheckBox (wnd, ID_USELATEST, _("Use latest"),
                     wxDefaultPosition, wxDefaultSize, 0, valLatest);
    m_checkUseLatest->SetHelpText (_("Set this to get the latest version of the files in the repository."));

    wxStaticBox * pegRevisionBox =
      new wxStaticBox (wnd, -1, _("Peg Revision"));
    wxTextValidator valPegRevision (wxFILTER_NUMERIC, &data.PegRevision);
    m_textPegRevision =
      new wxTextCtrl (wnd, -1, wxEmptyString, wxDefaultPosition,
                      wxSize(50, -1), 0, valPegRevision);
    m_textPegRevision->SetHelpText (_("If the files were renamed or moved some time, specify which peg revision to use here."));

    wxGenericValidator valNotSpecified (&data.NotSpecified);
    m_checkNotSpecified =
      new wxCheckBox (wnd, ID_NOTSPECIFIED, _("Not specified"),
                      wxDefaultPosition, wxDefaultSize, 0, valNotSpecified);
    m_checkNotSpecified->SetHelpText (_("Set this to use BASE/HEAD (current) peg revision of the files."));

    wxCheckBox * recursive =
      new wxCheckBox (wnd, -1, _("Recursive"),
                      wxDefaultPosition, wxDefaultSize, 0,
                      wxGenericValidator(&data.Recursive));
    recursive->SetHelpText (_("Set to get all subdirectories from the URL also."));
    wxCheckBox * bookmarks =
      new wxCheckBox (wnd, -1, _("Add to bookmarks"),
                      wxDefaultPosition, wxDefaultSize, 0,
                      wxGenericValidator(&data.Bookmarks));
    bookmarks->SetHelpText (_("Set to automatically create a new working copy bookmark."));

    wxCheckBox * ignoreExternals =
      new wxCheckBox (wnd, -1, _("Ignore externals"),
                      wxDefaultPosition, wxDefaultSize, 0,
                      wxGenericValidator(&data.IgnoreExternals));
    ignoreExternals->SetHelpText (_("Set to ignore external definitions and the external working copies managed by them."));
    m_buttonOk = new wxButton (wnd, wxID_OK, _("OK" ));
    wxButton * cancel = new wxButton( wnd, wxID_CANCEL, _("Cancel"));
    // TODO: online help. Help button doesn't work yet, so commented out.
    // wxButton* help = new wxButton( wnd, wxID_HELP, _("Help"));

    // place controls
    // URL row
    wxStaticBoxSizer * urlSizer =
      new wxStaticBoxSizer (urlBox, wxHORIZONTAL);
    urlSizer->Add (m_comboRepUrl, 1, wxALL | wxEXPAND, 5);

    // Destination row
    wxStaticBoxSizer * destSizer =
      new wxStaticBoxSizer (destBox, wxHORIZONTAL);
    destSizer->Add (m_comboDest, 1, wxALL | wxEXPAND, 5);
    destSizer->Add (browse, 0, wxALL, 5);

    // Revision row
    wxBoxSizer * reSizer = new wxBoxSizer (wxHORIZONTAL);
    wxStaticBoxSizer * revisionSizer =
      new wxStaticBoxSizer (revisionBox, wxHORIZONTAL);
    revisionSizer->Add (m_textRevision, 1, wxALL | wxEXPAND, 5);
    revisionSizer->Add (m_checkUseLatest, 0,
                        wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
    reSizer->Add (revisionSizer, 1, wxALL | wxEXPAND, 0);

    wxBoxSizer * preSizer = new wxBoxSizer (wxHORIZONTAL);
    // Peg revision row
    wxStaticBoxSizer * pegRevisionSizer =
      new wxStaticBoxSizer (pegRevisionBox, wxHORIZONTAL);
    pegRevisionSizer->Add (m_textPegRevision, 1, wxALL | wxEXPAND, 5);
    pegRevisionSizer->Add (m_checkNotSpecified, 0,
                           wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
    preSizer->Add (pegRevisionSizer, 1, wxALL | wxEXPAND, 0);

    // Button row
    wxBoxSizer * buttonSizer  = new wxBoxSizer (wxHORIZONTAL);
    buttonSizer->Add (m_buttonOk, 0, wxALL, 10);
    buttonSizer->Add (cancel, 0, wxALL, 10);
    // Online Help to be done later
//  buttonSizer->Add(help, 0, wxALL, 10);

    // Add explicit context-sensitive help button for non-MSW
#ifndef __WXMSW__
    buttonSizer->Add (new wxContextHelpButton(wnd), 0, wxALL, 10);
#endif

    // Extras sizer
    wxBoxSizer * extrasSizer = new wxBoxSizer (wxHORIZONTAL);
    extrasSizer->Add (bookmarks, 0, wxALL | wxCENTER, 5);
    extrasSizer->Add (recursive, 0, wxALL | wxCENTER, 5);
    extrasSizer->Add (ignoreExternals, 0, wxALL | wxCENTER, 5);

    // Add all sizers to main sizer
    wxBoxSizer * mainSizer = new wxBoxSizer (wxVERTICAL);
    mainSizer->Add (urlSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add (destSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add (reSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add (preSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add (extrasSizer, 0, wxALL | wxCENTER, 5);
    mainSizer->Add (buttonSizer, 0, wxALL | wxCENTER, 5);

    wnd->SetAutoLayout (true);
    wnd->SetSizer (mainSizer);

    mainSizer->SetSizeHints (wnd);
    mainSizer->Fit (wnd);

    m_buttonOk->SetDefault ();
  }

  void
  CheckControls()
  {
    bool useLatest = m_checkUseLatest->IsChecked ();
    bool notSpecified = m_checkNotSpecified->IsChecked ();

    m_textRevision->Enable (!useLatest);
    m_textPegRevision->Enable (!notSpecified);

    bool ok = true;
    if (!useLatest)
    {
      ok = CheckRevision (m_textRevision->GetValue ());
    }

    if (!notSpecified)
    {
      ok = CheckRevision (m_textPegRevision->GetValue ());
    }

    if (m_comboDest->GetValue ().Length () <= 0)
    {
      ok = false;
    }

    if (m_comboRepUrl->GetValue ().Length () <= 0)
    {
      ok = false;
    }

    m_buttonOk->Enable (ok);

    if (ok)
    {
      m_buttonOk->SetDefault ();
    }
  }
};

BEGIN_EVENT_TABLE (CheckoutDlg, wxDialog)
  EVT_BUTTON (ID_BUTTON_BROWSE, CheckoutDlg::OnBrowse)
  EVT_CHECKBOX (ID_USELATEST, CheckoutDlg::OnUseLatest)
  EVT_TEXT (-1, CheckoutDlg::OnText)
//  EVT_BUTTON (wxID_HELP, CheckoutDlg::OnHelp)
END_EVENT_TABLE ()

CheckoutDlg::CheckoutDlg (wxWindow * parent, 
                          const svn::Path & selectedUrl)
{
    // Add the context-sensitive help button on the caption for MSW
#ifdef __WXMSW__
  SetExtraStyle (wxDIALOG_EX_CONTEXTHELP);
#endif
  Create (parent, -1, _("Checkout"), wxDefaultPosition,
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

  m = new Data (this);
  CentreOnParent();

  m->data.RepUrl = Utf8ToLocal (selectedUrl.c_str ());
}

CheckoutDlg::~CheckoutDlg ()
{
  delete m;
}

/**
 * Brings up a directory dialog defaulted to the user's home directory.
 */
void
CheckoutDlg::OnBrowse (wxCommandEvent & event)
{
  // Transfer data from controls into m_pData:
  TransferDataFromWindow ();
  wxDirDialog dialog (this,
                      _("Select a destination directory"),
                      wxGetHomeDir (), wxDD_NEW_DIR_BUTTON);

  if (dialog.ShowModal () == wxID_OK)
  {
    m->data.DestFolder = dialog.GetPath ();
    // Transfer data from m_pData back into controls:
    TransferDataToWindow();
    m->CheckControls ();
  }
}

void
CheckoutDlg::InitDialog ()
{
  wxDialog::InitDialog();
  m->CheckControls();
}

void
CheckoutDlg::OnUseLatest (wxCommandEvent &)
{
  m->CheckControls();
}

const CheckoutData &
CheckoutDlg::GetData () const
{
  return m->data;
}

void
CheckoutDlg::OnText (wxCommandEvent &)
{
  m->CheckControls ();
}

void
CheckoutDlg::OnHelp (wxCommandEvent &)
{
  // @todo Has to be re-integrated for Online Help
  // ::wxGetApp ().GetHelpController().Display(wxT("Checkout dialog"));
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
