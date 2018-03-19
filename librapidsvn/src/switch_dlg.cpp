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
#include "svncpp/revision.hpp"

// app
#include "hist_entries.hpp"
#include "hist_val.hpp"
#include "switch_dlg.hpp"
#include "utils.hpp"


struct SwitchDlg::Data
{
public:
  wxString url;
  wxString revision;
  bool recursive;
  bool relocate;
  bool useLatest;

  Data(const wxString & url_, bool recursive_, bool relocate_)
    : url(url_), revision(wxT("")),
      recursive(recursive_), relocate(relocate_),
      useLatest(true)
  {
  }
};

SwitchDlg::SwitchDlg(wxWindow* parent, const wxString & url,
                     bool recursive, bool relocate)
  : SwitchDlgBase(parent, -1)
{
  m = new Data(url, recursive, relocate);

  m_comboUrl->SetValidator(HistoryValidator(HISTORY_REPOSITORY, &m->url));
  m_textRevision->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &m->revision));
  m_checkUseLatest->SetValidator(wxGenericValidator(&m->useLatest));
  m_checkRecursive->SetValidator(wxGenericValidator(&m->recursive));
  m_checkRelocate->SetValidator(wxGenericValidator(&m->relocate));

  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  Layout();
  CentreOnParent();

  CheckControls();
}

SwitchDlg::~SwitchDlg()
{
  delete m;
}

void
SwitchDlg::OnUseLatest(wxCommandEvent &)
{
  CheckControls();
}

void
SwitchDlg::OnText(wxCommandEvent &)
{
  CheckControls();
}

svn::Revision
SwitchDlg::GetRevision() const
{
  if (m->useLatest)
    return svn::Revision::HEAD;
  else
  {
    svn_revnum_t revnum;

    TrimString(m->revision);
    m->revision.ToLong(&revnum);

    return revnum;
  }
}


const wxString &
SwitchDlg::GetUrl() const
{
  return m->url;
}


bool
SwitchDlg::GetRelocate() const
{
  return m->relocate;
}


bool
SwitchDlg::GetRecursive() const
{
  return m->recursive;
}


void
SwitchDlg::CheckControls()
{
  bool useLatest = m_checkUseLatest->IsChecked();

  EnableCtrl(m_textRevision, !useLatest);

  bool ok = true;

  if (!useLatest)
    ok = CheckRevision(m_textRevision->GetValue());

  if (ok)
  {
    svn::Path UrlUtf8(PathUtf8(m_comboUrl->GetValue()));
    ok = UrlUtf8.isUrl();
  }

  EnableCtrl(m_buttonOK, ok);
}


/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
