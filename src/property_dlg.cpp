/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
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
#include <wx/valgen.h>

// svncpp
#include "svncpp/exception.hpp"

// app
#include "rapidsvn_app.hpp"
#include "property_dlg.hpp"

enum
{
  ID_OK = 1,
  ID_Cancel,
  PROPERTY_GRID
};

BEGIN_EVENT_TABLE (PropertyDlg, wxDialog)
  EVT_BUTTON    (ID_OK,    PropertyDlg::OnOK)
  EVT_BUTTON    (ID_Cancel,    PropertyDlg::OnClose)
END_EVENT_TABLE ()

BEGIN_EVENT_TABLE (PropertyGrid, wxGrid)
END_EVENT_TABLE()

PropertyDlg::PropertyDlg (wxWindow * parent, svn::Property *property)
           : wxDialog (parent, -1, "Property Editor", wxDefaultPosition, 
             wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
             m_property(property)
{
  InitializeData ();
  CentreOnParent ();
}

void
PropertyDlg::InitializeData ()
{
  wxBoxSizer * mainSizer = new wxBoxSizer (wxVERTICAL);

  wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer (
        new wxStaticBox(this, -1, _T("Properties")), 
        wxHORIZONTAL);

  propGrid = new PropertyGrid (this, m_property);
  propGrid->CreateGrid (m_property->count () + 5, 2);
  propGrid->SetGridLineColour (wxColour ("black"));

  propGrid->SetColLabelValue (0, "Name");
  propGrid->SetColLabelValue (1, "Value");
  propGrid->SetColSize (0, 120);
  propGrid->SetColSize (1, 120);
  propGrid->FillList ();
  propGrid->Show ();

  wxBoxSizer * buttonSizer = new wxBoxSizer (wxHORIZONTAL);
  wxButton * okButton = new wxButton (this, ID_OK, "OK");
  wxButton * cancelButton = new wxButton (this, ID_Cancel, "Cancel");
  buttonSizer->Add (okButton, 0, wxALL, 5);
  buttonSizer->Add (cancelButton, 0, wxALL, 5);

  boxSizer->Add (propGrid, 1, wxALL | wxEXPAND, 2);
  mainSizer->Add (boxSizer, 0, wxALL | wxCENTER , 5);
  mainSizer->Add (buttonSizer, 1, wxALL | wxCENTER , 5);

  SetAutoLayout (true);
  SetSizer (mainSizer);

  mainSizer->SetSizeHints (this);
  mainSizer->Fit (this);
}

void
PropertyGrid::FillList ()
{
  int index = 0;
  while(m_property->next ())
  {
    SetCellValue (index, 0, m_property->name ());
    SetCellValue (index, 1, m_property->value ());

    index++;
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
  m_property->beforeFirst ();

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
        m_property->set (name.c_str (), value.c_str (), false);
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
  m_property->beforeFirst ();

  while(m_property->next ())
  {
    propValue = m_property->name ();
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
        m_property->remove (propValue.c_str (), false);
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
  }
}

PropertyGrid::PropertyGrid (wxWindow * parent, svn::Property *property)
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
