/*
 * ====================================================================
 * Copyright (c) 2002 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _COMMIT_ACTION_H_INCLUDED_
#define _COMMIT_ACTION_H_INCLUDED_

#include "file_action.hpp"
#include "commit_dlg.hpp"

class CommitAction:public FileAction
{
private:
  CommitDlg::sData m_data;
  wxFrame *m_thisframe;

  apr_array_header_t *m_targets;

public:
  CommitAction (wxFrame * frame, 
                Tracer * tr, apr_array_header_t * targets);
  void Perform ();
  bool PerformUI ();
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
