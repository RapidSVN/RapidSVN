#ifndef _DELETE_ACTION_H_INCLUDED_
#define _DELETE_ACTION_H_INCLUDED_

#include "action_thread.h"
#include "delete_dlg.h"

class DeleteAction:public ActionThread
{
private:
  DeleteDlg::sData Data;
  wxFrame *m_pFrame;

  apr_array_header_t *targets;

public:
   DeleteAction (wxFrame * frame, apr_pool_t * __pool,
                 Tracer * tr, apr_array_header_t * trgts);

  void Perform ();
  void *Entry ();
};

#endif
