/*
 * ====================================================================
 * Copyright (c) 2002-2012 The RapidSVN Group.  All rights reserved.
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
#include "wx/valgen.h"

// svncpp
#include "svncpp/path.hpp"

// app
#include "hist_entries.hpp"
#include "hist_val.hpp"
#include "rapidsvn_generated.h"
#include "update_dlg.hpp"
#include "update_data.hpp"
#include "utils.hpp"

struct UpdateDlg::Data
{
public:
  UpdateData data;
  bool hasUrl;
  bool hasRevision;
  bool hasRecursive;
  bool hasIgnoreExternals;

  Data(int flags)
  {
    hasUrl = 0 != (flags & WITH_URL);
    hasRevision = 0 == (flags & WITHOUT_REVISION);
    hasRecursive = 0 == (flags & WITHOUT_RECURSIVE);
    hasIgnoreExternals = 0 == (flags & WITHOUT_IGNORE_EXTERNALS);
  }
};


UpdateDlg::UpdateDlg(wxWindow* parent, const wxString & title, int flags,
                     bool recursive)
  : UpdateDlgBase(parent, -1, title)
{
  m = new Data(flags);
  m->data.recursive = recursive;

  m_comboUrl->SetValidator(HistoryValidator(HISTORY_REPOSITORY, &m->data.url));
  m_textRevision->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &m->data.revision));
  m_checkUseLatest->SetValidator(wxGenericValidator(&m->data.useLatest));
  m_checkRecursive->SetValidator(wxGenericValidator(&m->data.recursive));
  m_checkIgnoreExternals->SetValidator(wxGenericValidator(&m->data.ignoreExternals));

  if (!m->hasUrl)
    m_mainSizer->Show(m_urlSizer, false);

  if (!m->hasRevision)
    m_mainSizer->Show(m_revisionSizer, false);

  m_checkRecursive->Show(m->hasRecursive);
  m_checkIgnoreExternals->Show(m->hasIgnoreExternals);

  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  Layout();
  CentreOnParent();

  CheckControls();
}

UpdateDlg::~UpdateDlg()
{
  delete m;
}


void
UpdateDlg::OnUseLatest(wxCommandEvent &)
{
  CheckControls();
}

UpdateData &
UpdateDlg::GetData()
{
  return m->data;
}

void
UpdateDlg::OnText(wxCommandEvent &)
{
  CheckControls();
}

void
UpdateDlg::CheckControls()
{
  bool useLatest = m_checkUseLatest->IsChecked();

  EnableCtrl(m_textRevision, !useLatest);

  bool ok=true;
  if (!useLatest)
    ok = CheckRevision(m_textRevision->GetValue());

  if (ok && m->hasUrl)
  {
    svn::Path UrlUtf8(PathUtf8(m_comboUrl->GetValue()));
    ok = UrlUtf8.isUrl();
  }

  m_buttonOK->Enable(ok);
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
