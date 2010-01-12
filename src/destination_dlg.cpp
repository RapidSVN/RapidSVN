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
#include "destination_dlg.hpp"
#include "hist_val.hpp"
#include "utils.hpp"


struct DestinationDlg::Data
{
public:
  wxString destination;
  wxString history;
  bool force;
  bool withForce;

  Data(int flags, const wxString & dest, const wxString history_)
    : destination(dest), history(history_), force(false)
  {
    withForce = (flags & WITH_FORCE) != 0;
  }
};


DestinationDlg::DestinationDlg(wxWindow* parent,
                               const wxString & title,
                               const wxString & descr,
                               int flags,
                               const wxString & dst,
                               const wxString & history)
  : DestinationDlgBase(parent, -1, title)
{
  m = new Data(flags, dst, history);

  // The destination:
  if (m->history.IsEmpty())
    m_textDestination->SetValidator(wxTextValidator(wxFILTER_NONE, &m->destination));
  else
    m_textDestination->SetValidator(HistoryValidator(m->history, &m->destination, false, false));

  m_staticQuestion->SetLabel(descr);
  m_checkForce->SetValidator(wxGenericValidator(&m->force));

  if (!m->withForce)
    m_checkForce->Show(false);
  
  m_mainSizer->SetSizeHints(this);
  m_mainSizer->Fit(this);

  Layout();
  CentreOnParent();

  CheckControls();
}


DestinationDlg::~DestinationDlg()
{
  delete m;
}


const wxString &
DestinationDlg::GetDestination() const
{
  return m->destination;
}


bool
DestinationDlg::GetForce() const
{
  return m->force;
}


void
DestinationDlg::OnText(wxCommandEvent &)
{
  CheckControls();
}


void 
DestinationDlg::CheckControls()
{
  bool ok = !m_textDestination->GetValue().IsEmpty();

  EnableCtrl(m_buttonOK, ok);
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
