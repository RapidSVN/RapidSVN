#ifndef _MERGE_ACTION_H_INCLUDED_
#define _MERGE_ACTION_H_INCLUDED_

#include "action_thread.h"
#include "merge_dlg.h"

class MergeAction:public ActionThread
{
private:
  MergeDlg::sData Data;
  wxFrame *m_pFrame;

  /**
   * Gets the revision from a string.
   */
  long getRevision (wxString & str);

public:
   MergeAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr);

  void Perform ();
  void *Entry ();
};

#endif

