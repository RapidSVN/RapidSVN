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

#ifndef _SVN_NOTIFY_H_INCLUDED_
#define _SVN_NOTIFY_H_INCLUDED_

#include "svncpp/notify.h"

class SvnNotify : public svn::Notify
{
private:
  Tracer * _tracer;
  bool sent_first_txdelta;
  bool received_some_change;
  bool is_checkout;
  bool suppress_final_line;

public:
  /**
   * Constructor. Sets the Tracer object.
   */
  SvnNotify (Tracer * tracer);
  ~SvnNotify ();

  /**
   * Overrides the base virtual Notify function to provide its 
   * own native response handling of internal events from the C API.
   */
  void onNotify (const char *path,
    svn_wc_notify_action_t action,
    svn_node_kind_t kind,
    const char *mime_type,
    svn_wc_notify_state_t content_state,
    svn_wc_notify_state_t prop_state,
    long revision); 
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
