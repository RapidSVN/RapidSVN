#include "client.h"

namespace Svn
{

Client::Client ()
{
  pool = svn_pool_create (NULL);
}

Client::~Client ()
{
  svn_pool_destroy (pool);
}

svn_error_t *
Client::GetError ()
{
  return Err;
}

}
