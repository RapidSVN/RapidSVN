
#include "log.h"

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
  _revision.clear ();
  _author.clear ();
  _date.clear ();
  _message.clear ();
}

void
Log::loadPath (const char * path, long revisionStart, 
               long revisionEnd)
{
  const apr_array_header_t * targets = NULL;
  void * baton = NULL;
  log_message_receiver_baton lb;
  svn_opt_revision_t revEnd;

  reset ();
  memset (&revEnd, 0, sizeof (revEnd));
  revEnd.kind = svn_opt_revision_number;
  revEnd.value.number = revisionEnd;
  
  lb.revision = &_revision;
  lb.author = &_author;
  lb.date = &_date;
  lb.message = &_message;
  lb.size = &size;
  lb.first_call = true;

  Err = svn_client_log (authenticate (), target (path), 
                        getRevision (revisionStart), 
                        &revEnd, 
                        0, // not reverse by default
                        1, // strict by default (not showing cp info)
                        messageReceiver,
                        &lb, pool);

  versioned = true;
  if(Err != NULL)
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

const char *
Log::message ()
{
  if(cursor == -1)
    return NULL;

  return _message[cursor].c_str ();
}

const char *
Log::date ()
{
  if(cursor == -1)
    return NULL;

  return _date[cursor].c_str ();
}

const char *
Log::author ()
{
  if(cursor == -1)
    return NULL;

  return _author[cursor].c_str ();
}

unsigned long
Log::revision ()
{
  if(cursor == -1)
    return 0;

  return _revision[cursor];
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
