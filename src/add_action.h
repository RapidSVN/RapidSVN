#ifndef _ADD_ACTION_H_INCLUDED_
#define _ADD_ACTION_H_INCLUDED_

#include "action_thread.h"

class AddAction:public ActionThread
{
private:
  apr_array_header_t * targets;

public:
  AddAction (wxFrame * frame, apr_pool_t * __pool,
             Tracer * tr, apr_array_header_t * trgts);

  void Perform ();
  void *Entry ();
};

#endif
