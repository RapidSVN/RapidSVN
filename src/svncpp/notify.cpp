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

#include "notify.h"

namespace svn
{

Notify::Notify()
{
}

Notify::~Notify()
{
}

void
Notify::onNotify (
    const char *path,
    svn_wc_notify_action_t action,
    svn_node_kind_t kind,
    const char *mime_type,
    svn_wc_notify_state_t content_state,
    svn_wc_notify_state_t prop_state,
    svn_revnum_t revision)
{
  // do nothing
}

void
Notify::notify (
  void *baton,
  const char *path,
  svn_wc_notify_action_t action,
  svn_node_kind_t kind,
  const char *mime_type,
  svn_wc_notify_state_t content_state,
  svn_wc_notify_state_t prop_state,
  svn_revnum_t revision)
{
  Notify * notify = (Notify *) baton;
  notify->onNotify(path, action, kind, mime_type,
    content_state, prop_state, revision);
}

}
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
