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
#ifndef _REVERT_ACTION_H_INCLUDED_
#define _REVERT_ACTION_H_INCLUDED_

#include "action_thread.h"

class RevertAction:public ActionThread
{
private:
  apr_array_header_t * targets;

public:
  RevertAction (wxFrame * frame, apr_pool_t * __pool,
                Tracer * tr, apr_array_header_t * trgts);

  void Perform ();
  void *Entry ();
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
