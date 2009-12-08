/*
 * ====================================================================
 * Copyright (c) 2002-2009 The RapidSvn Group.  All rights reserved.
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

// app
#include "hist_entries.hpp"
#include "hist_val.hpp"
#include "merge_dlg.hpp"
#include "utils.hpp"

struct MergeDlg::Data
{
public:
  MergeData data;

  Data(bool calledByLogDlg, const MergeData & data_)
    : data(data_)
  {
    data.calledByLogDlg = calledByLogDlg;
  }
};


MergeDlg::MergeDlg(wxWindow * parent, bool calledByLogDlg, MergeData & data)
  : MergeDlgBase(parent)
{
  m = new Data(calledByLogDlg, data);

  m_comboUrl1->SetValidator(HistoryValidator(HISTORY_MERGE_URL, &m->data.Path1));
  m_textRevision1->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &m->data.Path1Rev));
  m_comboUrl2->SetValidator(HistoryValidator(HISTORY_MERGE_URL, &m->data.Path2));
  m_textRevision2->SetValidator(wxTextValidator(wxFILTER_NUMERIC, &m->data.Path2Rev));
  m_comboDest->SetValidator(HistoryValidator(HISTORY_MERGE_DEST, &m->data.Destination));

  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  Layout();
  CentreOnParent();

  CheckControls();
}


MergeDlg::~MergeDlg()
{
  delete m;
}


bool 
MergeDlg::TransferDataFromWindow()
{
  if (!MergeDlgBase::TransferDataFromWindow())
    return false;

  TrimString(m->data.Path1);
  TrimString(m->data.Path1Rev);
  TrimString(m->data.Path2);
  TrimString(m->data.Path2Rev);
  TrimString(m->data.Destination);

  return true;
}


bool
MergeDlg::TransferDataToWindow()
{
  if (!MergeDlgBase::TransferDataToWindow())
    return false;

  CheckControls();
  return true;
}


/**
 * Brings up a directory dialog defaulted to the user's home directory.
 */
void
MergeDlg::OnBrowse(wxCommandEvent & WXUNUSED(event))
{
  wxString dest(m_comboDest->GetValue());
  TrimString(dest);

  wxDirDialog dialog(this,
                     _("Select a destination folder to merge to"),
                     dest);

  if (dialog.ShowModal() == wxID_OK)
    m_comboDest->SetValue(dialog.GetPath());
}


void
MergeDlg::OnText(wxCommandEvent &)
{
  CheckControls();
}


void
MergeDlg::CheckControls()
{
  wxString url1(m_comboUrl1->GetValue());
  wxString rev1(m_textRevision1->GetValue());
  wxString url2(m_comboUrl2->GetValue());
  wxString rev2(m_textRevision2->GetValue());
  wxString dest(m_comboDest->GetValue());

  TrimString(url1);
  TrimString(rev1);
  TrimString(url2);
  TrimString(rev2);
  TrimString(dest);

  bool ok = 
    !url1.IsEmpty() && !url1.IsEmpty() &&
    !dest.IsEmpty() && CheckRevision(rev1) && 
    CheckRevision(rev2);

  bool readOnly = m->data.calledByLogDlg;
  EnableCtrl(m_comboUrl1, !readOnly);
  EnableCtrl(m_comboUrl2, !readOnly);
  EnableCtrl(m_textRevision1, !readOnly);
  EnableCtrl(m_textRevision2, !readOnly);

  EnableCtrl(m_buttonOK, ok);
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
