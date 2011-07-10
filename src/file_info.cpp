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

// apr
#include "apr_time.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/exception.hpp"
#include "svncpp/path.hpp"
#include "svncpp/status.hpp"
#include "svncpp/url.hpp"

// app
#include "file_info.hpp"
#include "utils.hpp"

static void
info_print_time(apr_time_t atime, const wxChar * desc, wxString & str)
{
  apr_time_exp_t extime;
  apr_status_t apr_err;

  /* if this returns an error, just don't print anything out */
  apr_err = apr_time_exp_tz(&extime, atime, 0);
  if (!apr_err)
    str.Printf(wxT("%s: %04lu-%02lu-%02lu %02lu:%02lu GMT"), desc,
               (unsigned long)(extime.tm_year + 1900),
               (unsigned long)(extime.tm_mon + 1),
               (unsigned long)(extime.tm_mday),
               (unsigned long)(extime.tm_hour),
               (unsigned long)(extime.tm_min));
}

struct FileInfo::Data
{
  svn::Context * context;
  svn::PathVector targets;
  wxString info;
  bool withUpdate;

  Data(svn::Context * ctx, bool update)
      : context(ctx), withUpdate(update)
  {
  }

  void
  addLine(const wxString & line = wxEmptyString)
  {
    info += line;
    info += wxT("\n");
  }

  void
  addInfoForStatus(const svn::Status & status)
  {
    const svn::Entry entry = status.entry();
    wxString str, tmp;

    if (!entry.isValid())
    {
      addLine(_("Not versioned"));
      return;
    }

    svn_boolean_t text_conflict = FALSE;
    svn_boolean_t props_conflict = FALSE;

    str.Printf(_("Name: %s"), Utf8ToLocal(svn::Url::unescape(entry.name())).c_str());
    addLine(str);

    str.Printf(_("URL: %s"), Utf8ToLocal(svn::Url::unescape(entry.url())).c_str());
    addLine(str);

    if (entry.repos())
      tmp = Utf8ToLocal(svn::Url::unescape(entry.repos()));
    else
      tmp = _("<None>");

    str.Printf(_("Repository: %s"), tmp.c_str());
    addLine(str);

    if (entry.uuid()) {
      tmp = Utf8ToLocal(entry.uuid());
    } else {
      tmp = _("<None>");
    }
    str.Printf(_("Repository UUID: %s"),  tmp.c_str());
    addLine(str);

    str.Printf(_("Revision: %ld"), entry.revision());
    addLine(str);

    wxString fmt = _("Node Kind: %s");
    switch (entry.kind())
    {
    case svn_node_file:
      str.Printf(fmt, _("file"));
      break;

    case svn_node_dir:
      str.Printf(fmt, _("directory"));
//          SVN_ERR (svn_wc_conflicted_p (&text_conflict, &props_conflict,
//                                        path.c_str (), entry, pool));
      break;

    case svn_node_none:
      str.Printf(fmt, _("none"));
      break;

    case svn_node_unknown:
    default:
      str.Printf(fmt, _("unknown"));
      break;
    }
    addLine(str);

    fmt = _("Schedule: %s");
    switch (entry.schedule())
    {
    case svn_wc_schedule_normal:
      str.Printf(fmt, _("normal"));
      break;

    case svn_wc_schedule_add:
      str.Printf(fmt, _("add"));
      break;

    case svn_wc_schedule_delete:
      str.Printf(fmt, _("delete"));
      break;

    case svn_wc_schedule_replace:
      str.Printf(fmt, _("replace"));
      break;

    default:
      str.Printf(fmt, _("unknown"));
      break;
    }

    if (entry.isCopied())
    {
      tmp = Utf8ToLocal(entry.copyfromUrl());
      str.Printf(_("Copied From URL: %s"), tmp.c_str());
      addLine(str);

      str.Printf(_("Copied From Rev: %ld"), entry.copyfromRev());
      addLine(str);
    }

    tmp = Utf8ToLocal(entry.cmtAuthor());
    str.Printf(_("Last Changed Author: %s"), tmp.c_str());
    addLine(str);

    str.Printf(_("Last Changed Rev: %ld"), entry.cmtRev());
    addLine(str);

    info_print_time(entry.cmtDate(), _("Last Changed Date"), str);
    addLine(str);

    info_print_time(entry.textTime(), _("Text Last Updated"), str);
    addLine(str);

    info_print_time(entry.propTime(), _("Properties Last Updated"), str);
    addLine(str);

    if (entry.checksum()) {
      tmp = Utf8ToLocal(entry.checksum());
    } else {
      tmp = _("<None>");
    }
    str.Printf(_("Checksum: %s"), tmp.c_str());
    addLine(str);

    if (text_conflict)
    {
      tmp = Utf8ToLocal(entry.conflictOld());
      str.Printf(_("Conflict BASE File: %s"),
                 tmp.c_str());
      addLine(str);

      tmp = Utf8ToLocal(entry.conflictWrk());
      str.Printf(_("Conflict Working File: %s"),
                 tmp.c_str());
      addLine(str);

      tmp = Utf8ToLocal(entry.conflictNew());
      str.Printf(_("Conflict HEAD File: %s"),
                 tmp.c_str());
      addLine(str);
    }

    if (props_conflict)
    {
      tmp = Utf8ToLocal(entry.prejfile());
      str.Printf(_("Conflict Properties File: %s"),
                 tmp.c_str());
      addLine(str);
    }

    if (status.isLocked())
    {
      tmp = Utf8ToLocal(status.lockToken());
      str.Printf(_("Lock Token: %s"),
                 tmp.c_str());
      addLine(str);

      tmp = Utf8ToLocal(status.lockOwner());
      str.Printf(_("Lock Owner: %s"),
                 tmp.c_str());
      addLine(str);

      tmp = Utf8ToLocal(status.lockComment());
      str.Printf(_("Lock Comment:\n%s"),
                 tmp.c_str());
      addLine(str);

      info_print_time(status.lockCreationDate(), _("Lock Creation Date"), str);
    }
  }

  void
  createInfoForPath(svn::Client & client, const char * path)
  {
    try
    {
      svn::StatusEntries ent = client.status(path, false, true, withUpdate);
      svn::StatusEntries::const_iterator it;

      for (it=ent.begin(); it!=ent.end(); it++)
      {
        svn::Status status(*it);

        addLine(Utf8ToLocal(status.path()));
        addInfoForStatus(status);
        addLine();
      }
    }
    catch (svn::Exception & e)
    {
      addLine(_("Error retrieving status:"));
      addLine(Utf8ToLocal(e.message()));
    }
  }
};

FileInfo::FileInfo(svn::Context * context, bool withUpdate)
{
  m = new Data(context, withUpdate);
}

FileInfo::~FileInfo()
{
  delete m;
}

void
FileInfo::addPath(const char * path)
{
  m->targets.push_back(path);
}

const wxString &
FileInfo::info() const
{
  m->info.Clear();
  svn::Client client(m->context);

  svn::PathVector::const_iterator it;

  for (it = m->targets.begin(); it != m->targets.end(); it++)
  {
    svn::Path path = *it;

    m->createInfoForPath(client, path.unescape().c_str());
    m->addLine();
  }

  return m->info;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
