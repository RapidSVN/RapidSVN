
#ifndef _LOG_ACTION_H_INCLUDED_
#define _LOG_ACTION_H_INCLUDED_

#include "action_thread.h"
#include "log_dlg.h"

class LogAction : public ActionThread
{
private:
  wxFrame *thisframe;
  const char * _target;
  LogDlg *  logDialog;

public:
  LogAction (wxFrame * frame, apr_pool_t * __pool,
             Tracer * tr, const char * target);

  void Perform ();
  void *Entry ();
  void setLogMessage (const char * message);
};

#endif
