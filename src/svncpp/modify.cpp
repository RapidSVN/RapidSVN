#include "modify.h"

struct log_msg_baton
{
  const char *message;
  const char *base_dir;
};

namespace Svn
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
Modify::Revision (long revNumber)
{
  if(revNumber == -1)
    rev.kind = svn_client_revision_unspecified;
  else
    rev.kind = svn_client_revision_number;

  rev.value.number = revNumber;

  return &rev;
}

bool
Modify::Checkout (char * moduleName, char *destPath, long revision, 
                  bool recurse)
{
  if(notify_func == NULL)
    return false;

  Err = svn_client_checkout (notify_func,
                             notify_baton,
                             Authenticate (),
                             moduleName,
                             destPath,
                             Revision (revision),
                             recurse,
                             NULL,
                             pool);

  if(Err != NULL)
    return false;

  return true;
}

void
Modify::Notification (svn_wc_notify_func_t function, void * baton)
{
  notify_func = function;
  notify_baton = baton;
}

bool
Modify::Delete (const char * path, bool force)
{
  svn_client_commit_info_t *commit_info = NULL;

  Err = svn_client_delete (&commit_info, path, NULL, force,
                           Authenticate (),
                           &svn_cl__get_log_message,
                           LogMessage (NULL),
                           notify_func, notify_baton, pool);
  if(Err != NULL)
    return false;

  return true;
}

bool
Modify::Revert (const char * path, bool recurse)
{
  Err = svn_client_revert (path, recurse, notify_func, 
                           notify_baton, pool);

  if(Err != NULL)
    return false;

  return true;
}

bool
Modify::Add (char * path, bool recurse)
{
  Err = svn_client_add (path, recurse, notify_func, 
                        notify_baton, pool);

  if(Err != NULL)
    return false;

  return true;
}

bool
Modify::Update (char * path, long revision, bool recurse)
{
  Err = svn_client_update (Authenticate (),
                           path,
                           NULL,
                           Revision (revision),
                           recurse,
                           notify_func,
                           notify_baton,
                           pool);
  if(Err != NULL)
    return false;

  return true;
}

bool
Modify::Commit (char * path, char * logMessage, bool recurse)
{
  svn_client_commit_info_t *commit_info = NULL;
  svn_revnum_t revnum = SVN_INVALID_REVNUM;

  Err = svn_client_commit (&commit_info, notify_func, 
                           notify_baton, 
                           Authenticate (), Target (path), 
                           &svn_cl__get_log_message, 
                           LogMessage (logMessage), NULL, revnum, 
                           !recurse,
                           pool);
  if(Err != NULL)
    return false;

  return true;
}

bool
Modify::Copy (char * srcPath, char * destPath)
{
  svn_client_commit_info_t *commit_info = NULL;

  Err = svn_client_copy (&commit_info,
                         srcPath,
                         Revision (-1),
                         destPath,
                         NULL,
                         Authenticate (),
                         &svn_cl__get_log_message,
                         LogMessage (NULL),
                         notify_func, 
                         notify_baton,
                         pool);
  
  if(Err != NULL)
    return false;

  return true;
}

bool
Modify::Move (char * srcPath, char * destPath, long revision, bool force)
{
  svn_client_commit_info_t *commit_info = NULL;

  Err = svn_client_move (&commit_info,
                         srcPath,
                         Revision (-1),
                         destPath,
                         force,
                         Authenticate (),
                         &svn_cl__get_log_message,
                         LogMessage (NULL),
                         notify_func,
                         notify_baton,
                         pool);
  if(Err != NULL)
    return false;

  return true;
}

apr_array_header_t *
Modify::Target (char * path)
{
  apr_array_header_t *targets = apr_array_make (pool,
                                                DEFAULT_ARRAY_SIZE,
                                                sizeof (const char *));

  const char * target = apr_pstrdup (pool, path);
  (*((const char **) apr_array_push (targets))) = target;

  return targets;
}

void *
Modify::LogMessage (char * message, char * baseDirectory)
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
