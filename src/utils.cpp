/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
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

// svncpp
#include "svncpp/client.hpp"
#include "svncpp/status.hpp"

// app
#include "include.hpp"
#include "utils.hpp"
#include "tracer.hpp"

wxString & UnixPath (wxString & path)
{
#ifdef WIN32
  /*
     wxChar  *str = path.GetWriteBuf( path.Length() );
     wxDos2UnixFilename( str );
     path.UngetWriteBuf();
   */
  path.Replace ("\\", "/");
#endif

  return path;
}

void
GetStatusText (wxString & str, svn_wc_status_kind st)
{
  switch (st)
  {
  case svn_wc_status_none:
    str = wxT ("Non-svn");
    break;
  case svn_wc_status_normal:
    str = wxT ("Normal");
    break;
  case svn_wc_status_added:
    str = wxT ("Added");
    break;
  case svn_wc_status_absent:
    str = wxT ("Absent");
    break;
  case svn_wc_status_deleted:
    str = wxT ("Deleted");
    break;
  case svn_wc_status_replaced:
    str = wxT ("Replaced");
    break;
  case svn_wc_status_modified:
    str = wxT ("Modified");
    break;
  case svn_wc_status_merged:
    str = wxT ("Merged");
    break;
  case svn_wc_status_conflicted:
    str = wxT ("Conflicted");
    break;
  case svn_wc_status_unversioned:
  default:
    str = wxT ("Unversioned");
    break;
  }
}

void
TrimString (wxString & str)
{
  str.Trim (TRUE);
  str.Trim (FALSE);
}

struct log_msg_baton
{
  const char *message;
  const char *base_dir;
};

void *
svn_cl__make_log_msg_baton (const char *message,
                            const char *base_dir, apr_pool_t * pool)
{
  log_msg_baton *baton = (log_msg_baton *) apr_palloc (pool, sizeof (*baton));

  baton->message = message;
  baton->base_dir = base_dir ? base_dir : ".";

  return baton;
}

void
handle_svn_error (svn_error_t * err, Tracer * tracer)
{
  char buf[200];
  wxString str;

  /* Is this a Subversion-specific error code? */
  if ((err->apr_err > APR_OS_START_USEERR)
      && (err->apr_err <= APR_OS_START_CANONERR))
  {
    str.Printf (_T ("svn_error: #%d : <%s>"), err->apr_err,
                svn_strerror (err->apr_err, buf, sizeof (buf)));
    tracer->Trace (str);
  }
  /* Otherwise, this must be an APR error code. */
  else
  {
    str.Printf (_T ("apr_error: #%d : <%s>"),
                err->apr_err,
                apr_strerror (err->apr_err, buf, sizeof (buf)));
    tracer->Trace (str);
  }

  if (err->message)
  {
    str.Printf (_T ("  %s"), err->message);
    tracer->Trace (str);
  }

  if (err->child)
    handle_svn_error (err->child, tracer);
}

svn_error_t *
svn_cl__may_need_force (svn_error_t * err)
{
  if (err
      && (err->apr_err == SVN_ERR_CLIENT_UNVERSIONED ||
          err->apr_err == SVN_ERR_CLIENT_MODIFIED))
  {
    err = svn_error_quick_wrap (err,
                                "Check 'Force' to override this restriction");
  }

  return err;
}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
