
#include "error.h"

namespace svn
{

Error::Error ()
{
  err = NULL;
}

Error::~Error ()
{
}

void
Error::setError (svn_error_t * error)
{
  err = error;
}

bool
Error::exists ()
{
  if(err != NULL)
    return true;

  return false;
}

const char * 
Error::message ()
{
  if(err == NULL)
    return NULL;

  return err->message;
}

const char * 
Error::source ()
{
  if(err == NULL)
    return NULL;

  return err->file;
}

int
Error::aprError ()
{
  if(err == NULL)
    return -1;

  return err->apr_err;
}

}