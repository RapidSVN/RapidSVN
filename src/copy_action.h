#ifndef _COPY_ACTION_H_INCLUDED_
#define _COPY_ACTION_H_INCLUDED_

#include "action_thread.h"

class CopyAction:public ActionThread
{
private:
  wxString dest;
  wxString src;
  wxString logMsg;
  wxString user;
  wxString pass;
  unsigned long revnum;

     /**
     * This is true if the revision edit box
     * from the dialog was not empty. Otherwise,
     * it is false, too indicate that the rev passed to
     * svn_client_copy must remain unspecified.
     */
  bool rev_specified;

public:
   CopyAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr);

  void Perform ();
  void *Entry ();
};

#endif
