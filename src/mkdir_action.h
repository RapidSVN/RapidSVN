#ifndef _MKDIR_ACTION_H_INCLUDED_
#define _MKDIR_ACTION_H_INCLUDED_

#include "action_thread.h"

class MkdirAction:public ActionThread
{
private:
  wxString target;
  wxString user;
  wxString pass;
  wxString logMsg;

public:
   MkdirAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr);

  void Perform ();
  void *Entry ();
};

#endif
