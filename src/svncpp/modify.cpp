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
Modify::Delete (const char * sPath, bool force)
{
  svn_client_commit_info_t *commit_info = NULL;

  Err = svn_client_delete (&commit_info, sPath, NULL, force,
                           Authenticate (),
                           &svn_cl__get_log_message,
                           LogMessage (NULL),
                           notify_func, notify_baton, pool);
  if(Err != NULL)
    return false;

  return true;
}

bool
Modify::Revert (const char * sPath, bool recurse)
{
  Err = svn_client_revert (sPath, recurse, notify_func, 
                           notify_baton, pool);

  if(Err != NULL)
    return false;

  return true;
}

bool
Modify::Add (char * sPath, bool recurse)
{
  Err = svn_client_add (sPath, recurse, notify_func, 
                        notify_baton, pool);

  if(Err != NULL)
    return false;

  return true;
}

bool
Modify::Update (char * sPath, long revision, bool recurse)
{
  Err = svn_client_update (Authenticate (),
                           sPath,
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
Modify::Commit (char * sPath, char * sLogMessage, bool recurse)
{
  svn_client_commit_info_t *commit_info = NULL;
  svn_revnum_t revnum = SVN_INVALID_REVNUM;

  Err = svn_client_commit (&commit_info, notify_func, 
                           notify_baton, 
                           Authenticate (), Target (sPath), 
                           &svn_cl__get_log_message, 
                           LogMessage (sLogMessage), NULL, revnum, 
                           !recurse,
                           pool);
  if(Err != NULL)
    return false;

  return true;
}

bool
Modify::Copy (char * sPath, char * sDestPath)
{
  svn_client_commit_info_t *commit_info = NULL;

  Err = svn_client_copy (&commit_info,
                         sPath,
                         Revision (-1),
                         sDestPath,
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
Modify::Move (char * sPath, char * sDestPath, long revision, bool force)
{
  svn_client_commit_info_t *commit_info = NULL;

  Err = svn_client_move (&commit_info,
                         sPath,
                         Revision (-1),
                         sDestPath,
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
Modify::Target (char * sTarget)
{
  apr_array_header_t *targets = apr_array_make (pool,
                                                DEFAULT_ARRAY_SIZE,
                                                sizeof (const char *));

  const char * target = apr_pstrdup (pool, sTarget);
  (*((const char **) apr_array_push (targets))) = target;

  return targets;
}

void *
Modify::LogMessage (char * sMessage, char * baseDirectory)
{
  log_msg_baton *baton = (log_msg_baton *) 
                         apr_palloc (pool, sizeof (*baton));

  baton->message = sMessage;
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
