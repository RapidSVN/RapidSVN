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

// wxwindows
#include "wx/wx.h"
#include "wx/valgen.h"
#include "wx/sizer.h"

// app
#include "auth_dlg.hpp"

BEGIN_EVENT_TABLE (AuthDlg, wxDialog)
END_EVENT_TABLE ()

struct AuthDlg::Data
{
public:
  wxString Username;
  wxString Password;

  Data (wxWindow * window, int flags)
  {
    wxStaticText * labelUser=0;
    wxTextCtrl * textUser=0;
    const bool showUser = (flags & HIDE_USERNAME) == 0;

    // create controls first

    // authentication fields
    if (showUser)
    {
      labelUser = 
        new wxStaticText (window, -1, _("User"));

      textUser = new wxTextCtrl (
        window, -1, "", wxDefaultPosition, wxDefaultSize, 0,
        wxTextValidator (wxFILTER_NONE, &Username));
    }

    wxStaticText * labelPassword =
      new wxStaticText (window, -1, _("Password"));

    wxTextCtrl* textPassword = new wxTextCtrl (
      window, -1, "", wxPoint(-1,-1), 
      wxDefaultSize, wxTE_PASSWORD, 
      wxTextValidator (wxFILTER_NONE, &Password));

    // buttons
    wxButton * buttonOk = new wxButton (window, wxID_OK, 
                                        _("OK"));
    wxButton * buttonCancel = new wxButton (window, wxID_CANCEL, 
                                            _("Cancel"));

    // create sizers and position controls
    wxFlexGridSizer *authSizer = new wxFlexGridSizer(2, 2, 5, 5);
    if (showUser)
    {
      authSizer->Add (labelUser, 0, 
                      wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
    
      authSizer->Add (textUser, 1, 
                      wxALL | wxALIGN_CENTER_VERTICAL | wxEXPAND, 5);
    }

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

    window->SetAutoLayout (true);
    window->SetSizer (mainSizer);

    mainSizer->SetSizeHints (window);
    mainSizer->Fit (window);
  }
  
};

const int AuthDlg::HIDE_USERNAME = 1;

AuthDlg::AuthDlg (wxWindow* parent, const char * username,
                  const char * password, int flags)
: wxDialog (parent, -1, _("Authentication"),
            wxDefaultPosition, wxDefaultSize,
            wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (this, flags);
  CentreOnParent ();
}

AuthDlg::~AuthDlg ()
{
  delete m;
}

const char * 
AuthDlg::GetUsername () const
{
  return m->Username.c_str ();
}

const char *
AuthDlg::GetPassword () const
{
  return m->Password.c_str ();
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
