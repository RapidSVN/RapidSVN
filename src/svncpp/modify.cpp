
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
Modify::checkout (const char * moduleName, const char *destPath, 
                  long revision, bool recurse)
{
  if(_notify == NULL)
    return;

  Err = svn_client_checkout (Notify::notify,
                             (void *) _notify,
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
Modify::notification (Notify * notify)
{
  _notify = notify;
}

void
Modify::remove (const char * path, bool force)
{
  svn_client_commit_info_t *commit_info = NULL;

  Err = svn_client_delete (&commit_info, path, NULL, force,
                           authenticate (),
                           &svn_cl__get_log_message,
                           logMessage (NULL),
                           Notify::notify, (void *) _notify, pool);
  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::revert (const char * path, bool recurse)
{
  Err = svn_client_revert (path, recurse, Notify::notify, 
                           (void *) _notify, pool);

  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::add (const char * path, bool recurse)
{
  Err = svn_client_add (path, recurse, Notify::notify, 
                        (void *) _notify, pool);

  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::update (const char * path, long revision, bool recurse)
{
  Err = svn_client_update (authenticate (),
                           path,
                           NULL,
                           getRevision (revision),
                           recurse,
                           Notify::notify,
                           (void *) _notify,
                           pool);
  if(Err != NULL)
    throw ClientException (Err);
}

long
Modify::commit (const char * path, const char * message, bool recurse)
{
  svn_client_commit_info_t *commit_info = NULL;
  svn_revnum_t revnum = SVN_INVALID_REVNUM;

  Err = svn_client_commit (&commit_info, Notify::notify, 
                           (void *) _notify, 
                           authenticate (), target (path), 
                           &svn_cl__get_log_message, 
                           logMessage (message), NULL, revnum, 
                           !recurse,
                           pool);
  if(Err != NULL)
    throw ClientException (Err);

  if(commit_info && SVN_IS_VALID_REVNUM (commit_info->revision))
    return commit_info->revision;

  return -1;
}

void
Modify::copy (const char * srcPath, const char * destPath)
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
                         Notify::notify, 
                         (void *) _notify,
                         pool);
  
  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::move (const char * srcPath, const char * destPath, 
              long revision, bool force)
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
                         Notify::notify,
                         (void *) _notify,
                         pool);
  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::mkdir (const char * path, const char * message)
{
  svn_client_commit_info_t *commit_info = NULL;

  Err = svn_client_mkdir (&commit_info,
                          path,
                          authenticate (),
                          &svn_cl__get_log_message,
                          logMessage (message),
                          Notify::notify,
                          (void *) _notify,
                          pool);

  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::cleanup (const char * path)
{
  Err = svn_client_cleanup (path, pool);

  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::resolve (const char * path, bool recurse)
{
  Err = svn_client_resolve (path,
                            Notify::notify,
                            (void *) _notify,
                            recurse,
                            pool);

  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::doExport (const char * srcPath, const char * destPath, 
                  long revision)
{
  Err = svn_client_export (srcPath,
                           destPath,
                           getRevision (revision),
                           authenticate (),
                           Notify::notify,
                           (void *) _notify,
                           pool);

  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::doSwitch (const char * path, const char * url, 
                  long revision, bool recurse)
{
  Err = svn_client_switch (authenticate (),
                           path,
                           url,
                           getRevision (revision),
                           recurse,
                           Notify::notify,
                           (void *) _notify,
                           pool);

  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::import (const char * path, const char * url, const char * newEntry,
                const char * message, bool recurse)
{
  svn_client_commit_info_t *commit_info = NULL;

  Err = svn_client_import (&commit_info,
                           Notify::notify,
                           (void *) _notify,
                           authenticate (),   
                           path,
                           url,
                           newEntry,
                           &svn_cl__get_log_message,
                           logMessage (message),
                           NULL,
                           SVN_INVALID_REVNUM,
                           !recurse,
                           pool);

  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::merge (const char * path1, long revision1, 
               const char * path2, long revision2,
               const char * localPath, bool force, bool recurse)
{
  Err = svn_client_merge (Notify::notify,
                          (void *) _notify,
                          authenticate (),
                          path1,
                          getRevision (revision1),
                          path2,
                          getRevision (revision2),
                          localPath,
                          recurse,
                          force,
                          pool);

  if(Err != NULL)
    throw ClientException (Err);
}

apr_array_header_t *
Modify::target (const char * path)
{
  apr_array_header_t *targets = apr_array_make (pool,
                                                DEFAULT_ARRAY_SIZE,
                                                sizeof (const char *));

  const char * target = apr_pstrdup (pool, path);
  (*((const char **) apr_array_push (targets))) = target;

  return targets;
}

void *
Modify::logMessage (const char * message, char * baseDirectory)
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
