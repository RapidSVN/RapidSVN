#ifndef _CHECKOUT_ACTION_H_INCLUDED_
#define _CHECKOUT_ACTION_H_INCLUDED_

#include "action_thread.h"

class CheckoutAction:public ActionThread
{
private:
  wxString destinationFolder;
  wxString moduleName;
  wxString user;
  wxString pass;
  svn_boolean_t recursive;
  unsigned long revnum;

     /**
     * This is true if the revision edit box
     * from the dialog was not empty. Otherwise,
     * it is false, too indicate that the rev passed to
     * svn_client_checkout must remain unspecified, so the
     * last revision in the repository should be extracted.
     */
  bool rev_specified;

public:
   CheckoutAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr);

  void Perform ();
  void *Entry ();
};

#endif
