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
#include "wx/sizer.h"
#include <wx/valgen.h>

// app
#include "diff_dlg.hpp"
#include "utils.hpp"

enum
{
  ID_USELATEST = 100,
  ID_BUTTON_BROWSE,
};

struct DiffDlg::Data
{
private:

public:
  Data (wxWindow * wnd)
  {
    // Add all sizers to main sizer
    wxBoxSizer *mainSizer = new wxBoxSizer (wxVERTICAL);

    wnd->SetAutoLayout(true);
    wnd->SetSizer(mainSizer);

    mainSizer->SetSizeHints(wnd);
    mainSizer->Fit(wnd);
  }

  void 
  CheckControls()
  {
  }


};

BEGIN_EVENT_TABLE (DiffDlg, wxDialog)
  EVT_BUTTON (ID_BUTTON_BROWSE, DiffDlg::OnBrowse)
  EVT_BUTTON (wxID_OK, DiffDlg::OnOK)
  EVT_CHECKBOX (ID_USELATEST, DiffDlg::OnUseLatest)
  EVT_TEXT (-1, DiffDlg::OnText)
END_EVENT_TABLE ()

DiffDlg::DiffDlg (wxWindow * parent)
  : wxDialog (parent, -1, _("Diff"), wxDefaultPosition, 
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (this);
  CentreOnParent();
}

DiffDlg::~DiffDlg ()
{
  delete m;
}

void
DiffDlg::OnBrowse (wxCommandEvent & event)
{
}

void 
DiffDlg::OnUseLatest(wxCommandEvent &)
{
}

void
DiffDlg::OnText (wxCommandEvent &)
{
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
