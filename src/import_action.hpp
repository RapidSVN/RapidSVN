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
#ifndef _IMPORT_ACTION_H_INCLUDED_
#define _IMPORT_ACTION_H_INCLUDED_

#include "action_thread.hpp"
#include "import_dlg.hpp"
#include "svncpp/pool.hpp"

class ImportAction:public ActionThread
{
private:
  ImportDlg::sData m_data;
  wxFrame *m_pFrame;

public:
  ImportAction (wxFrame * frame, Tracer * tr);

  void Perform ();
  void *Entry ();
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
