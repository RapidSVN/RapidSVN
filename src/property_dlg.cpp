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
#include "wx/listctrl.h"
#include "wx/valgen.h"

// svncpp
#include "svncpp/exception.hpp"

// app
#include "property_dlg.hpp"

enum
{
  ID_OK = 1,
  ID_Cancel,
  PROPERTY_GRID
};

BEGIN_EVENT_TABLE (PropertyDlg, wxDialog)
  EVT_BUTTON (ID_OK, PropertyDlg::OnOK)
  EVT_BUTTON (ID_Cancel, PropertyDlg::OnClose)
END_EVENT_TABLE ()

BEGIN_EVENT_TABLE (PropertyGrid, wxGrid)
END_EVENT_TABLE()

PropertyDlg::PropertyDlg (wxWindow * parent, const char * target)
           : wxDialog (parent, -1, _T("Property Editor"), wxDefaultPosition, 
             wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
             m_property(NULL, target)
{
  InitializeData ();
  CentreOnParent ();
}

void
PropertyDlg::InitializeData ()
{
  // create controls
  wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer (
        new wxStaticBox(this, -1, _T("Properties")), 
        wxHORIZONTAL);

  propGrid = new PropertyGrid (this, m_property);
  propGrid->CreateGrid (1, 2);
  propGrid->SetGridLineColour (wxColour ("black"));

  propGrid->SetColLabelValue (0, _T("Name"));
  propGrid->SetColLabelValue (1, _T("Value"));
  propGrid->SetColSize (0, 120);
  propGrid->SetColSize (1, 120);
  propGrid->FillList ();
  propGrid->Show ();

  wxBoxSizer * buttonSizer = new wxBoxSizer (wxHORIZONTAL);
  wxButton * okButton = new wxButton (this, ID_OK, _T("OK"));
  wxButton * cancelButton = new wxButton (this, ID_Cancel, _T("Cancel"));
  buttonSizer->Add (okButton, 0, wxALL, 5);
  buttonSizer->Add (cancelButton, 0, wxALL, 5);

  wxBoxSizer * mainSizer = new wxBoxSizer (wxVERTICAL);

  boxSizer->Add (propGrid, 1, wxALL | wxEXPAND, 2);

  mainSizer->Add (boxSizer, 1, wxALL | wxCENTER | wxEXPAND , 5);
  mainSizer->Add (buttonSizer, 0, wxALL | wxCENTER , 5);

  SetAutoLayout (true);
  SetSizer (mainSizer);

  mainSizer->SetSizeHints (this);
  mainSizer->Fit (this);
}

void
PropertyGrid::FillList ()
{
  int index = 0;
  const std::vector<svn::PropertyEntry> & entries
    = m_property.entries ();
  std::vector<svn::PropertyEntry>::const_iterator it = entries.begin ();

  DeleteRows ();
  AppendRows (entries.size () + 1);
  while (it != entries.end ())
  {
    const svn::PropertyEntry & entry = *it;
  
    SetCellValue (index, 0, entry.name.c_str ());
    SetCellValue (index, 1, entry.value.c_str ());

    index++;
    it++;
  }
}

void
PropertyDlg::OnClose (wxCommandEvent & event)
{
  Close (TRUE);
}

void
PropertyDlg::OnOK (wxCommandEvent & event)
{
  SaveData ();
  RemoveData ();
  Close (TRUE);
}

void
PropertyDlg::SaveData ()
{
  wxString name, value;

  for(int i = 0; i < propGrid->GetNumberRows () - 1; i++)
  {
    name = propGrid->GetCellValue (i, 0);
    value = propGrid->GetCellValue (i, 1);
    name.Trim ();
    value.Trim ();

    if(name.Length () > 0 && value.Length () > 0)
    {
      try
      {
        m_property.set (name.c_str (), value.c_str ());
      }
      catch (svn::ClientException &e)
      {
        wxString msg = "Failed to save property: ";
        msg += name;
        msg += ", error: ";
        msg += e.description ();
        wxMessageDialog dlg (this, msg, "Error Saving", wxOK);
        dlg.ShowModal ();
      }
    }
  }
}

void
PropertyDlg::RemoveData ()
{
  wxString propValue, cellValue;
  bool found;
  const std::vector<svn::PropertyEntry> & entries = m_property.entries ();
  std::vector<svn::PropertyEntry>::const_iterator it = entries.begin ();

  while (it != entries.end ())
  {
    const svn::PropertyEntry & entry = *it;
    propValue = entry.name.c_str();
    found = false;

    for(int i = 0; i < propGrid->GetNumberRows () - 1; i++)
    {
      cellValue = propGrid->GetCellValue (i, 0);
      cellValue.Trim ();
      if(propValue == cellValue)
        found = true;
    }

    if(!found)
    {
      try
      {
        m_property.remove (propValue.c_str ());
      }
      catch (svn::ClientException &e)
      {
        wxString msg = "Failed to remove property: ";
        msg += propValue;
        msg += ", error: ";
        msg += e.description ();
        wxMessageDialog dlg (this, msg, "Error Removing", wxOK);
        dlg.ShowModal ();
      }
    }

    it++;
  }
}

PropertyGrid::PropertyGrid (wxWindow * parent, const svn::Property & property)
           : wxGrid (parent, PROPERTY_GRID, wxDefaultPosition, 
             wxSize(350, 150), wxLC_REPORT),
             m_property(property)
{
  InitializeData ();
  CentreOnParent ();
}

void
PropertyGrid::InitializeData ()
{
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
