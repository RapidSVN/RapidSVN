#ifndef _CHECKOUT_ACTION_H_INCLUDED_
#define _CHECKOUT_ACTION_H_INCLUDED_

#include "action_thread.h"
#include "checkout_dlg.h"

class CheckoutAction:public ActionThread
{
private:
  CheckoutDlg::sData Data;
  wxFrame *m_pFrame;

public:
   CheckoutAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr);

  void Perform ();
  void *Entry ();
};

#endif
