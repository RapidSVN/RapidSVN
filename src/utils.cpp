
#include "svncpp/status.h"
#include "include.h"
#include "utils.h"

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
    str.Printf (_T ("apr_error: #%d, src_err %d : <%s>"),
                err->apr_err,
                err->src_err, apr_strerror (err->apr_err, buf, sizeof (buf)));
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

void
statusString (const wxString & path, wxString & line)
{
  svn::Status status;
  wxChar text_statuschar;
  wxChar prop_statuschar;
  wxString newPath = path;

  wxString str_status;
  wxString str_rev;
  wxString last_committed;

  line.Empty ();

  status.loadPath (UnixPath (newPath));

  switch (status.textType ())
  {
  case svn_wc_status_none:
    text_statuschar = ' ';
    break;
  case svn_wc_status_normal:
    text_statuschar = '_';
    break;
  case svn_wc_status_added:
    text_statuschar = 'A';
    break;
  case svn_wc_status_absent:
    text_statuschar = '!';
    break;
  case svn_wc_status_deleted:
    text_statuschar = 'D';
    break;
  case svn_wc_status_replaced:
    text_statuschar = 'R';
    break;
  case svn_wc_status_modified:
    text_statuschar = 'M';
    break;
  case svn_wc_status_merged:
    text_statuschar = 'G';
    break;
  case svn_wc_status_conflicted:
    text_statuschar = 'C';
    break;
  case svn_wc_status_unversioned:
  default:
    text_statuschar = '?';
    break;
  }

  switch (status.propType ())
  {
  case svn_wc_status_none:
    prop_statuschar = ' ';
    break;
  case svn_wc_status_normal:
    prop_statuschar = '_';
    break;
  case svn_wc_status_added:
    prop_statuschar = 'A';
    break;
  case svn_wc_status_absent:
    prop_statuschar = '!';
    break;
  case svn_wc_status_deleted:
    prop_statuschar = 'D';
    break;
  case svn_wc_status_replaced:
    prop_statuschar = 'R';
    break;
  case svn_wc_status_modified:
    prop_statuschar = 'M';
    break;
  case svn_wc_status_merged:
    prop_statuschar = 'G';
    break;
  case svn_wc_status_conflicted:
    prop_statuschar = 'C';
    break;
  case svn_wc_status_unversioned:
  default:
    prop_statuschar = '?';
    break;
  }

  str_status.Printf ("%c%c", text_statuschar, prop_statuschar);

  if (status.isVersioned ())
  {
    wxString revbuf;
    const char *revstr = revbuf;
    const char *author;

    str_status.Printf ("%c%c", status.isLocked () ? 'L' : ' ',
                       status.isCopied () ? '+' : ' ');

    author = status.lastCommitAuthor ();
    if (status.isVersioned ())
      revbuf.Printf ("%" SVN_REVNUM_T_FMT, status.lastChanged () );
    else
      revstr = "    ? ";

    /* ### we shouldn't clip the revstr and author, but that implies a
       ### variable length 'last_committed' which means an allocation,
       ### which means a pool, ...  */
    last_committed.Printf ("%6.6s   %8.8s   ",
                           revstr, author ? author : "      ? ");
  }
  else
    last_committed = _T ("                    ");

  /* Determine the appropriate local revision string. */
  if (!status.isVersioned ())
    str_rev = _T ("  ?   ");
  else if (status.isCopied ())
    str_rev = _T ("     -");
  else
    str_rev.Printf ("%6ld", status.revision ());

  /* One Printf to rule them all, one Printf to bind them..." */
  line = str_status + _T ("   ") +
    str_rev + _T ("   ") + last_committed + path;
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
