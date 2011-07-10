/*
 * ====================================================================
 * Copyright (c) 2002-2011 The RapidSVN Group.  All rights reserved.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt.
 * If not, see <http://www.gnu.org/licenses/>.
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
#include "svncpp/url.hpp"

// app
#include "property_dlg.hpp"
#include "utils.hpp"

struct PropertyDlg::Data
{
  wxWindow * window;
  svn::Property property;

public:
  Data(wxWindow * wnd, svn::Context * context, const char * target)
      : window(wnd), property(context, target)
  {
  }
};

PropertyDlg::PropertyDlg(wxWindow * parent,
                         svn::Context * context,
                         const svn::Path & target)
    : ListEditorDlg(parent, _("Property Editor"))
{
  const char * target_ = target.c_str();
  // the property list is read-only when working
  // on a repository
  const bool readOnly = svn::Url::isValid(target_);
  m = new Data(this, context, target_);

  SetCaption(_("Properties:"));
  SetAddTitle(_("New Property"));
  SetEditTitle(_("Edit Property"));
  SetReadOnly(readOnly);
}

PropertyDlg::~PropertyDlg()
{
  delete m;
}

/**
 * transfer the contents of the grid to @a property.
 * Properties, that are set only in @a property will be
 * set to NULL
 *
 * @see TransferDataToWindow
 */
bool
PropertyDlg::TransferDataFromWindow()
{
  try
  {
    const std::vector<svn::PropertyEntry> & entries =
      m->property.entries();
    std::vector<svn::PropertyEntry>::const_iterator it = entries.begin();

    // first check for deleted properties
    while (it != entries.end())
    {
      const svn::PropertyEntry & entry = *it;
      const char * name = entry.name.c_str();
      bool found = FindEntry(Utf8ToLocal(name)) != -1;

      if (!found)
        m->property.remove(name);

      it++;
    }

    // now set the rest. this will add missing properties as well
    int index;
    const int count = GetEntryCount();

    for (index = 0; index < count; index++)
    {
      wxString name, value;

      GetEntryAtIndex(index, name, value);

      std::string nameUtf8(LocalToUtf8(name));
      std::string valueUtf8(LocalToUtf8(value));

      m->property.set(nameUtf8.c_str(), valueUtf8.c_str());
    }
    return true;
  }
  catch (svn::ClientException & e)
  {
    wxMessageBox(_("Error setting the property values:\n%s"),
                 Utf8ToLocal(e.message()).c_str());
    return false;
  }
  catch (...)
  {
    wxMessageBox(_("Error setting the property values"), _("Error"));
    return false;
  }
}

/**
 * this method takes the contents of @a property and write it contents
 * to the grid. all user manipulations will be performed only in the
 * grid.
 *
 * @see TransferDataToWindow
 */
bool
PropertyDlg::TransferDataToWindow()
{
  const std::vector<svn::PropertyEntry> &
  entries = m->property.entries();
  std::vector<svn::PropertyEntry>::const_iterator
  it = entries.begin();

  DeleteAllEntries();
  while (it != entries.end())
  {
    const svn::PropertyEntry & entry = *it;

    SetEntry(Utf8ToLocal(entry.name.c_str()),
             Utf8ToLocal(entry.value.c_str()));

    it++;
  }

  return true;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
