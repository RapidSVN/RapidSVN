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
#include "mkdir_dlg.hpp"


struct MkdirDlg::Data
{
public:
  wxWindow * window;
  wxString target;

  Data (wxWindow * wnd, const char * trget)
    : window (wnd), target (trget)
  {
    Initialize ();
  }

private:
  void
  Initialize ()
  {
    // Create controls
    // The target field:
    wxStaticBox * boxTarget = 
      new wxStaticBox (window, -1, 
                      _("New directory or repository URL"));
    
    wxTextValidator val (wxFILTER_NONE, &target);
    wxTextCtrl* textTarget = 
      new wxTextCtrl(window, -1, "", wxDefaultPosition, 
                     wxDefaultSize, 0,                   val);

    // The buttons:
    wxButton * buttonOK = 
      new wxButton( window, wxID_OK, _("OK" ));
    wxButton * buttonCancel = 
      new wxButton( window, wxID_CANCEL, _("Cancel"));

    // Position controls
    wxStaticBoxSizer *sizerTarget = 
      new wxStaticBoxSizer (boxTarget, wxHORIZONTAL);
    sizerTarget->Add(textTarget, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer *targetouterSizer = new wxBoxSizer(wxHORIZONTAL);
    targetouterSizer->Add(sizerTarget, 1, wxALL | wxEXPAND, 5);

    wxBoxSizer *buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(buttonOK, 0, wxALL, 10);
    buttonSizer->Add(buttonCancel, 0, wxALL, 10);


    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
  
    // Add all the sizers to the main sizer
    mainSizer->Add (targetouterSizer, 0, wxLEFT | wxRIGHT | wxEXPAND, 5);
    mainSizer->Add (buttonSizer, 0, wxLEFT | wxRIGHT | wxCENTER, 5);

    window->SetAutoLayout(true);
    window->SetSizer(mainSizer);

    mainSizer->SetSizeHints(window);
    mainSizer->Fit(window);

  }
};

MkdirDlg::MkdirDlg(wxWindow* parent, const char * target)
: wxDialog(parent, -1, _("Make directory"),
  wxDefaultPosition, wxDefaultSize,
    wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (this, target);

  CentreOnParent();
}

MkdirDlg::~MkdirDlg ()
{
  delete m;
}

const char *
MkdirDlg::GetTarget () const
{
  return m->target.c_str ();
}

BEGIN_EVENT_TABLE (MkdirDlg, wxDialog)
END_EVENT_TABLE ()

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
