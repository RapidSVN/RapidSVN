/*
 * ====================================================================
 * Copyright (c) 2002, 2003 The RapidSvn Group.  All rights reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _MKDIR_ACTION_H_INCLUDED_
#define _MKDIR_ACTION_H_INCLUDED_

#include "action_thread.hpp"
#include "mkdir_dlg.hpp"

class MkdirAction:public ActionThread
{
private:
  MkdirDlg::sData m_data;
  wxFrame *m_thisframe;

public:
  MkdirAction (wxFrame * frame, Tracer * tr);

  void Perform ();
  void *Entry ();
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
