
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
}

Modify::~Modify ()
{
}

void
Modify::checkout (const char * moduleName, const char *destPath, 
                  long revision, bool recurse)
{
  lastPath = destPath;
  internalPath (lastPath);

  if(_notify == NULL)
    return;

  Err = svn_client_checkout (Notify::notify,
                             (void *) _notify,
                             authenticate (),
                             moduleName,
                             lastPath.c_str (),
                             getRevision (revision),
                             recurse,
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
  lastPath = path;
  internalPath (lastPath);

  Err = svn_client_delete (&commit_info, lastPath.c_str (), NULL, force,
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
  lastPath = path;
  internalPath (lastPath);
  Err = svn_client_revert (lastPath.c_str (), recurse, Notify::notify, 
                           (void *) _notify, pool);

  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::add (const char * path, bool recurse)
{
  lastPath = path;
  internalPath (lastPath);
  Err = svn_client_add (lastPath.c_str (), recurse, Notify::notify, 
                        (void *) _notify, pool);

  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::update (const char * path, long revision, bool recurse)
{
  lastPath = path;
  internalPath (lastPath);
  Err = svn_client_update (authenticate (),
                           lastPath.c_str (),
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
  lastPath = path;
  internalPath (lastPath);

  Err = svn_client_commit (&commit_info, Notify::notify, 
                           (void *) _notify, 
                           authenticate (), target (lastPath.c_str ()), 
                           &svn_cl__get_log_message, 
                           logMessage (message), !recurse, pool);
  if(Err != NULL)
    throw ClientException (Err);

  if(commit_info && SVN_IS_VALID_REVNUM (commit_info->revision))
    return commit_info->revision;

  return -1;
}

void
Modify::copy (const char * srcPath, const char * destPath)
{
  std::string sourcePath = srcPath;
  lastPath = destPath;

  internalPath (sourcePath);
  internalPath (lastPath);
  svn_client_commit_info_t *commit_info = NULL;

  Err = svn_client_copy (&commit_info,
                         sourcePath.c_str (),
                         getRevision (-1),
                         lastPath.c_str (),
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
  std::string sourcePath = srcPath;
  lastPath = destPath;

  internalPath (sourcePath);
  internalPath (lastPath);

  Err = svn_client_move (&commit_info,
                         sourcePath.c_str (),
                         getRevision (-1),
                         lastPath.c_str (),
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
  lastPath = path;
  internalPath (lastPath);

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
  lastPath = path;
  internalPath (lastPath);
  Err = svn_client_cleanup (lastPath.c_str (), pool);

  if(Err != NULL)
    throw ClientException (Err);
}

void
Modify::resolve (const char * path, bool recurse)
{
  lastPath = path;
  internalPath (lastPath);
  Err = svn_client_resolve (lastPath.c_str (),
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
  std::string sourcePath = srcPath;
  lastPath = destPath;
  internalPath (sourcePath);
  internalPath (lastPath);
  Err = svn_client_export (sourcePath.c_str (),
                           lastPath.c_str (),
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
  lastPath = path;
  internalPath (lastPath);
  Err = svn_client_switch (authenticate (),
                           lastPath.c_str (),
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
  lastPath = path;
  internalPath (lastPath);
  svn_client_commit_info_t *commit_info = NULL;

  Err = svn_client_import (&commit_info,
                           Notify::notify,
                           (void *) _notify,
                           authenticate (),   
                           lastPath.c_str (),
                           url,
                           newEntry,
                           &svn_cl__get_log_message,
                           logMessage (message),
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
  lastPath = localPath;
  std::string srcPath1 = path1;
  std::string srcPath2 = path2;

  internalPath (lastPath);
  internalPath (srcPath1);
  internalPath (srcPath2);

  Err = svn_client_merge (Notify::notify,
                          (void *) _notify,
                          authenticate (),
                          srcPath1.c_str (),
                          getRevision (revision1),
                          srcPath2.c_str (),
                          getRevision (revision2),
                          localPath,
                          recurse,
                          force,
                          pool);

  if(Err != NULL)
    throw ClientException (Err);
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
