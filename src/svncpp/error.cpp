
#include "error.h"

namespace Svn
{

Error::Error ()
{
  err = NULL;
}

Error::~Error ()
{
}

void
Error::SetError (svn_error_t * error)
{
  err = error;
}

bool
Error::Exists ()
{
  if(err != NULL)
    return true;

  return false;
}

const char * 
Error::Message ()
{
  if(err == NULL)
    return NULL;

  return err->message;
}

const char * 
Error::Source ()
{
  if(err == NULL)
    return NULL;

  return err->file;
}

int
Error::AprError ()
{
  if(err == NULL)
    return -1;

  return err->apr_err;
}

}