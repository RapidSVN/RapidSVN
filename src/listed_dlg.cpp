/*
 * ====================================================================
 * Copyright (c) 2002-2007 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxWidgets
#include "wx/wx.h"
#include "wx/grid.h"
#include "wx/listctrl.h"
#include "wx/valgen.h"

// svncpp
#include "svncpp/exception.hpp"

// app
#include "listed_dlg.hpp"
#include "utils.hpp"

enum
{
  ID_New = 0x2000,
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

static const wxChar * LABEL_EDIT=_("&Edit...");
static const wxChar * LABEL_VIEW=_("&View...");
static const wxChar * LABEL_NEW=_("&New...");
static const wxChar * LABEL_DELETE=_("&Delete...");

class ListCtrl : public wxListView
{
public:
  ListCtrl (wxWindow * parent)
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
   * Adds/modifys an entry in the list.
   * If the entry is not already there (identified by name),
   * add it to the list, otherwise modify the value.
   *
   * @param name
   * @param value
   * @return id of new item if inserted or old item if updated
   */
  long
  SetEntry (const wxString & name, const wxString & value)
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
   * @param name
   * @param value
   * @retval false nothing selected
   */
  bool
  GetSelectedEntry (wxString & name, wxString & value)
  {
    long id = GetFirstSelected ();
    if (id == -1)
      return false;

    GetEntryAtIndex (id, name, value);
    return true;
  }

  /**
   * returns the value of the entry at the given position
   *
   * @param id zero based position int the list
   * @param name property name
   * @param value property value
   */
  void
  GetEntryAtIndex (long id, wxString & name, wxString & value)
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
 * private class for the dialog.
 * this edits one single name/value pair
 */
class EntryDlg : public wxDialog
{
public:
  /**
   * Constructor
   */
  EntryDlg (wxWindow * parent, const wxString & title)
    : wxDialog (parent, -1, title, wxDefaultPosition),
      m_readOnly (false)
  {
    wxStaticText * labelName = new wxStaticText (this, -1, _("Name"));
    wxStaticText * labelValue = new wxStaticText (this, -1, _("Value"));
    wxTextCtrl * textName = new wxTextCtrl (this, ID_Name);

    wxTextCtrl * textValue =
      new wxTextCtrl (this, ID_Value, wxEmptyString, wxDefaultPosition,
                      wxSize (300, 100), wxTE_MULTILINE);

    wxFlexGridSizer * textSizer = new wxFlexGridSizer (2, 5, 5);
    textSizer->Add (labelName);
    textSizer->Add (textName, 1, wxEXPAND);
    textSizer->Add (labelValue);
    textSizer->Add (textValue, 1, wxEXPAND);

    wxButton * okButton = new wxButton (this, wxID_OK, _("OK"));
    wxButton * cancelButton = new wxButton (this, wxID_CANCEL, _("Cancel"));
    wxBoxSizer * buttonSizer = new wxBoxSizer (wxHORIZONTAL);
    buttonSizer->Add (okButton, 0, wxALL, 5);
    buttonSizer->Add (cancelButton, 0, wxALL, 5);

    wxBoxSizer * mainSizer = new wxBoxSizer (wxVERTICAL);

    mainSizer->Add (textSizer, 1, wxALL | wxEXPAND, 5);
    mainSizer->Add (buttonSizer, 0, wxALL | wxALIGN_RIGHT , 5);

    okButton->SetDefault ();

    SetAutoLayout (true);
    SetSizer (mainSizer);

    mainSizer->SetSizeHints (this);
    mainSizer->Fit (this);

    m_buttonOk = okButton;
    m_textName = textName;
    m_textValue = textValue;
    m_labelName = labelName;
    m_labelValue = labelValue;

    m_mode = EDIT_NEW;
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
           wxString & value)
  {
    switch (mode)
    {
    case EDIT_NEW:
    case EDIT_EDIT:
      break;
    default:
      // invalid mode, exit
      return false;
    }
    m_mode = mode;

    SetReturnCode (wxID_CANCEL);
    m_textName->Enable (mode == EDIT_NEW);
    TrimString (name);
    TrimString (value);
    m_textName->SetValue (name);
    m_textValue->SetValue (value);

    CheckButtons ();

    bool ok = ShowModal () == wxID_OK;

    if (ok)
    {
      name = m_textName->GetValue ();
      value = m_textValue->GetValue ();
      TrimString (name);
      TrimString (value);
    }

    return ok;
  }

  void SetNameCaption (const wxString & caption)
  {
    m_labelName->SetLabel (caption);
  }

  void SetValueCaption (const wxString & caption)
  {
    m_labelValue->SetLabel (caption);
  }

  void SetReadOnly (bool value)
  {
    m_readOnly = value;
    m_textName->Enable ((m_mode == EDIT_NEW) && !value);
    m_textValue->Enable (!value);
    CheckButtons ();
  }

private:
  bool m_readOnly;
  wxTextCtrl * m_textName;
  wxTextCtrl * m_textValue;
  wxStaticText * m_labelName;
  wxStaticText * m_labelValue;
  wxButton * m_buttonOk;
  int m_mode;

  void
  OnName (wxCommandEvent &)
  {
    CheckButtons ();
  }

  void
  CheckButtons ()
  {
    wxString name = m_textName->GetValue ();
    TrimString (name);
    m_buttonOk->Enable ((name.Length () > 0) && !m_readOnly);
  }

private:
  DECLARE_EVENT_TABLE ()
};

BEGIN_EVENT_TABLE (EntryDlg, wxDialog)
  EVT_TEXT (ID_Name, EntryDlg::OnName)
END_EVENT_TABLE ()

struct ListEditorDlg::Data
{
  wxWindow * window;
  wxString nameCaption;
  wxString valueCaption;
  bool readOnly;
  ListCtrl * listCtrl;
  wxButton * newButton;
  wxButton * okButton;
  wxButton * editButton;
  wxButton * delButton;
  wxStaticBox * box;
  wxString addTitle;
  wxString editTitle;

public:
  Data (wxWindow * wnd)
    : window (wnd), nameCaption (_("Name")), valueCaption (_("Value")),
      readOnly (false)
  {
    // create controls
    wxStaticBoxSizer *boxSizer = new wxStaticBoxSizer (
      box = new wxStaticBox (wnd, -1, wxEmptyString),
      wxHORIZONTAL);

    listCtrl = new ListCtrl (wnd);
    boxSizer->Add (listCtrl, 1, wxALL | wxEXPAND, 2);

    // buttons
    newButton = new wxButton (wnd, ID_New, LABEL_NEW);
    editButton = new wxButton (wnd, ID_Edit, LABEL_EDIT);
    editButton->Enable (false);
    delButton = new wxButton (wnd, ID_Delete, LABEL_DELETE);
    delButton->Enable (false);
    okButton = new wxButton (wnd, wxID_OK, _("OK"));
    wxButton * cancelButton = new wxButton (wnd, wxID_CANCEL, _("Cancel"));

    wxBoxSizer * buttonSizer = new wxBoxSizer (wxHORIZONTAL);
    buttonSizer->Add (newButton, 0, wxALL, 5);
    buttonSizer->Add (editButton, 0, wxALL, 5);
    buttonSizer->Add (delButton, 0, wxALL, 5);
    buttonSizer->Add (20, 20);
    buttonSizer->Add (okButton, 0, wxALL, 5);
    buttonSizer->Add (cancelButton, 0, wxALL, 5);

    cancelButton->SetDefault ();
    wxBoxSizer * mainSizer = new wxBoxSizer (wxVERTICAL);

    mainSizer->Add (boxSizer, 1, wxALL | wxCENTER | wxEXPAND , 5);
    mainSizer->Add (buttonSizer, 0, wxALL | wxALIGN_RIGHT , 5);

    wnd->SetAutoLayout (true);
    wnd->SetSizer (mainSizer);

    mainSizer->SetSizeHints (wnd);
    mainSizer->Fit (wnd);
  }


  /**
   * returns whether an item in the list is selected
   *
   * @retval true item is selected
   */
  bool
  IsSelected ()
  {
    if (!listCtrl)
      return false;

    return listCtrl->GetFirstSelected () != -1;
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
    delButton->Enable (selected && !readOnly);
  }

  /**
   * deletes the selected item in the list
   */
  void
  DeleteSelected ()
  {
    long id = listCtrl->GetFirstSelected ();

    if (id == -1)
      return;

    listCtrl->DeleteItem (id);
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
    wxString name, value, title;

    if (mode != EDIT_EDIT)
      title = addTitle;
    else
    {
      title = editTitle;
      listCtrl->GetSelectedEntry (name, value);
    }

    EntryDlg dlg (window, title);
    dlg.SetReadOnly (readOnly);
    if (!dlg.Execute (mode, name, value))
      return;

    listCtrl->SetEntry (name, value);
  }

};

BEGIN_EVENT_TABLE (ListEditorDlg, wxDialog)
  EVT_BUTTON (ID_New, ListEditorDlg::OnNew)
  EVT_BUTTON (ID_Edit, ListEditorDlg::OnEdit)
  EVT_BUTTON (ID_Delete, ListEditorDlg::OnDelete)
  EVT_LIST_ITEM_SELECTED (ID_List, ListEditorDlg::OnSelected)
END_EVENT_TABLE ()

ListEditorDlg::ListEditorDlg (wxWindow * parent,
                              const wxString & title)
  : wxDialog (parent, -1, title, wxDefaultPosition,
              wxDefaultSize, wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
  m = new Data (this);
  CentreOnParent ();
}

ListEditorDlg::~ListEditorDlg ()
{
  delete m;
}

void
ListEditorDlg::OnNew (wxCommandEvent & event)
{
  m->Edit (EDIT_NEW);
}

void
ListEditorDlg::OnEdit (wxCommandEvent & event)
{
  m->Edit (EDIT_EDIT);
}

void
ListEditorDlg::OnDelete (wxCommandEvent & event)
{
  m->DeleteSelected ();
}

void
ListEditorDlg::OnSelected (wxListEvent &)
{
  m->OnSelected ();
}

void
ListEditorDlg::SetCaption (const wxString & caption)
{
  m->box->SetLabel (caption);
}

void
ListEditorDlg::SetNameCaption (const wxString & caption)
{
  m->nameCaption = caption;
}

void
ListEditorDlg::SetValueCaption (const wxString & caption)
{
  m->valueCaption = caption;
}

void
ListEditorDlg::SetAddTitle (const wxString & title)
{
  m->addTitle = title;
}

void
ListEditorDlg::SetEditTitle (const wxString & title)
{
  m->editTitle = title;
}

void
ListEditorDlg::DeleteAllEntries ()
{
  m->listCtrl->DeleteAllItems ();
}

long
ListEditorDlg::SetEntry (const wxString & name, const wxString & value)
{
  return m->listCtrl->SetEntry (name, value);
}

void
ListEditorDlg::GetEntryAtIndex (long id, wxString & name, wxString & value) const
{
  m->listCtrl->GetEntryAtIndex (id, name, value);
}

long
ListEditorDlg::GetEntryCount () const
{
  return m->listCtrl->GetItemCount ();
}

int
ListEditorDlg::ShowModal ()
{
  int result;
  WriteToGrid ();

  result = wxDialog::ShowModal ();

  if (result == wxID_OK)
    ReadFromGrid ();

  return result;
}

long
ListEditorDlg::FindEntry (const wxString & name) const
{
  return m->listCtrl->FindItem (-1, name, false);
}

void
ListEditorDlg::SetReadOnly (bool value)
{
  m->readOnly = value;
  m->newButton->Enable (!value);
  m->okButton->Enable (!value);
  m->OnSelected ();
  m->editButton->SetLabel (value ? LABEL_VIEW : LABEL_EDIT);
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
