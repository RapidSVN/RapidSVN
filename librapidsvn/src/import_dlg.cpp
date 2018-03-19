/*
 * ====================================================================
 * Copyright (c) 2002-2018 The RapidSVN Group.  All rights reserved.
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
 * along with this program (in the file GPL.txt).
 * If not, see <http://www.gnu.org/licenses/>.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// wxWidgets
#include "wx/wx.h"
#include "wx/valgen.h"

// svncpp
#include "svncpp/path.hpp"

// app
#include "hist_entries.hpp"
#include "hist_val.hpp"
#include "import_data.hpp"
#include "import_dlg.hpp"
#include "utils.hpp"

struct ImportDlg::Data
{
public:
  ImportData data;

  Data(const svn::Path & selectedUrl)
  {

    data.PathType = ImportData::DIRECTORY;
    data.Recursive = true;

    if (selectedUrl.isUrl())
      data.Repository = Utf8ToLocal(selectedUrl.c_str());

  }

};


ImportDlg::ImportDlg(wxWindow * parent, const svn::Path & selectedUrl)
  : ImportDlgBase(parent)
{
  m = new Data(selectedUrl);
  CentreOnParent();

  m_comboUrl->SetValidator(HistoryValidator(HISTORY_REPOSITORY, &m->data.Repository));
  m_textPath->SetValidator(wxTextValidator(wxFILTER_NONE, &m->data.Path));
  m_textMessage->SetValidator(wxTextValidator(wxFILTER_NONE, &m->data.LogMessage));
  m_choicePathType->SetValidator(wxGenericValidator(&m->data.PathType));
  m_checkRecursive->SetValidator(wxGenericValidator(&m->data.Recursive));

  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  Layout();
  CentreOnParent();

  CheckControls();
}

ImportDlg::~ImportDlg()
{
  delete m;
}


void
ImportDlg::CheckControls()
{
  wxString url(m_comboUrl->GetValue());
  wxString path(m_textPath->GetValue());
  int pathType = m_choicePathType->GetSelection();

  bool ok = true;

  // valid url?
  if (url.IsEmpty() || !PathUtf8(url).isUrl())
    ok = false;

  // path entered?
  if (path.IsEmpty())
    ok = false;

  if (ok)
  {
    // if entered, is path a valid dir/file?
    switch (pathType)
    {
    case ImportData::DIRECTORY:
      if (!::wxDirExists(path))
        ok = false;
      break;
    case ImportData::FILE:
      if (!::wxFileExists(path))
        ok = false;
    }
  }

  EnableCtrl(m_buttonOK, ok);
}


void
ImportDlg::OnBrowse(wxCommandEvent & WXUNUSED(event))
{
  wxString oldPath(m_textPath->GetValue());

  if (ImportData::DIRECTORY == m_choicePathType->GetSelection())
  {
    wxDirDialog dlg(this, _("Select a directory to import"), oldPath);
    if (wxID_OK == dlg.ShowModal())
      UpdateValue(m_textPath, dlg.GetPath());
  }
  else
  {
    wxFileDialog dlg(this,_("Select a file to import"), oldPath);
    if (wxID_OK == dlg.ShowModal())
      UpdateValue(m_textPath, dlg.GetPath());
  }

  CheckControls();
}


ImportData &
ImportDlg::GetData()
{
  return m->data;
}


void
ImportDlg::OnCommand(wxCommandEvent &)
{
  CheckControls();
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
