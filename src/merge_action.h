#ifndef _MERGE_ACTION_H_INCLUDED_
#define _MERGE_ACTION_H_INCLUDED_

#include "action_thread.h"

class MergeAction:public ActionThread
{
private:
  wxString path1;
  wxString path2;

  /**
   * Gets the revision from a string.
   */
  long getRevision (wxString & str);

  long rev1;
  long rev_end;
  long rev2;

  wxString user;
  wxString pass;

  bool recursive;
  bool force;

public:
   MergeAction (wxFrame * frame, apr_pool_t * __pool, Tracer * tr);
  ~MergeAction ();

  void Perform ();
  void *Entry ();
};

#endif
