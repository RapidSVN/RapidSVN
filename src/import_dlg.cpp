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
#include "import_data.hpp"
#include "import_dlg.hpp"
#include "utils.hpp"

enum
{
  ID_BUTTON_BROWSE = 100,
};

struct ImportDlg::Data
{
public:
  ImportData data;

  Data (wxWindow * window)
  {
    data.TreeType = true;
    data.FileType = !data.TreeType;
    data.Recursive = true;

    InitControls (window);
  }

  void
  InitControls (wxWindow * wnd)
  {
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    wxFlexGridSizer* Grid = new wxFlexGridSizer(2, 3, 5, 0);
    Grid->AddGrowableCol(1);  // The second column can be expanded.

    // Row 0:  
    Grid->Add(new wxStaticText(wnd, -1, _("Repository URL")), 0, 
              wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *Repository = new wxTextCtrl(wnd, -1, "",
                                            wxDefaultPosition, wxSize(300, -1), 0,
                                            wxTextValidator(wxFILTER_NONE, &data.Repository));
    Grid->Add(Repository, 1, wxLEFT | wxEXPAND | wxALIGN_CENTER_VERTICAL, 5);

    Grid->Add(new wxStaticText(wnd, -1, ""), 0, 
              0, 5);

    // Row 1:  
    Grid->Add(new wxStaticText(wnd, -1, _("Path")), 0, 
              wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
    wxTextCtrl *Path = new wxTextCtrl(wnd, -1, "",
                                      wxDefaultPosition, wxSize(300, -1), 0,
                                      wxTextValidator(wxFILTER_NONE, &data.Path));
    Grid->Add(Path, 1, wxLEFT | wxEXPAND, 5);
    wxButton* BrowseButton = new wxButton(wnd, ID_BUTTON_BROWSE, "...", 
                                          wxPoint(-1,-1), wxSize(20, -1));
    Grid->Add(BrowseButton, 0, wxLEFT | wxALIGN_CENTER_VERTICAL, 5);
  
    mainSizer->Add(Grid, 0, wxALL | wxEXPAND, 5);

  
    // The message field:
    wxStaticBoxSizer *messageSizer = new wxStaticBoxSizer(
      new wxStaticBox(wnd, -1, _("Enter log message")), wxHORIZONTAL);
    
    wxTextCtrl* Log = new wxTextCtrl(wnd, -1, "", wxDefaultPosition, 
                                     wxSize(-1, 50), wxTE_MULTILINE,
                                     wxTextValidator(wxFILTER_NONE, &data.LogMessage));
    
    messageSizer->Add(Log, 1, wxALL | wxEXPAND, 5);
  
    mainSizer->Add(messageSizer, 1, wxALL | wxEXPAND, 5);

    // Sundry items row:
    wxBoxSizer *SundrySizer = new wxBoxSizer(wxHORIZONTAL);
  
    wxCheckBox* Recursive = new wxCheckBox (wnd, -1, _("Recursive"),
                                            wxDefaultPosition, wxDefaultSize, 0, wxGenericValidator(&data.Recursive));
    SundrySizer->Add(Recursive, 0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
  
    SundrySizer->Add(new wxStaticText(wnd, -1, _("Path type:")), 0, 
                     wxLEFT | wxALIGN_CENTER_VERTICAL, 30);

    SundrySizer->Add(
      new wxRadioButton(wnd, -1, _("Tree"), wxDefaultPosition, wxDefaultSize, 0,
                        wxGenericValidator(&data.TreeType)),
      0, wxALL | wxALIGN_CENTER_VERTICAL, 5);
    SundrySizer->Add(
      new wxRadioButton(wnd, -1, _("File"), wxDefaultPosition, wxDefaultSize, 0,
                        wxGenericValidator(&data.FileType)),
      0, wxALL | wxALIGN_CENTER_VERTICAL, 5);

    mainSizer->Add(SundrySizer, 0, wxALL | wxCENTER, 5);

    // Button row
    wxBoxSizer *ButtonSizer = new wxBoxSizer(wxHORIZONTAL);
    ButtonSizer->Add(new wxButton(wnd, wxID_OK, _("OK" )), 0, 
                     wxALL, 10);
    ButtonSizer->Add(new wxButton(wnd, wxID_CANCEL, _("Cancel")), 0, 
                     wxALL, 10);

    mainSizer->Add(ButtonSizer, 0, wxALL | wxCENTER, 5);

    wnd->SetAutoLayout(true);
    wnd->SetSizer(mainSizer);

    mainSizer->SetSizeHints(wnd);
    mainSizer->Fit(wnd);
  }
};

BEGIN_EVENT_TABLE (ImportDlg, wxDialog)
  EVT_BUTTON (wxID_OK, ImportDlg::OnOk)
  EVT_BUTTON (ID_BUTTON_BROWSE, ImportDlg::OnBrowse)
END_EVENT_TABLE ()

ImportDlg::ImportDlg (wxWindow * parent)
  : wxDialog (parent, -1, _("Import"), 
              wxDefaultPosition, 
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (this);
  CentreOnParent();
}

ImportDlg::~ImportDlg ()
{
  delete m;
}

void
ImportDlg::OnOk (wxCommandEvent &event)
{
  wxString val;

    // Transfer data from controls into m_data:
  TransferDataFromWindow();

  TrimString (m->data.Repository);
  TrimString (m->data.Path);
  TrimString (m->data.LogMessage);
  
  if (m->data.Repository.IsEmpty ())
  {
    wxMessageBox (_("Repository URL is required for import!"),
                  _("Error"), wxOK | wxCENTRE | wxICON_ERROR);

    // Do not allow the user to continue if the path is empty 
    // and the import is addressing a file.
    return;
  }

  if (m->data.FileType)
  {

    if (m->data.Path.IsEmpty ())
    {
      wxMessageBox (_("File path required when importing a file!"),
                    _("Error"), wxOK | wxCENTRE | wxICON_ERROR);

      // Do not allow the user to continue if the reposURL is empty.
      return;
    }
  }

  wxDialog::OnOK(event);
}

void
ImportDlg::OnBrowse (wxCommandEvent & WXUNUSED (event))
{
  // Transfer data from controls into m->data:
  TransferDataFromWindow();
  
  if (m->data.TreeType)
  {
    wxDirDialog dialog (this,
                        _("Select a directory to import"),
                        m->data.Path);

    if (dialog.ShowModal () == wxID_OK)
      m->data.Path = dialog.GetPath ();
  }
  else                          // it's a file 
  {
    wxFileDialog dialog (this,
                         _("Select a file to import"),
                         m->data.Path);

    if (dialog.ShowModal () == wxID_OK)
      m->data.Path = dialog.GetPath ();
  }
  // Transfer data from m->data back into controls:
  TransferDataToWindow();
}

ImportData &
ImportDlg::GetData ()
{
  return m->data;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
