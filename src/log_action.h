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

#ifndef _LOG_ACTION_H_INCLUDED_
#define _LOG_ACTION_H_INCLUDED_

#include "action_thread.h"
#include "log_dlg.h"

class LogAction : public ActionThread
{
private:
  wxFrame *m_thisframe;
  const char * m_target;
  LogDlg * m_logDialog;

public:
  LogAction (wxFrame * frame, Tracer * tr, const char * target);

  void Perform ();
  void *Entry ();
  void setLogMessage (const char * message);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
