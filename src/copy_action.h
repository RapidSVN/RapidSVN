#ifndef _COPY_ACTION_H_INCLUDED_
#define _COPY_ACTION_H_INCLUDED_

#include "action_thread.h"
#include "svncpp/modify.h"

class CopyAction:public ActionThread
{
private:
  wxString dest;
  wxString src;
  wxString logMsg;
  apr_array_header_t *targets;
  wxString DestinationPath (wxString src);
  apr_pool_t * pool;

public:
  CopyAction (wxFrame * frame, apr_pool_t * __pool, 
               Tracer * tr, apr_array_header_t * trgts);

  void Perform ();
  void *Entry ();
};

#endif
