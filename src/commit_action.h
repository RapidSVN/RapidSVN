#ifndef _COMMIT_ACTION_H_INCLUDED_
#define _COMMIT_ACTION_H_INCLUDED_

#include "action_thread.h"

class CommitAction:public ActionThread
{
private:
  wxString user;
  wxString pass;
  wxString logMsg;
  bool recursive;

  apr_array_header_t *targets;

public:
   CommitAction (wxFrame * frame, apr_pool_t * __pool,
                 Tracer * tr, apr_array_header_t * trgts);

  void Perform ();
  void *Entry ();
};

#endif
