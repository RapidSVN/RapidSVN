#ifndef _RESOLVE_ACTION_H_INCLUDED_
#define _RESOLVE_ACTION_H_INCLUDED_

#include "action_thread.h"

class ResolveAction:public ActionThread
{
private:
  apr_array_header_t * targets;

public:
  ResolveAction (wxFrame * frame, apr_pool_t * __pool,
                 Tracer * tr, apr_array_header_t * trgts);

  void Perform ();
  void *Entry ();
};

#endif
