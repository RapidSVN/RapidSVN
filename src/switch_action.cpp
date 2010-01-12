/*
 * ====================================================================
 * Copyright (c) 2002-2010 The RapidSvn Group.  All rights reserved.
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

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/info.hpp"
#include "svncpp/status.hpp"
#include "svncpp/url.hpp"

// app
#include "switch_action.hpp"
#include "switch_dlg.hpp"
#include "utils.hpp"

struct SwitchAction::Data
{
public:
  wxString old_url;
  wxString new_url;
  svn::Revision revision;
  bool recursive;
  bool relocate;
};

SwitchAction::SwitchAction(wxWindow * parent)
    : Action(parent, _("Switch URL"))
{
  m = new Data();
}


SwitchAction::~SwitchAction()
{
  delete m;
}


bool
SwitchAction::Prepare()
{
  if (!Action::Prepare())
  {
    return false;
  }

  // first try to get the URL for the target
  svn::Path path = GetTarget();
  svn::Client client(GetContext());
  svn::InfoVector infoVector(client.info(path));
  if (infoVector.size() != 1)
    return false;

  m->old_url = Utf8ToLocal(svn::Url::unescape(infoVector[0].url()));

  SwitchDlg dlg(GetParent(), m->old_url, true, false);

  if (dlg.ShowModal() != wxID_OK)
  {
    return false;
  }

  m->new_url = dlg.GetUrl();
  m->revision = dlg.GetRevision();
  m->recursive = dlg.GetRecursive();
  m->relocate = dlg.GetRelocate();

  return true;
}

bool
SwitchAction::Perform()
{
  svn::Path path = GetTarget();
  svn::Client client(GetContext());

  if (m->relocate)
    client.relocate(path,
                    LocalToUtf8(m->old_url).c_str(),
                    LocalToUtf8(m->new_url).c_str(),
                    m->recursive);
  else
    client.doSwitch(path,
                    LocalToUtf8(m->new_url).c_str(),
                    m->revision,
                    m->recursive);

  return true;
}

bool
SwitchAction::CheckStatusSel(const svn::StatusSel & WXUNUSED(statusSel))
{
  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
