
#include "modify.h"

struct log_msg_baton
{
  const char *message;
  const char *base_dir;
};

namespace svn
{

Modify::Modify ()
{
  notify_func = NULL;
  notify_baton = NULL;
  memset (&rev, 0, sizeof (rev));
}

Modify::~Modify ()
{
}

svn_client_revision_t *
Modify::getRevision (long revNumber)
{
  if(revNumber == -1)
    rev.kind = svn_client_revision_unspecified;
  else
    rev.kind = svn_client_revision_number;

  rev.value.number = revNumber;

  return &rev;
}

void
Modify::checkout (char * moduleName, char *destPath, long revision, 
                  bool recurse)
{
  if(notify_func == NULL)
    return;

  Err = svn_client_checkout (notify_func,
                             notify_baton,
                             authenticate (),
                             moduleName,
                             destPath,
                             getRevision (revision),
                             recurse,
                             NULL,
                             pool);

  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::notification (svn_wc_notify_func_t function, void * baton)
{
  notify_func = function;
  notify_baton = baton;
}

void
Modify::remove (const char * path, bool force)
{
  svn_client_commit_info_t *commit_info = NULL;

  Err = svn_client_delete (&commit_info, path, NULL, force,
                           authenticate (),
                           &svn_cl__get_log_message,
                           logMessage (NULL),
                           notify_func, notify_baton, pool);
  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::revert (const char * path, bool recurse)
{
  Err = svn_client_revert (path, recurse, notify_func, 
                           notify_baton, pool);

  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::add (char * path, bool recurse)
{
  Err = svn_client_add (path, recurse, notify_func, 
                        notify_baton, pool);

  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::update (char * path, long revision, bool recurse)
{
  Err = svn_client_update (authenticate (),
                           path,
                           NULL,
                           getRevision (revision),
                           recurse,
                           notify_func,
                           notify_baton,
                           pool);
  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::commit (char * path, char * message, bool recurse)
{
  svn_client_commit_info_t *commit_info = NULL;
  svn_revnum_t revnum = SVN_INVALID_REVNUM;

  Err = svn_client_commit (&commit_info, notify_func, 
                           notify_baton, 
                           authenticate (), target (path), 
                           &svn_cl__get_log_message, 
                           logMessage (message), NULL, revnum, 
                           !recurse,
                           pool);
  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::copy (char * srcPath, char * destPath)
{
  svn_client_commit_info_t *commit_info = NULL;

  Err = svn_client_copy (&commit_info,
                         srcPath,
                         getRevision (-1),
                         destPath,
                         NULL,
                         authenticate (),
                         &svn_cl__get_log_message,
                         logMessage (NULL),
                         notify_func, 
                         notify_baton,
                         pool);
  
  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::move (char * srcPath, char * destPath, long revision, bool force)
{
  svn_client_commit_info_t *commit_info = NULL;

  Err = svn_client_move (&commit_info,
                         srcPath,
                         getRevision (-1),
                         destPath,
                         force,
                         authenticate (),
                         &svn_cl__get_log_message,
                         logMessage (NULL),
                         notify_func,
                         notify_baton,
                         pool);
  if(Err != NULL)
    throw ClientException (Err);
}

apr_array_header_t *
Modify::target (char * path)
{
  apr_array_header_t *targets = apr_array_make (pool,
                                                DEFAULT_ARRAY_SIZE,
                                                sizeof (const char *));

  const char * target = apr_pstrdup (pool, path);
  (*((const char **) apr_array_push (targets))) = target;

  return targets;
}

void *
Modify::logMessage (char * message, char * baseDirectory)
{
  log_msg_baton *baton = (log_msg_baton *) 
                         apr_palloc (pool, sizeof (*baton));

  baton->message = message;
  baton->base_dir = baseDirectory ? baseDirectory : ".";

  return baton;
}

}

svn_error_t *
svn_cl__get_log_message (const char **log_msg,
                         apr_array_header_t * commit_items,
                         void *baton, apr_pool_t * pool)
{
  log_msg_baton *lmb = (log_msg_baton *) baton;

  if (lmb->message)
  {
    *log_msg = apr_pstrdup (pool, lmb->message);
    return SVN_NO_ERROR;
  }

  return SVN_NO_ERROR;
}
