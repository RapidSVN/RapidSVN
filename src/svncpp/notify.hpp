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

#ifndef _SVNCPP_NOTIFY_H_
#define _SVNCPP_NOTIFY_H_

// subversion api
#include "svn_types.h"
#include "svn_wc.h"

namespace svn
{

  /**
   * Subversion notification class.
   *
   * Override this class and implement the onNotify method
   * to provide a custom notification handler to the Modify
   * class.
   * If you need to pass extra information to the notification
   * handler then just add these to you overridden class
   */
  class Notify 
  {
  public:
	  Notify();
	  virtual ~Notify();

    /**
     * Handler for Subversion notifications.
     *
     * Override this function to allow Subversion to
     * send notifications
     * @param path on which action happen
     * @param action subversion action, see svn_wc_notify_action_t
     * @param kind node kind of path after action occurred
     * @param mime_type mime type of path after action occurred
     * @param content_state state of content after action occurred
     * @param prop_state state of properties after action occurred
     * @param revision revision number  after action occurred
     */
    virtual void onNotify (
      const char *path,
      svn_wc_notify_action_t action,
      svn_node_kind_t kind,
      const char *mime_type,
      svn_wc_notify_state_t content_state,
      svn_wc_notify_state_t prop_state,
      svn_revnum_t revision); 
  
    /** 
     * notification function passed as svn_wc_notify_func_t
     * @param baton notification instance is passed using this parameter
     * @param path on which action happen
     * @param action subversion action, see svn_wc_notify_action_t
     * @param kind node kind of path after action occurred
     * @param mime_type mime type of path after action occurred
     * @param content_state state of content after action occurred
     * @param prop_state state of properties after action occurred
     * @param revision revision number after action occurred
     */
    static void notify (
      void *baton,
      const char *path,
      svn_wc_notify_action_t action,
      svn_node_kind_t kind,
      const char *mime_type,
      svn_wc_notify_state_t content_state,
      svn_wc_notify_state_t prop_state,
      svn_revnum_t revision);
  };

}

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../../rapidsvn-dev.el")
 * end:
 */
