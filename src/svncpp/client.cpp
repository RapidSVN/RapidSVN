#include "client.h"

namespace svn
{

Client::Client ()
{
  pool = svn_pool_create (NULL);
}

Client::~Client ()
{
  svn_pool_destroy (pool);
}

}
