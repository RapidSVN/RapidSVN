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

// app
#include "file_action.hpp"
#include "commit_dlg.hpp"

// forward declarations
namespace svn
{
  class Targets;
}

class CommitAction:public FileAction
{
private:
  CommitDlg::sData m_data;
  wxFrame *m_thisframe;

  svn::Targets m_targets;

  // hide default and copy constructor
  CommitAction ();
  CommitAction (const CommitAction &);
public:
  CommitAction (wxFrame * frame, Tracer * tr, 
                const svn::Targets & targets);
  void Perform ();
  bool PerformUI ();
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
