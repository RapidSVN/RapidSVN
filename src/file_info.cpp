/*
 * ====================================================================
 * Copyright (c) 2002-2004 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// stl
#include <vector>
// wxwindows
#include "wx/wx.h"
// apr
//#include "apr_time.h"

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/exception.hpp"
#include "svncpp/path.hpp"
#include "svncpp/status.hpp"

// app
#include "file_info.hpp"


static void
info_print_time (apr_time_t atime, const char * desc, wxString & str)
{
  apr_time_exp_t extime;
  apr_status_t apr_err;

  /* if this returns an error, just don't print anything out */
  apr_err = apr_time_exp_tz (&extime, atime, 0);
  if (!apr_err)
    str.Printf ("%s: %04lu-%02lu-%02lu %02lu:%02lu GMT", desc,
                (unsigned long) (extime.tm_year + 1900),
                (unsigned long) (extime.tm_mon + 1),
                (unsigned long) (extime.tm_mday),
                (unsigned long) (extime.tm_hour),
                (unsigned long) (extime.tm_min));
}

struct FileInfo::Data
{
  svn::Context * context;
  std::vector<svn::Path> targets;
  wxString info;
  

  Data (svn::Context * ctx)
    : context (ctx)
  {
  }

  void 
  addLine (const char * line)
  {
    info += line;
    info += "\n";
  }

  void
  createInfoForPath (svn::Client & client, const char * path)
  {
    addLine (path);
    try
    {
      svn::Status status = client.singleStatus (path);

      const svn::Entry entry = status.entry ();
      wxString str;

      if (!entry.isValid ())
      {
        addLine (_("Not versioned"));
        return;
      }

      svn_boolean_t text_conflict = FALSE;
      svn_boolean_t props_conflict = FALSE;

      str.Printf (_("Name: %s"), entry.name ());
      addLine (str);

      str.Printf (_("Url: %s"), entry.url ());
      addLine (str);

      str.Printf (_("Repository: %s"), entry.repos ());
      addLine (str);

      str.Printf (_("Revision: %ld"), entry.revision ());
      addLine (str);

      wxString fmt = _("Node Kind: %s");
      switch (entry.kind ())
      {
      case svn_node_file:
        str.Printf (fmt, _("file"));
        break;

      case svn_node_dir:
        str.Printf (fmt, _("directory"));
//          SVN_ERR (svn_wc_conflicted_p (&text_conflict, &props_conflict,
//                                        path.c_str (), entry, pool));
        break;

      case svn_node_none:
        str.Printf (fmt, _("none"));
        break;

      case svn_node_unknown:
      default:
        str.Printf (fmt, _("unknown"));
        break;
      }
      addLine (str);

      fmt = _("Schedule: %s");
      switch (entry.schedule ())
      {
      case svn_wc_schedule_normal:
        str.Printf (fmt, _("normal"));
        break;

      case svn_wc_schedule_add:
        str.Printf (fmt, _("add"));
        break;

      case svn_wc_schedule_delete:
        str.Printf (fmt, _("delete"));
        break;

      case svn_wc_schedule_replace:
        str.Printf (fmt, _("replace"));
        break;

      default:
        str.Printf (fmt, _("unknown"));
        break;
      }

      if (entry.isCopied ())
      {
        str.Printf (_("Copied From Url: %s"), entry.copyfromUrl ());
        addLine (str);

        str.Printf (_("Copied From Rev: %ld"), entry.copyfromRev ());
        addLine (str);
      }

      str.Printf (_("Last Changed Author: %s"), entry.cmtAuthor ());
      addLine (str);

      str.Printf (_("Last Changed Rev: %ld"), entry.cmtRev ());
      addLine (str);

      info_print_time (entry.cmtDate (), _("Last Changed Date"), str);
      addLine (str);

      info_print_time (entry.textTime (), _("Text Last Updated"), str);
      addLine (str);

      info_print_time (entry.propTime (), _("Properties Last Updated"), str);
      addLine (str);

      str.Printf (_("Checksum: %s"), entry.checksum ());
      addLine (str);

      if (text_conflict)
      {
        str.Printf (_("Conflict Previous Base File: %s"),
                    entry.conflictOld ());
        addLine (str);

        str.Printf (_("Conflict Previous Working File: %s"),
                    entry.conflictWrk ());
        addLine (str);

        str.Printf (_("Conflict Current Base File: %s"), 
                    entry.conflictNew ());
        addLine (str);
      }

      if (props_conflict)
      {
        str.Printf (_("Conflict Properties File: %s"), 
                    entry.prejfile ());
        addLine (str);
      }
    }
    catch (svn::Exception & e)
    {
      addLine (_("Error retrieving status:"));
      addLine (e.message ());
    }
  }
};

FileInfo::FileInfo (svn::Context * context)
{
  m = new Data (context);
}

FileInfo::~FileInfo ()
{
  delete m;
}

void 
FileInfo::addPath (const char * path)
{
  m->targets.push_back (path);
}

const char *
FileInfo::info () const
{
  m->info = "";
  svn::Client client (m->context);

  std::vector<svn::Path>::const_iterator it;

  for (it = m->targets.begin (); it != m->targets.end (); it++)
  {
    svn::Path path = *it;
    svn::Status status = client.singleStatus (path.c_str ());

    m->createInfoForPath (client, path.c_str ());
    m->addLine ("");
  }

  return m->info;
}

/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
