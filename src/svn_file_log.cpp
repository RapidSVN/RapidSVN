#include "include.h"
#include "svn_sorts.h"
#include "auth_baton.h"
#include "svn_file_log.h"

struct log_message_receiver_baton
{
  svn_boolean_t first_call;
  wxString *str;
};

static int
num_lines (const char *msg)
{
  int count = 1;
  const char *p;

  for (p = msg; *p; p++)
  {
    if (*p == '\n')
    {
      count++;
      if (*(p + 1) == '\r')
        p++;
    }
    else if (*p == '\r')
    {
      count++;
      if (*(p + 1) == '\n')
        p++;
    }
  }

  return count;
}

/* Maximum length of a human-readable date string, including final '\0'. */
#define SVN_LOG__DATE_MAX 38

/**
 * Transforms a date from a format like this:
 *
 * "Sat 2 Mar 2002 20:41:01.695108 (day 061, dst 0, gmt_off -21600)",
 * provided by svn_time_to_string()
 *
 * to something like this:
 *
 *  "Sat 2 Mar 2002 20:41:01"
 */
static void
transform_date (char *result, const char *date)
{
  const char *p = strchr (date, '.');

  if (p && ((p - date) < SVN_LOG__DATE_MAX))
  {
    strncpy (result, date, (p - date));
    result[p - date] = '\0';
  }
  else
  {
    strncpy (result, date, (SVN_LOG__DATE_MAX - 1));
    result[SVN_LOG__DATE_MAX - 1] = '\0';
  }
}

static svn_error_t *
log_message_receiver (void *baton,
                      apr_hash_t * changed_paths,
                      svn_revnum_t rev,
                      const char *author,
                      const char *date, const char *msg, apr_pool_t * pool)
{
  log_message_receiver_baton *lb = (log_message_receiver_baton *) baton;
  wxString s;
  wxString *str = lb->str;

  /* Number of lines in the msg. */
  int lines;

  /* As much date as we ever want to see. */
  char dbuf[SVN_LOG__DATE_MAX];

  if (rev == 0)
  {
    str->Append (_T ("No commits in repository.\n"));
    return SVN_NO_ERROR;
  }

  transform_date (dbuf, date);

#define SEP_STRING \
 _T("------------------------------------------------------------------------\n")

  if (lb->first_call)
  {
    str->Append (SEP_STRING);
    lb->first_call = 0;
  }

  lines = num_lines (msg);
  s.Printf (_T ("rev %" SVN_REVNUM_T_FMT ":  %s | %s | %d line%s\n"),
            rev, author, dbuf, lines, (lines > 1) ? _T ("s") : _T (""));
  str->Append (s);

  if (changed_paths)
  {
    apr_array_header_t *sorted_paths;
    int i;

    /* Get an array of sorted hash keys. */
    sorted_paths = apr_hash_sorted_keys (changed_paths,
                                         svn_sort_compare_items_as_paths,
                                         pool);

    str->Append (_T ("Changed paths:\n"));
    for (i = 0; i < sorted_paths->nelts; i++)
    {
      svn_item_t *item = &(APR_ARRAY_IDX (sorted_paths, i, svn_item_t));
      const char *path = (const char *) item->key;
      char action = (char) ((int) apr_hash_get (changed_paths,
                                                item->key, item->klen));
      s.Printf (_T ("   %c %s\n"), (action == 'R' ? 'U' : action), path);
      str->Append (s);
    }
  }

  str->Append (_T ("\n"));
  str->Append (msg);
  str->Append (_T ("\n"));
  str->Append (SEP_STRING);

  return SVN_NO_ERROR;
}

svn_error_t *
svn_cl_log (apr_array_header_t * targets,
            wxString * str_all, apr_pool_t * pool)
{
  svn_client_revision_t rev_start;
  svn_client_revision_t rev_end;
  AuthBaton auth_baton (pool);
  log_message_receiver_baton lb;

  memset (&rev_start, 0, sizeof (rev_start));
  memset (&rev_end, 0, sizeof (rev_end));

  rev_start.kind = svn_client_revision_head;
  rev_end.kind = svn_client_revision_number;
  rev_end.value.number = 1;     // oldest commit

  lb.first_call = 1;
  lb.str = str_all;

  SVN_ERR (svn_client_log (auth_baton.auth_obj, targets, &(rev_start), &(rev_end), 0,   // not verbose by default
                           1,   // strict by default (not showing cp info)
                           log_message_receiver, &lb, pool));

  return SVN_NO_ERROR;
}
