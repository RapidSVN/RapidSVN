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
#include "auth_dlg.hpp"

BEGIN_EVENT_TABLE (AuthDlg, wxDialog)
END_EVENT_TABLE ()

struct AuthDlg::Data
{
public:
  wxString User;
  wxString Password;

  Data (wxWindow * window)
  {
    // create controls first

    // authentication fields
    wxStaticText * labelUser = 
      new wxStaticText (window, -1, _("User"));

    wxTextCtrl* textUser = new wxTextCtrl (
      window, -1, "", wxDefaultPosition, wxDefaultSize, 0,
      wxTextValidator (wxFILTER_NONE, &User));

    wxStaticText * labelPassword =
      new wxStaticText (window, -1, _("Password"));

    wxTextCtrl* textPassword = new wxTextCtrl (
      window, -1, "", wxPoint(-1,-1), 
      wxDefaultSize, wxTE_PASSWORD, 
      wxTextValidator (wxFILTER_NONE, &Password));

    // buttons
    wxButton * buttonOk = new wxButton (window, wxID_OK, 
                                        _("OK"));
    wxButton * buttonCancel = new wxButton (this, wxID_CANCEL, 
                                            _("Cancel"));

    // create sizers and position controls
    wxFlexGridSizer *authSizer = new wxFlexSizer(2, 2, 5, 5);
    authSizer->Add (labelUser, 0, 
                   wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
    
    authSizer->Add (textUser, 1, 
                    wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);

    authSizer->Add (labelPassword, 0,
                   wxLEFT | wxALIGN_CENTER_VERTICAL, 5);  

    authSizer->Add (textPassword, 1, 
                   wxALL | wxALIGN_CENTER_VERTICAL, 5);
  
    // The buttons:
    wxBoxSizer *buttonSizer = new wxBoxSizer (wxHORIZONTAL);
    buttonSizer->Add (buttonOk, 0, wxALL, 10);
    buttonSizer->Add (buttonCancel, 0, wxALL, 10);

    // Add all the sizers to the main sizer
    wxBoxSizer *mainSizer = new wxBoxSizer (wxVERTICAL);
    mainSizer->Add (authSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

    SetAutoLayout (true);
    SetSizer (mainSizer);

    mainSizer->SetSizeHints (window);
    mainSizer->Fit (window);
  }
  
};

AuthDlg::AuthDlg (wxWindow* parent, const wxString & username,
                  const wxString & password)
: wxDialog (parent, -1, _("Authentication"),
            wxDefaultPosition, wxDefaultSize,
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  new m (this);
  CentreOnParent ();
}

AuthDlg::~AuthDlg ()
{
  delete m;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
