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
#include "wx/grid.h"
#include "wx/listctrl.h"
#include "wx/valgen.h"

// svncpp
#include "svncpp/exception.hpp"

// app
#include "property_dlg.hpp"
#include "utils.hpp"

enum
{
  ID_OK = 1,
  ID_Cancel,
  ID_New,
  ID_Edit,
  ID_Delete,
  ID_List,
  ID_Name,
  ID_Value
};

enum
{
  EDIT_NEW,
  EDIT_EDIT
};

static const int PROPERTY_VALUE_WIDTH = 50;
static const int PROPERTY_VALUE_HEIGHT = 10;


class PropertyList : public wxListView
{
public:
  PropertyList (wxWindow * parent)
    : wxListView (parent, ID_List, wxDefaultPosition,
                  wxSize (350, 150), wxLC_REPORT | 
                  wxLC_SINGLE_SEL)
  {
    wxListItem info;

    info.m_mask = wxLIST_MASK_TEXT | wxLIST_MASK_WIDTH;
    info.m_col = 0;
    info.m_width = 120;
    info.m_text = _("Name");
    InsertColumn (0, info);

    info.m_col = 1;
    info.m_width = 240;
    info.m_text = _("Value");
    InsertColumn (1, info);

  }

  /**
   * Adds/modifys a property in the list.
   * If the property is not already there (identified by name),
   * add it to the list, otherwise modify the value.
   *
   * @param name property value
   * @param value property value
   * @return id of new item if inserter or old item if updated
   */
  long 
  SetProperty (const wxString & name, const wxString & value)
  {
    long id = FindItem (-1, name, false);

    if (id == -1)
    {
      id = GetItemCount ();
      InsertItem (id, name);
    }

    wxListItem item;
    item.m_itemId = id;
    item.m_text = value;
    item.m_col = 1;
    item.m_mask = wxLIST_MASK_TEXT;
    SetItem (item);

    return id;
  }

  /**
   * returns the name/value pair for the selected item in the
   * list
   *
   * @param name property name
   * @param value property value
   */
  void
  GetSelected (wxString & name, wxString & value)
  {
    long id = GetFirstSelected ();
    if (id == -1)
      return;
    GetProperty (id, name, value);
  }

  /**
   * returns the value of the property at the given position
   *
   * @param id zero based position int the list
   * @param name property name
   * @param value property value
   */
  void 
  GetProperty (long id, wxString & name, wxString & value)
  {
    // get name
    name = GetItemText (id);

    // get value
    wxListItem item;
    item.m_itemId = id;
    item.m_col = 1;
    item.m_mask = wxLIST_MASK_TEXT;

    GetItem (item);
    value = item.m_text;
  }

};


/**
 * private class for the property dialog.
 * this edits one single property name/value pair
 */
class PropertyEditor : public wxDialog
{
public:
  /**
   * Constructor
   */
  PropertyEditor (wxWindow * parent)
    : wxDialog (parent, -1, "bla", wxDefaultPosition)
  {
    wxStaticText * labelName = new wxStaticText (this, -1, _("Name"));
    wxStaticText * labelValue = new wxStaticText (this, -1, _("Value"));
    wxTextCtrl * textName = new wxTextCtrl (this, ID_Name);

    wxTextCtrl * textValue = 
      new wxTextCtrl (this, ID_Value, "", wxDefaultPosition, 
                      wxSize (300, 100), wxTE_MULTILINE);

    wxFlexGridSizer * textSizer = new wxFlexGridSizer (2, 5, 5);
    textSizer->Add (labelName);
    textSizer->Add (textName, 1, wxEXPAND);
    textSizer->Add (labelValue);
    textSizer->Add (textValue, 1, wxEXPAND);

    wxButton * okButton = new wxButton (this, ID_OK, _("OK"));
    wxButton * cancelButton = new wxButton (this, ID_Cancel, _("Cancel"));
    wxBoxSizer * buttonSizer = new wxBoxSizer (wxHORIZONTAL);
    buttonSizer->Add (okButton, 0, wxALL, 5);
    buttonSizer->Add (cancelButton, 0, wxALL, 5);

    wxBoxSizer * mainSizer = new wxBoxSizer (wxVERTICAL);

    mainSizer->Add (textSizer, 1, wxALL | wxEXPAND, 5);
    mainSizer->Add (buttonSizer, 0, wxALL | wxALIGN_RIGHT , 5);

    SetAutoLayout (true);
    SetSizer (mainSizer);

    mainSizer->SetSizeHints (this);
    mainSizer->Fit (this);

    m_buttonOk = okButton;
    m_textName = textName;
    m_textValue = textValue;
  }
  /**
   * Execute the dialog
   *
   * @param caption caption of the dialog
   * @param value name of the property
   * @retval true if OK button was clicked
   */

  bool
  Execute (const int mode,
           wxString & name,
           wxString &value)
  {
    wxString caption;
    switch (mode)
    {
    case EDIT_NEW:
      caption = _("New Property");
      break;
    case EDIT_EDIT:
      caption = _("Edit Property");
      break;
    default:
      // invalid mode, exit
      return false;
    }

    SetTitle (caption);
    SetReturnCode (ID_Cancel);
    m_textName->Enable (mode == EDIT_NEW);
    TrimString (name);
    TrimString (value);
    m_textName->SetValue (name);
    m_textValue->SetValue (value);

    CheckButtons ();

    bool ok = ShowModal () == ID_OK;

    if (ok)
    {
      name = m_textName->GetValue ();
      value = m_textValue->GetValue ();
      TrimString (name);
      TrimString (value);
    }

    return ok;
  }

private:
  wxTextCtrl * m_textName;
  wxTextCtrl * m_textValue;
  wxButton * m_buttonOk;

  void 
  OnCancel (wxCommandEvent &)
  {
    EndModal (ID_Cancel);
  }

  void
  OnOk (wxCommandEvent &)
  {
    EndModal (ID_OK);
  }

  void
  OnName (wxCommandEvent &)
  {
    CheckButtons ();
  }

  void CheckButtons ()
  {
    wxString name = m_textName->GetValue ();
    TrimString (name);
    m_buttonOk->Enable (name.Length () > 0);
  }

private:
  DECLARE_EVENT_TABLE ()
};

BEGIN_EVENT_TABLE (PropertyEditor, wxDialog)
  EVT_BUTTON (ID_Cancel, PropertyEditor::OnCancel)
  EVT_BUTTON (ID_OK,   PropertyEditor::OnOk)
  EVT_TEXT   (ID_Name, PropertyEditor::OnName)
END_EVENT_TABLE ()

struct PropertyDlg::Data
{
  wxWindow * window;
  svn::Property property;
  //PropertyGrid * grid;
  PropertyList * grid;
  wxButton * editButton;
  wxButton * delButton;
  
public:
  Data (wxWindow * wnd, svn::Context * context, const char * target)
    : window (wnd), property (context, target)
  {
    // create controls
    wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer (
      new wxStaticBox (wnd, -1, _("Properties")), 
      wxHORIZONTAL);

    grid = new PropertyList (wnd);
    boxSizer->Add (grid, 1, wxALL | wxEXPAND, 2);

    // buttons
    wxButton * newButton = new wxButton (wnd, ID_New, _("New..."));
    editButton = new wxButton (wnd, ID_Edit, _("Edit..."));
    editButton->Enable (false);
    delButton = new wxButton (wnd, ID_Delete, _("Delete"));
    delButton->Enable (false);
    wxButton * okButton = new wxButton (wnd, ID_OK, _("OK"));
    wxButton * cancelButton = new wxButton (wnd, ID_Cancel, _("Cancel"));

    wxBoxSizer * buttonSizer = new wxBoxSizer (wxHORIZONTAL);
    buttonSizer->Add (newButton, 0, wxALL, 5);
    buttonSizer->Add (editButton, 0, wxALL, 5);
    buttonSizer->Add (delButton, 0, wxALL, 5);
    buttonSizer->Add (20, 20);
    buttonSizer->Add (okButton, 0, wxALL, 5);
    buttonSizer->Add (cancelButton, 0, wxALL, 5);

    wxBoxSizer * mainSizer = new wxBoxSizer (wxVERTICAL);

    mainSizer->Add (boxSizer, 1, wxALL | wxCENTER | wxEXPAND , 5);
    mainSizer->Add (buttonSizer, 0, wxALL | wxALIGN_RIGHT , 5);

    wnd->SetAutoLayout (true);
    wnd->SetSizer (mainSizer);

    mainSizer->SetSizeHints (wnd);
    mainSizer->Fit (wnd);

    WriteToGrid ();
  }

  /**
   * transfer the contents of the grid to @a property.
   * Properties, that are set only in @a property will be
   * set to NULL
   * 
   * @see WriteToGrid
   */
  void
  ReadFromGrid ()
  {
    try
    {
      const std::vector<svn::PropertyEntry> & entries
        = property.entries ();
      std::vector<svn::PropertyEntry>::const_iterator it = entries.begin ();

      // first check for deleted properties
      while (it != entries.end ())
      {
        const svn::PropertyEntry & entry = *it;
        const char * name = entry.name.c_str ();
        long found = grid->FindItem (-1, name, false) != -1;

        if (!found)
        {
          property.remove (name);
        }

        it++;
      }

      // now set the rest. this will add missing properties as well
      int index;
      const int count = grid->GetItemCount ();

      for (index = 0; index < count; index++ )
      {
        wxString name, value;

        grid->GetProperty (index, name, value);

        property.set (name.c_str (), value.c_str ());

        index++;
      }    
    }
    catch (...)
    {
      wxMessageBox (_("Error setting the property values"), _("Error"));
    }

  }

  /**
   * this method takes the contents of @a property and write it contents
   * to the grid. all user manipulations will be performed only in the
   * grid.
   *
   * @see ReadFromGrid
   */
  void 
  WriteToGrid ()
  {
    const std::vector<svn::PropertyEntry> & entries
      = property.entries ();
    std::vector<svn::PropertyEntry>::const_iterator it = entries.begin ();

    grid->DeleteAllItems ();
    while (it != entries.end ())
    {
      const svn::PropertyEntry & entry = *it;

      grid->SetProperty (entry.name.c_str (), entry.value.c_str ());
  
      it++;
    }
  }

  /**
   * returns whether an item in the list is selected
   *
   * @retval true item is selected
   */
  bool
  IsSelected ()
  {
    if (!grid)
      return false;

    return grid->GetFirstSelected () != -1;
  }

  /**
   * methode gets called by the selection event of
   * the grid
   */
  void 
  OnSelected ()
  {
    // the edit and delete buttons will only be
    // enabled if there is a selected item
    bool selected = IsSelected ();
    editButton->Enable (selected);
    delButton->Enable (selected);
  }

  /**
   * deletes the selected item in the list
   */
  void 
  DeleteSelected ()
  {
    long id = grid->GetFirstSelected ();

    if (id == -1)
      return;

    grid->DeleteItem (id);
    OnSelected ();
  }

  /**
   * shows the dialog for a property to edit or add.
   *
   * @param edit EDIT_NEW or EDIT_EDIT
   */
  void
  Edit (int mode)
  {
    wxString name, value;

    if (mode == EDIT_EDIT)
    {
      grid->GetSelected (name, value);
    }

    PropertyEditor dlg (window);
    if (!dlg.Execute (mode, name, value))
      return;

    grid->SetProperty (name, value);
  }

};

BEGIN_EVENT_TABLE (PropertyDlg, wxDialog)
  EVT_BUTTON (ID_OK, PropertyDlg::OnOK)
  EVT_BUTTON (ID_Cancel, PropertyDlg::OnClose)
  EVT_BUTTON (ID_New, PropertyDlg::OnNew)
  EVT_BUTTON (ID_Edit, PropertyDlg::OnEdit)
  EVT_BUTTON (ID_Delete, PropertyDlg::OnDelete)
  EVT_LIST_ITEM_SELECTED (ID_List, PropertyDlg::OnSelected)
END_EVENT_TABLE ()

PropertyDlg::PropertyDlg (wxWindow * parent, 
                          svn::Context * context,
                          const svn::Path & target)
  : wxDialog (parent, -1, _("Property Editor"), wxDefaultPosition, 
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (this, context, target.c_str ());
  CentreOnParent ();
}

PropertyDlg::~PropertyDlg ()
{
  delete m;
}

void
PropertyDlg::OnClose (wxCommandEvent & event)
{
  Close (TRUE);
}

void
PropertyDlg::OnOK (wxCommandEvent & event)
{
  m->ReadFromGrid ();
  Close (TRUE);
}

void
PropertyDlg::OnNew (wxCommandEvent & event)
{
  m->Edit (EDIT_NEW);
}

void
PropertyDlg::OnEdit (wxCommandEvent & event)
{
  m->Edit (EDIT_EDIT);
}

void
PropertyDlg::OnDelete (wxCommandEvent & event)
{
  m->DeleteSelected ();
}

void 
PropertyDlg::OnSelected (wxListEvent &)
{
  m->OnSelected ();
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
