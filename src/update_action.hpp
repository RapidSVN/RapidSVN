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
#ifndef _UPDATE_ACTION_H_INCLUDED_
#define _UPDATE_ACTION_H_INCLUDED_

// wxwindows
//#include "wx/frame.h"

// svncpp
#include "svncpp/targets.hpp"

// app
//#include "action_thread.hpp"
#include "file_action.hpp"
#include "update_dlg.hpp"

class UpdateAction:public FileAction
{
private:
  UpdateDlg::sData m_data;

  svn::Targets m_targets;
  wxFrame *m_pFrame;

public:
  UpdateAction (wxFrame * frame, Tracer * tr, const svn::Targets & targets);
  void Perform ();
  bool PerformUI ();
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
