/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */

// subversion api
#include "svn_client.h"
#include "svn_time.h"
#include "svn_utf.h"

// svncpp
#include "log.hpp"
#include "pool.hpp"
#include "targets.hpp"

struct log_message_receiver_baton
{
  bool first_call;
  std::vector<long> * revision;
  std::vector<std::string> * author;
  std::vector<std::string> * date;
  std::vector<std::string> * message;
  int * size;
};

namespace svn
{
  // forward declaration
  static svn_error_t *
  messageReceiver (void *baton,
                   apr_hash_t * changed_paths,
                   svn_revnum_t rev,
                   const char *author,
                   const char *date, 
                   const char *msg, 
                   apr_pool_t * pool);

Log::Log()
{
  reset ();
}

Log::~Log()
{
}

void
Log::reset ()
{
  cursor = -1;
  size = 0;
  versioned = false;
  m_revision.clear ();
  m_author.clear ();
  m_date.clear ();
  m_message.clear ();
}

void
Log::loadPath (const char * path, const Revision & revisionStart, 
               const Revision & revisionEnd)
{
  log_message_receiver_baton lb;
  m_lastPath = path;

  reset ();
  
  lb.revision = &m_revision;
  lb.author = &m_author;
  lb.date = &m_date;
  lb.message = &m_message;
  lb.size = &size;
  lb.first_call = true;

  Targets target (m_lastPath.c_str () );
  Pool pool;
  apr_pool_t *apr_pool = pool.pool ();

  m_Err = svn_client_log (authenticate (), target.array (apr_pool), 
                        revisionStart.revision (), 
                        revisionEnd.revision (), 
                        0, // not reverse by default
                        1, // strict by default (not showing cp info)
                        messageReceiver,
                        &lb, apr_pool);

  versioned = true;
  if(m_Err != NULL)
    versioned = false;
}

bool
Log::next ()
{
  if((cursor + 1) >= count ())
    return false;

  cursor++;
  return true;
}

bool
Log::previous ()
{
  if((cursor - 1) < 0)
    return false;

  cursor--;
  return true;
}

bool
Log::last ()
{
  if(count () < 1)
    return false;

  cursor = count () - 1;
  return true;
}

bool
Log::first ()
{
  if(count () < 1)
    return false;

  cursor = 0;
  return true;
}

void
Log::beforeFirst ()
{
  if(count () < 1)
    return;

  cursor = -1;
}

const char *
Log::message ()
{
  if(cursor == -1)
    return NULL;

  return m_message[cursor].c_str ();
}

const char *
Log::date ()
{
  if(cursor == -1)
    return NULL;

  return m_date[cursor].c_str ();
}

const char *
Log::formatDate (const char * dateText, const char * format)
{
  apr_time_t timeTemp;
  const char * date;
  char * dateNative;
  time_t time;
  char buffer[40];
  struct tm * localTime;
  svn_string_t * str;
  Pool subPool;
  apr_pool_t *apr_pool = subPool.pool();

  svn_time_from_cstring (&timeTemp, dateText, apr_pool);
  date = svn_time_to_human_cstring(timeTemp, apr_pool);
  dateNative = (char *)date;

  time = svn_parse_date (dateNative, NULL);
  localTime = localtime (&time);

  strftime (buffer, 40, format, localTime);
  str = svn_string_create (buffer, apr_pool);

  return str->data;
}

const char *
Log::author ()
{
  if(cursor == -1)
    return NULL;

  return m_author[cursor].c_str ();
}

long
Log::revision ()
{
  if(cursor == -1)
    return 0;

  return m_revision[cursor];
}

int
Log::count ()
{
  return size;
}

bool
Log::isVersioned ()
{
  return versioned;
}

static svn_error_t *
messageReceiver (void *baton,
                 apr_hash_t * changed_paths,
                 svn_revnum_t rev,
                 const char *author,
                 const char *date, 
                 const char *msg, 
                 apr_pool_t * pool)
{
  svn_error_t * error = NULL;
  log_message_receiver_baton *lb = (log_message_receiver_baton *) baton;

  lb->revision->push_back (rev);
  lb->date->push_back (date);
  lb->author->push_back (author);
  lb->message->push_back (msg);
  *lb->size = *lb->size + 1;

  return error;
}

}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
