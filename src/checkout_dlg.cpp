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
//TODO #include "include.hpp"
//TODO #include "rapidsvn_app.hpp"

// wxWidgets
#include "wx/sizer.h"
#include <wx/valgen.h>
#include "wx/cshelp.h"

// app
#include "rapidsvn_app.hpp"
#include "checkout_dlg.hpp"
#include "utils.hpp"

enum
{
  ID_USELATEST = 100,
  ID_BUTTON_BROWSE,
};

struct CheckoutDlg::Data
{
private:
  wxCheckBox * m_checkUseLatest;
  wxTextCtrl * m_textRevision;
  wxTextCtrl * m_textDest;
  wxTextCtrl * m_textRepUrl;
  wxButton * m_buttonOk;

public:
  CheckoutData data;

  Data (wxWindow * wnd)
  {
    // create controls
    wxStaticBox* urlBox =
      new wxStaticBox (wnd, 0, _("URL"));
    wxTextValidator valModule (wxFILTER_NONE, &data.RepUrl);
    m_textRepUrl =
      new wxTextCtrl (wnd, -1, wxEmptyString, wxDefaultPosition,
                      wxSize(235, -1), 0, valModule);
    m_textRepUrl->SetHelpText (_("Enter the repository URL (not local path) here."));
    wxStaticBox* destBox =
      new wxStaticBox (wnd, 0, _("Destination Directory"));
    wxTextValidator valDest (wxFILTER_NONE, &data.DestFolder);
    m_textDest =
      new wxTextCtrl (wnd, -1, wxEmptyString, wxDefaultPosition,
                      wxSize(205, -1), 0, valDest);
    m_textDest->SetHelpText (_("Enter the local path where you want the code checked out to here."));
    wxButton* browse =
      new wxButton (wnd, ID_BUTTON_BROWSE, wxT("..."),
                   wxDefaultPosition, wxSize(20, -1) );

    wxStaticBox* revisionBox =
      new wxStaticBox (wnd, -1, _("Revision"));
    wxTextValidator valRevision (wxFILTER_NUMERIC, &data.Revision);
    m_textRevision =
      new wxTextCtrl (wnd, -1, wxEmptyString, wxDefaultPosition,
                      wxSize(50, -1), 0, valRevision);
    m_textRevision->SetHelpText(_("If not using the latest version of the files, specify which revision to use here."));
    wxGenericValidator valLatest (&data.UseLatest);
    m_checkUseLatest =
      new wxCheckBox (wnd, ID_USELATEST, _("Use latest"),
                     wxDefaultPosition, wxDefaultSize, 0, valLatest);
    m_checkUseLatest->SetHelpText(_("Set this to get the latest version of the files in the repository."));
    wxCheckBox* recursive =
      new wxCheckBox (wnd, -1, _("Recursive"),
                      wxDefaultPosition, wxDefaultSize, 0,
                      wxGenericValidator(&data.Recursive));
    recursive->SetHelpText(_("Set to get all subdirectories from the URL also."));
    wxCheckBox* bookmarks =
      new wxCheckBox (wnd, -1, _("Add to bookmarks"),
                      wxDefaultPosition, wxDefaultSize, 0,
                      wxGenericValidator(&data.Bookmarks));
    bookmarks->SetHelpText(_("Set to automatically create a new working copy bookmark."));

    m_buttonOk = new wxButton( wnd, wxID_OK, _("OK" ));
    wxButton* cancel = new wxButton( wnd, wxID_CANCEL, _("Cancel"));
    wxButton* help = new wxButton( wnd, wxID_HELP, _("Help"));

    // place controls
    // URL row
    wxStaticBoxSizer *urlSizer =
      new wxStaticBoxSizer (urlBox, wxHORIZONTAL);
    urlSizer->Add (m_textRepUrl, 1, wxALL | wxEXPAND, 5);

    // Destination row
    wxStaticBoxSizer *destSizer =
      new wxStaticBoxSizer (destBox, wxHORIZONTAL);
    destSizer->Add (m_textDest, 1, wxALL | wxEXPAND, 5);
    destSizer->Add (browse, 0, wxALL, 5);

    // Revision row
    wxBoxSizer *reSizer = new wxBoxSizer (wxHORIZONTAL);
    wxStaticBoxSizer *revisionSizer =
      new wxStaticBoxSizer (revisionBox, wxHORIZONTAL);
    revisionSizer->Add (m_textRevision, 1, wxALL | wxEXPAND, 5);
    revisionSizer->Add (m_checkUseLatest, 0,
                        wxLEFT | wxRIGHT | wxALIGN_CENTER_VERTICAL, 5);
    reSizer->Add (revisionSizer, 1, wxALL | wxEXPAND, 0);

    // Button row
    wxBoxSizer *buttonSizer  = new wxBoxSizer (wxHORIZONTAL);
    buttonSizer->Add(m_buttonOk, 0, wxALL, 10);
    buttonSizer->Add(cancel, 0, wxALL, 10);
    // Add explicit context-sensitive help button for non-MSW
    buttonSizer->Add(help, 0, wxALL, 10);
#ifndef __WXMSW__
    buttonSizer->Add(new wxContextHelpButton(wnd), 0, wxALL, 10);
#endif

    // Extras sizer
    wxBoxSizer *extrasSizer = new wxBoxSizer (wxHORIZONTAL);
    extrasSizer->Add(bookmarks, 0, wxALL | wxCENTER, 5);
    extrasSizer->Add(recursive, 0, wxALL | wxCENTER, 5);

    // Add all sizers to main sizer
    wxBoxSizer *mainSizer    = new wxBoxSizer (wxVERTICAL);
    mainSizer->Add (urlSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add (destSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add (reSizer, 0, wxALL | wxEXPAND, 5);
    mainSizer->Add (extrasSizer, 0, wxALL | wxCENTER, 5);
    mainSizer->Add (buttonSizer, 0, wxALL | wxCENTER, 5);

    wnd->SetAutoLayout(true);
    wnd->SetSizer(mainSizer);

    mainSizer->SetSizeHints(wnd);
    mainSizer->Fit(wnd);
  }

  void
  CheckControls()
  {
    bool checked = m_checkUseLatest->IsChecked();
    m_textRevision->Enable (!checked);

    bool ok = true;
    if (!checked)
    {
      ok = CheckRevision (m_textRevision->GetValue ());
    }

    if (m_textDest->GetValue ().Length () <= 0)
    {
      ok = false;
    }

    if (m_textRepUrl->GetValue ().Length () <= 0)
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
  EVT_BUTTON (wxID_OK, CheckoutDlg::OnOK)
  EVT_CHECKBOX (ID_USELATEST, CheckoutDlg::OnUseLatest)
  EVT_TEXT (-1, CheckoutDlg::OnText)
  EVT_BUTTON (wxID_HELP, CheckoutDlg::OnHelp)
END_EVENT_TABLE ()

CheckoutDlg::CheckoutDlg (wxWindow * parent)
{
    // Add the context-sensitive help button on the caption for MSW
#ifdef __WXMSW__
  SetExtraStyle (wxDIALOG_EX_CONTEXTHELP);
#endif
  Create (parent, -1, _("Checkout"), wxDefaultPosition,
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

  m = new Data (this);
  CentreOnParent();
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
  TransferDataFromWindow();
  wxDirDialog dialog (this,
                      _("Select a destination directory"),
                      wxGetHomeDir(), wxDD_NEW_DIR_BUTTON);

  if (dialog.ShowModal () == wxID_OK)
  {
    m->data.DestFolder = dialog.GetPath ();
    // Transfer data from m_pData back into controls:
    TransferDataToWindow();
  }
}

void
CheckoutDlg::InitDialog()
{
  wxDialog::InitDialog();
  m->CheckControls();
}

void
CheckoutDlg::OnUseLatest(wxCommandEvent &)
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
CheckoutDlg::OnHelp (wxCommandEvent & event)
{
  ::wxGetApp ().GetHelpController().Display(wxT("Checkout dialog"));
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
