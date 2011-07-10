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

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/path.hpp"
#include "svncpp/status_selection.hpp"

// app
#include "action_event.hpp"
#include "exceptions.hpp"
#include "get_data.hpp"
#include "ids.hpp"
#include "preferences.hpp"
#include "utils.hpp"
#include "annotate_action.hpp"

struct AnnotateAction::Data
{
private:
  Action * action;

public:
  AnnotateData data;
  wxWindow * parent;

  Data(Action * action_)
      : action(action_)
  {
  }


  Data(Action * action_, AnnotateData & data_)
      : action(action_), data(data_)
  {
  }

  svn::Context *
  GetContext()
  {
    return action->GetContext();
  }

  svn::AnnotatedFile *
  GetAnnotatedFile(svn::Path path)
  {
    svn::Client client(GetContext());
    svn::AnnotatedFile * annotatedFile = client.annotate(path,
                                         data.startRevision, data.endRevision);
    return annotatedFile;
  }
};

AnnotateAction::AnnotateAction(wxWindow * parent,
                               const AnnotateData & data)
    : Action(parent, _("Annotate"), UPDATE_LATER),
    dlg(parent, _("Annotate"))
{
  m = new Data(this);
  m->parent = parent;
  m->data = data;
}


AnnotateAction::~AnnotateAction()
{
  delete m;
}


bool
AnnotateAction::Prepare()
{
  if (!Action::Prepare())
  {
    return false;
  }

  { // Busy cursor scope
    wxBusyCursor busyCursor;

    // Now Annotate
    // If the data's path variable is set, then use that value
    // Otherwise use the value of GetTarget()
    // The data's path variable is set on the log dialog
    svn::AnnotatedFile * annotatedFile = NULL;
    svn::Path path;
    if (m->data.path.IsEmpty())
    {
      path = GetTarget();
    }
    else
    {
      path = PathUtf8(m->data.path);
    }
    annotatedFile = m->GetAnnotatedFile(path);
    svn::AnnotatedFile::const_iterator it;
    for (it=annotatedFile->begin(); it!=annotatedFile->end(); it++)
    {
      svn::AnnotateLine line(*it);
      dlg.AddAnnotateLine(line.revision(), Utf8ToLocal(line.author()),
                          Utf8ToLocal(line.line()));
    }
    dlg.AutoSizeColumn();
    delete annotatedFile;
  }

  dlg.ShowModal();

  return true;
}

bool
AnnotateAction::Perform()
{
  return true;
}

bool
AnnotateAction::CheckStatusSel(const svn::StatusSel & statusSel)
{
  if (statusSel.size() != 1)
    return false;

  if (statusSel.hasUnversioned())
    return false;

  if (statusSel.hasDirs())
    return false;

  return true;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
