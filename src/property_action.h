
#ifndef _PROPERTY_ACTION_H_INCLUDED_
#define _PROPERTY_ACTION_H_INCLUDED_

#include "action_thread.h"
#include "property_dlg.h"

class PropertyAction : public ActionThread
{
private:
  wxFrame *thisframe;
  const char * _target;
  PropertyDlg * propDialog;

public:
  PropertyAction (wxFrame * frame, apr_pool_t * __pool,
                  Tracer * tr, const char * target);

  void Perform ();
  void *Entry ();
};

#endif
