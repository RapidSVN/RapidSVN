/*
 * ====================================================================
 * Copyright (c) 2000 The Apache Software Foundation.  All rights
 * reserved.
 *
 * This software is licensed as described in the file LICENSE.txt,
 * which you should have received as part of this distribution.
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _COPY_ACTION_H_INCLUDED_
#define _COPY_ACTION_H_INCLUDED_

#include "action_thread.h"

class CopyAction:public ActionThread
{
private:
  wxString dest;
  wxString src;
  wxString logMsg;
  apr_array_header_t *targets;
  wxString DestinationPath (wxString src);

public:
  CopyAction (wxFrame * frame, apr_pool_t * __pool, 
               Tracer * tr, apr_array_header_t * trgts);

  void Perform ();
  void *Entry ();
};

#endif
