
#include "client.h"
#include "svn_path.h"

#define DEFAULT_ARRAY_SIZE 5

namespace svn
{

Client::Client ()
{
  pool = svn_pool_create (NULL);
  memset (&rev, 0, sizeof (rev));
}

Client::~Client ()
{
  svn_pool_destroy (pool);
}

svn_opt_revision_t *
Client::getRevision (long revNumber)
{
  rev.value.number = 0;
  rev.value.date = 0;
  rev.kind = svn_opt_revision_unspecified;

  switch(revNumber)
  {
    case -1:
      rev.kind = svn_opt_revision_unspecified;
      break;
    case -2:
      rev.kind = svn_opt_revision_head;
      break;
    default:
      rev.kind = svn_opt_revision_number;
      rev.value.number = revNumber;
      break;
  }

  return &rev;
}

apr_array_header_t *
Client::target (const char * path)
{
  apr_array_header_t *targets = apr_array_make (pool,
                                                DEFAULT_ARRAY_SIZE,
                                                sizeof (const char *));

  const char * target = apr_pstrdup (pool, path);
  (*((const char **) apr_array_push (targets))) = target;

  return targets;
}

const char *
Client::getLastPath ()
{
  return lastPath.c_str ();
}

void
Client::internalPath (std::string & path)
{
  svn_stringbuf_t * buf = svn_stringbuf_create (path.c_str (), pool);
  svn_path_internal_style (buf);
  path = buf->data;
}

}
