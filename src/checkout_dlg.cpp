#include "include.h"
#include "rapidsvn_app.h"
#include "checkout_dlg.h"
#include "wx/sizer.h"

BEGIN_EVENT_TABLE (CheckoutDlg, wxDialog)
EVT_BUTTON (ID_BUTTON_OK, CheckoutDlg::OnOk)
EVT_BUTTON (ID_BUTTON_CANCEL, CheckoutDlg::OnCancel)
EVT_BUTTON (ID_BUTTON_BROWSE, CheckoutDlg::OnBrowse) END_EVENT_TABLE ()

CheckoutDlg::CheckoutDlg (wxWindow * parent)
           : wxDialog (parent, -1, "Checkout module", wxDefaultPosition, 
             wxDefaultSize, wxDEFAULT_DIALOG_STYLE )
{
  InitializeData ();
}

void
CheckoutDlg::OnOk (wxCommandEvent & WXUNUSED (event))
{
  wxString val;
  unsigned long rev = 0;

  val = revision->GetValue ();
  TrimString (val);

  if (!val.ToULong (&rev, 10) && rev < 0)
  {
    // could not convert revision to a number
    wxMessageBox (_T ("Revision must be an unsigned number!"),
                  _T ("Error"), wxOK | wxCENTRE | wxICON_ERROR);
    return;
  }

  EndModal (ID_BUTTON_OK);
}

void
CheckoutDlg::OnCancel (wxCommandEvent & WXUNUSED (event))
{
  EndModal (ID_BUTTON_CANCEL);
}

/**
 * Brings up a directory dialog defaulted to the user's home directory.
 */
void
CheckoutDlg::OnBrowse (wxCommandEvent & event)
{
  wxDirDialog dialog (this,
                      _T ("Select a destination folder to checkout to"),
                      wxGetHomeDir());

  if (dialog.ShowModal () == wxID_OK)
    destFolder->SetValue (dialog.GetPath ());
}

/**
 * Set the dialog controls and layout.
 */
void
CheckoutDlg::InitializeData ()
{
  wxBoxSizer *mainSizer    = new wxBoxSizer (wxVERTICAL);
  wxBoxSizer *buttonSizer  = new wxBoxSizer (wxHORIZONTAL);

  // Module row
  wxStaticBoxSizer *moduleSizer = new wxStaticBoxSizer (
          new wxStaticBox(this, -1, _T("Module to checkout (full path)")), 
          wxHORIZONTAL);
  moduleName = new wxTextCtrl (this, -1, _T(""), wxPoint(-1,-1),
                               wxSize(235, -1));
  moduleSizer->Add (moduleName, 0, wxALL, 5);

  // Destination row
  wxStaticBoxSizer *destSizer = new wxStaticBoxSizer (
          new wxStaticBox(this, 0, _T("Destination Folder")), 
          wxHORIZONTAL);
  destFolder = new wxTextCtrl (this, -1, _T(""), wxPoint(-1, -1), 
                               wxSize(205, -1));
  destSizer->Add (destFolder, 0, wxALL, 5);
  destSizer->Add (new wxButton( this, ID_BUTTON_BROWSE, _T("..."), 
                               wxPoint(-1,-1), wxSize(20, -1) ),
                               0, wxALL, 5);

  // Re* row
  wxBoxSizer *reSizer = new wxBoxSizer (wxHORIZONTAL);
  wxStaticBoxSizer *revisionSizer = new wxStaticBoxSizer (
          new wxStaticBox(this, -1, _T("Revision")), wxHORIZONTAL);
  revision = new wxTextCtrl (this, -1, _T(""), wxPoint(-1, -1), 
                             wxSize(50, -1));
  revisionSizer->Add (revision, 0, wxALL, 0);

  recursive = new wxCheckBox (this, -1, _T("Recursive"));
  recursive->SetValue (true);

  reSizer->Add (revisionSizer, 0, wxALL, 0);
  reSizer->Add (recursive, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 10);

  // Authentication row
  wxStaticBoxSizer *authSizer = new wxStaticBoxSizer (
          new wxStaticBox(this, -1, _T("Authentication")), wxHORIZONTAL);

  authSizer->Add (new wxStaticText (this, -1, _T("User ")), 0, 
                              wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  authSizer->Add (user = new wxTextCtrl (this, -1, _T("")), 0, 
                  wxLEFT | wxALIGN_CENTER_VERTICAL, 3);

  authSizer->Add (new wxStaticText (this, -1, _T("Pass ")), 0,
                           wxLEFT | wxALIGN_CENTER_VERTICAL, 5);  
  pass = new wxTextCtrl (this, -1, _T(""), wxPoint(-1,-1), 
                         wxDefaultSize, wxTE_PASSWORD);
  authSizer->Add (pass, 0, wxRIGHT | wxALIGN_CENTER_VERTICAL, 5 );
 
  // Button row
  buttonSizer->Add(new wxButton( this, ID_BUTTON_OK, _T("OK" )), 0, 
                   wxALL, 10);
  buttonSizer->Add(new wxButton( this, ID_BUTTON_CANCEL, _T("Cancel")), 0, 
                   wxALL, 10);

  // Add all sizers to main sizer
  mainSizer->Add (moduleSizer, 0, wxALL, 5);
  mainSizer->Add (destSizer, 0, wxALL, 5);
  mainSizer->Add (reSizer, 0, wxALL, 5);
  mainSizer->Add (authSizer, 0, wxALL, 5);
  mainSizer->Add (buttonSizer, 0, wxALL | wxCENTER, 5);

  SetAutoLayout(true);
  SetSizer(mainSizer);

  mainSizer->SetSizeHints(this);
  mainSizer->Fit(this);
}
