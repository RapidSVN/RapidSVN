#ifndef _UPDATE_ACTION_H_INCLUDED_
#define _UPDATE_ACTION_H_INCLUDED_

#include "action_thread.h"
#include "update_dlg.h"

class UpdateAction:public ActionThread
{
private:
  UpdateDlg::sData Data;

  apr_array_header_t *targets;
  wxFrame *m_pFrame;

public:
  UpdateAction (wxFrame * frame, apr_pool_t * __pool,
                 Tracer * tr, apr_array_header_t * trgts);

  void Perform ();
  void *Entry ();
};

#endif
