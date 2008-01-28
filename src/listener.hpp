/*
 * ====================================================================
 * Copyright (c) 2002-2008 The RapidSvn Group.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program (in the file GPL.txt); if not, write to 
 * the Free Software Foundation, Inc., 51 Franklin St, Fifth Floor, 
 * Boston, MA  02110-1301  USA
 *
 * This software consists of voluntary contributions made by many
 * individuals.  For exact contribution history, see the revision
 * history and logs, available at http://rapidsvn.tigris.org/.
 * ====================================================================
 */
#ifndef _LISTENER_H
#define _LISTENER_H

// svncpp
#include "svncpp/context_listener.hpp"
#include "svncpp/path.hpp"
#include "svncpp/targets.hpp"

// wxWidgets
#include "wx/string.h"

// forward declarations
class Tracer;
class wxWindow;
class wxCommandEvent;

namespace svn
{
  class Context;
}

/**
 * Class that listens to the @a svn::ContextListener interface
 * and forwards information to the app and requests informations
 * like authentication and such
 */
class Listener : public svn::ContextListener
{
public:
  /**
   * constructor
   *
   * @param parent parent window
   */
  Listener (wxWindow * parent);

  /**
   * destructor
   */
  virtual ~Listener ();

  /**
   * @return Tracer instance
   */
  Tracer * GetTracer ();

  /**
   * Sets the tracer passed as an argument.
   * If own is TRUE, then the @a Listener class
   * is responsible for deleting the tracer.
   */
  void SetTracer (Tracer * t, bool own);

  /**
   * set actions parent window
   *
   * @param parent the parent that will receive events
   */
  void SetParent (wxWindow * parent);

  /**
   * @return parent
   */
  wxWindow * GetParent ();

  /**
   * sets the context
   *
   * @param context
   */
  void SetContext (svn::Context * context);

  /**
   * @return the context of the action
   */
  svn::Context *
  GetContext ();

  /**
   * @see svn::ContextListener
   */
  virtual bool
  contextGetLogin (const std::string & realm,
                   std::string & username,
                   std::string & password,
                   bool & maySave);

  /**
   * @see svn::ContextListener
   */
  virtual void
  contextNotify (const char *path,
                 svn_wc_notify_action_t action,
                 svn_node_kind_t kind,
                 const char *mime_type,
                 svn_wc_notify_state_t content_state,
                 svn_wc_notify_state_t prop_state,
                 svn_revnum_t revision);

  /**
   * @see svn::ContextListener
   */
  virtual bool
  contextGetLogMessage (std::string & msg);

  /**
   * @see svn::ContextListener
   */
  virtual svn::ContextListener::SslServerTrustAnswer
  contextSslServerTrustPrompt (
    const svn::ContextListener::SslServerTrustData & data,
    apr_uint32_t & acceptedFailures);

  /**
   * @see svn::ContextListener
   */
  virtual bool
  contextSslClientCertPrompt (std::string & certFile);

  /**
   * @see svn::ContextListener
   */
  virtual bool
  contextSslClientCertPwPrompt (std::string & password,
                                const std::string & realm,
                                bool & maySave);

  /**
   * @see svn::ContextListener
   */
  virtual bool
  contextCancel ();

  /**
   * shall the ongoing operation be cancelled?
   *
   * @see isCancelled
   *
   * @param value
   *  @li true cancel operation
   *  @li false dont cancel
   */
  void
  cancel (bool value);

  /**
   * @see cancel
   * @return check whether the ongoing operation is to be cancelled
   * @retval true cancel
   * @retval false dont cancel
   */
  bool
  isCancelled () const;

  /**
   * handles @a event that is passed from the parent frame
   * to this class.
   *
   * @param event forwarded event
   */
  void
  handleEvent (wxCommandEvent & event);


protected:
  void Trace (const wxString & msg);

private:
  // implementation specific data
  struct Data;
  Data * m;

  /**
   * disable private default constructor
   */
  Listener ();

  /**
   * disable private copy constructor
   */
  Listener (const Listener &);
};

#endif
/* -----------------------------------------------------------------
 * local variables:
 * eval: (load-file "../rapidsvn-dev.el")
 * end:
 */
