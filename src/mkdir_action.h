#ifndef _MKDIR_ACTION_H_INCLUDED_
#define _MKDIR_ACTION_H_INCLUDED_

#include "action_thread.h"
#include "mkdir_dlg.h"

class MkdirAction:public ActionThread
{
private:
  MkdirDlg::sData Data;
  wxFrame *thisframe;

public:
  MkdirAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr);

  void Perform ();
  void *Entry ();
};

#endif
