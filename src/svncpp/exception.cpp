
#include "exception.h"

namespace svn
{

// Generic Exception implementation
Exception::Exception (std::string &message)  throw () : 
  _message(message)
{
}

Exception::~Exception () throw ()
{
}

const char *
Exception::message ()
{
  return _message.c_str ();
}

// ClientException implementation

ClientException::ClientException (svn_error_t * error, 
                                  std::string message) throw () : 
  Exception (message), _error(error)
{
}

ClientException::~ClientException () throw ()
{
}

const char *
ClientException::description ()
{
  return _error->message;
}

const char *
ClientException::source ()
{
  return _error->file;
}

int
ClientException::aprError ()
{
  return _error->apr_err;
}

}
